#define _POSIX_C_SOURCE 200809L
#include "GLFW/glfw3.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/* Tiny GLFW-compatible capture host. It only implements the subset used by
   Luna Files and backs it with a surfaceless EGL pbuffer. */
typedef void* EGLDisplay; typedef void* EGLConfig; typedef void* EGLContext; typedef void* EGLSurface;
typedef void* EGLClientBuffer; typedef void* EGLNativeDisplayType; typedef int EGLint; typedef unsigned int EGLBoolean; typedef unsigned int EGLenum;
#define EGL_FALSE 0
#define EGL_TRUE 1
#define EGL_NONE 0x3038
#define EGL_SURFACE_TYPE 0x3033
#define EGL_PBUFFER_BIT 0x0001
#define EGL_RED_SIZE 0x3024
#define EGL_GREEN_SIZE 0x3023
#define EGL_BLUE_SIZE 0x3022
#define EGL_ALPHA_SIZE 0x3021
#define EGL_RENDERABLE_TYPE 0x3040
#define EGL_OPENGL_BIT 0x0008
#define EGL_WIDTH 0x3057
#define EGL_HEIGHT 0x3056
#define EGL_OPENGL_API 0x30A2
#define EGL_CONTEXT_MAJOR_VERSION_KHR 0x3098
#define EGL_CONTEXT_MINOR_VERSION_KHR 0x30FB
#define EGL_CONTEXT_OPENGL_PROFILE_MASK_KHR 0x30FD
#define EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT_KHR 0x00000001
#define EGL_PLATFORM_SURFACELESS_MESA 0x31DD
#define EGL_DEFAULT_DISPLAY ((EGLNativeDisplayType)0)
#define EGL_NO_DISPLAY ((EGLDisplay)0)
#define EGL_NO_CONTEXT ((EGLContext)0)
#define EGL_NO_SURFACE ((EGLSurface)0)

typedef EGLDisplay (*PFNEGLGETDISPLAY)(EGLNativeDisplayType);
typedef EGLBoolean (*PFNEGLINITIALIZE)(EGLDisplay,EGLint*,EGLint*);
typedef EGLBoolean (*PFNEGLCHOOSECONFIG)(EGLDisplay,const EGLint*,EGLConfig*,EGLint,EGLint*);
typedef EGLBoolean (*PFNEGLBINDAPI)(EGLenum);
typedef EGLSurface (*PFNEGLCREATEPBUFFERSURFACE)(EGLDisplay,EGLConfig,const EGLint*);
typedef EGLContext (*PFNEGLCREATECONTEXT)(EGLDisplay,EGLConfig,EGLContext,const EGLint*);
typedef EGLBoolean (*PFNEGLMAKECURRENT)(EGLDisplay,EGLSurface,EGLSurface,EGLContext);
typedef EGLBoolean (*PFNEGLSWAPBUFFERS)(EGLDisplay,EGLSurface);
typedef EGLBoolean (*PFNEGLDESTROYSURFACE)(EGLDisplay,EGLSurface);
typedef EGLBoolean (*PFNEGLDESTROYCONTEXT)(EGLDisplay,EGLContext);
typedef EGLBoolean (*PFNEGLTERMINATE)(EGLDisplay);
typedef void* (*PFNEGLGETPROCADDRESS)(const char*);
typedef EGLDisplay (*PFNEGLGETPLATFORMDISPLAYEXT)(EGLenum,void*,const EGLint*);

typedef void (*PFNGLFINISH)(void);
typedef void (*PFNGLPIXELSTOREI)(GLenum,GLint);
typedef void (*PFNGLREADPIXELS)(GLint,GLint,GLsizei,GLsizei,GLenum,GLenum,void*);

struct GLFWcursor { int shape; };
struct GLFWwindow {
    int width,height,requested_width,requested_height,x,y,should_close,maximized;
    int emitted_resize, poll_count, swap_count, settings_sent;
    EGLSurface surface; EGLContext context;
    GLFWcursorposfun cursor_cb; GLFWmousebuttonfun mouse_cb; GLFWscrollfun scroll_cb;
    GLFWkeyfun key_cb; GLFWcharfun char_cb; GLFWwindowsizefun size_cb;
    GLFWframebuffersizefun fb_cb; GLFWwindowcontentscalefun scale_cb;
};

static void *egl_lib,*gl_lib; static EGLDisplay display; static GLFWwindow *current_window;
static GLFWerrorfun error_cb; static char *clipboard; static struct timespec epoch;
static PFNEGLGETDISPLAY p_eglGetDisplay; static PFNEGLINITIALIZE p_eglInitialize; static PFNEGLCHOOSECONFIG p_eglChooseConfig;
static PFNEGLBINDAPI p_eglBindAPI; static PFNEGLCREATEPBUFFERSURFACE p_eglCreatePbufferSurface; static PFNEGLCREATECONTEXT p_eglCreateContext;
static PFNEGLMAKECURRENT p_eglMakeCurrent; static PFNEGLSWAPBUFFERS p_eglSwapBuffers; static PFNEGLDESTROYSURFACE p_eglDestroySurface;
static PFNEGLDESTROYCONTEXT p_eglDestroyContext; static PFNEGLTERMINATE p_eglTerminate; static PFNEGLGETPROCADDRESS p_eglGetProcAddress;
static EGLConfig egl_config;

static void report(int code,const char*msg){ if(error_cb) error_cb(code,msg); else fprintf(stderr,"capture-glfw: %s\n",msg); }
static void* sym(void*lib,const char*n){ return dlsym(lib,n); }
static int env_int(const char*n,int def){ const char*s=getenv(n); if(!s||!*s)return def; int v=atoi(s); return v>0?v:def; }
static void pump(GLFWwindow*w){
    if(!w)return; w->poll_count++;
    if(!w->emitted_resize){w->emitted_resize=1; if(w->size_cb)w->size_cb(w,w->width,w->height); if(w->fb_cb)w->fb_cb(w,w->width,w->height); if(w->scale_cb)w->scale_cb(w,1.0f,1.0f);}
    const char*sc=getenv("LUNA_CAPTURE_SCENARIO");
    if(sc && !strcmp(sc,"settings") && !w->settings_sent && w->poll_count>=2 && w->key_cb){
        w->settings_sent=1; w->key_cb(w,GLFW_KEY_COMMA,0,GLFW_PRESS,GLFW_MOD_CONTROL); w->key_cb(w,GLFW_KEY_COMMA,0,GLFW_RELEASE,GLFW_MOD_CONTROL);
    }
}
static void write_capture(GLFWwindow*w){
    const char*path=getenv("LUNA_CAPTURE_PATH"); if(!path||!*path)return;
    size_t n=(size_t)w->width*(size_t)w->height*3; unsigned char*raw=(unsigned char*)malloc(n); if(!raw)return;
    PFNGLPIXELSTOREI ps=(PFNGLPIXELSTOREI)glfwGetProcAddress("glPixelStorei"); PFNGLREADPIXELS rp=(PFNGLREADPIXELS)glfwGetProcAddress("glReadPixels");
    if(ps)ps(GL_PACK_ALIGNMENT,1); if(!rp){free(raw);return;} rp(0,0,w->width,w->height,GL_RGB,GL_UNSIGNED_BYTE,raw);
    FILE*f=fopen(path,"wb"); if(f){fprintf(f,"P6\n%d %d\n255\n",w->width,w->height); for(int y=w->height-1;y>=0;y--)fwrite(raw+(size_t)y*w->width*3,1,(size_t)w->width*3,f); fclose(f); fprintf(stderr,"capture-glfw: wrote %s (%dx%d)\n",path,w->width,w->height);} free(raw);
}
GLFWerrorfun glfwSetErrorCallback(GLFWerrorfun cb){GLFWerrorfun old=error_cb;error_cb=cb;return old;}
int glfwInit(void){
    clock_gettime(CLOCK_MONOTONIC,&epoch); egl_lib=dlopen("libEGL.so.1",RTLD_NOW|RTLD_LOCAL); gl_lib=dlopen("libGL.so.1",RTLD_NOW|RTLD_LOCAL);
    if(!egl_lib||!gl_lib){report(1,"could not load libEGL/libGL");return 0;}
#define LOAD(name) p_##name=(void*)sym(egl_lib,#name); if(!p_##name){report(2,"missing " #name);return 0;}
    LOAD(eglGetDisplay); LOAD(eglInitialize); LOAD(eglChooseConfig); LOAD(eglBindAPI); LOAD(eglCreatePbufferSurface); LOAD(eglCreateContext); LOAD(eglMakeCurrent); LOAD(eglSwapBuffers); LOAD(eglDestroySurface); LOAD(eglDestroyContext); LOAD(eglTerminate); LOAD(eglGetProcAddress);
#undef LOAD
    PFNEGLGETPLATFORMDISPLAYEXT gp=(PFNEGLGETPLATFORMDISPLAYEXT)p_eglGetProcAddress("eglGetPlatformDisplayEXT");
    display=gp?gp(EGL_PLATFORM_SURFACELESS_MESA,NULL,NULL):p_eglGetDisplay(EGL_DEFAULT_DISPLAY);
    EGLint maj=0,min=0; if(display==EGL_NO_DISPLAY||!p_eglInitialize(display,&maj,&min)){display=p_eglGetDisplay(EGL_DEFAULT_DISPLAY); if(display==EGL_NO_DISPLAY||!p_eglInitialize(display,&maj,&min)){report(3,"eglInitialize failed");return 0;}}
    EGLint attrs[]={EGL_SURFACE_TYPE,EGL_PBUFFER_BIT,EGL_RENDERABLE_TYPE,EGL_OPENGL_BIT,EGL_RED_SIZE,8,EGL_GREEN_SIZE,8,EGL_BLUE_SIZE,8,EGL_ALPHA_SIZE,8,EGL_NONE}; EGLint count=0;
    if(!p_eglChooseConfig(display,attrs,&egl_config,1,&count)||count<1||!p_eglBindAPI(EGL_OPENGL_API)){report(4,"no OpenGL pbuffer EGL config");return 0;} return 1;
}
void glfwTerminate(void){ if(display)p_eglTerminate(display); display=0; if(gl_lib)dlclose(gl_lib); if(egl_lib)dlclose(egl_lib); gl_lib=egl_lib=NULL; free(clipboard);clipboard=NULL;}
void glfwDefaultWindowHints(void){} void glfwWindowHint(int h,int v){(void)h;(void)v;}
GLFWwindow* glfwCreateWindow(int width,int height,const char*title,GLFWmonitor*m,GLFWwindow*share){(void)title;(void)m;(void)share; GLFWwindow*w=calloc(1,sizeof(*w)); if(!w)return NULL; w->requested_width=width;w->requested_height=height;w->width=env_int("LUNA_CAPTURE_WIDTH",width);w->height=env_int("LUNA_CAPTURE_HEIGHT",height);
    EGLint pa[]={EGL_WIDTH,w->width,EGL_HEIGHT,w->height,EGL_NONE}; w->surface=p_eglCreatePbufferSurface(display,egl_config,pa); if(w->surface==EGL_NO_SURFACE){free(w);report(5,"eglCreatePbufferSurface failed");return NULL;}
    EGLint ca[]={EGL_CONTEXT_MAJOR_VERSION_KHR,3,EGL_CONTEXT_MINOR_VERSION_KHR,3,EGL_CONTEXT_OPENGL_PROFILE_MASK_KHR,EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT_KHR,EGL_NONE}; w->context=p_eglCreateContext(display,egl_config,EGL_NO_CONTEXT,ca); if(w->context==EGL_NO_CONTEXT){EGLint fallback[]={EGL_NONE};w->context=p_eglCreateContext(display,egl_config,EGL_NO_CONTEXT,fallback);} if(w->context==EGL_NO_CONTEXT){p_eglDestroySurface(display,w->surface);free(w);report(6,"eglCreateContext failed");return NULL;} return w;}
void glfwDestroyWindow(GLFWwindow*w){if(!w)return;if(current_window==w){p_eglMakeCurrent(display,EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT);current_window=NULL;}if(w->context)p_eglDestroyContext(display,w->context);if(w->surface)p_eglDestroySurface(display,w->surface);free(w);}
void glfwMakeContextCurrent(GLFWwindow*w){current_window=w;if(w)p_eglMakeCurrent(display,w->surface,w->surface,w->context);else p_eglMakeCurrent(display,EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT);}
GLFWglproc glfwGetProcAddress(const char*n){void*p=gl_lib?sym(gl_lib,n):NULL;if(!p&&p_eglGetProcAddress)p=p_eglGetProcAddress(n);return (GLFWglproc)p;}
void glfwSwapInterval(int i){(void)i;} void glfwSwapBuffers(GLFWwindow*w){PFNGLFINISH f=(PFNGLFINISH)glfwGetProcAddress("glFinish");if(f)f();p_eglSwapBuffers(display,w->surface);w->swap_count++;int target=env_int("LUNA_CAPTURE_FRAME",8);if(w->swap_count>=target){write_capture(w);w->should_close=1;}}
void glfwPollEvents(void){pump(current_window);} void glfwWaitEventsTimeout(double t){if(t>0){if(t>0.02)t=0.02;struct timespec ts={(time_t)t,(long)((t-(time_t)t)*1e9)};nanosleep(&ts,NULL);}pump(current_window);}
double glfwGetTime(void){struct timespec n;clock_gettime(CLOCK_MONOTONIC,&n);return (n.tv_sec-epoch.tv_sec)+(n.tv_nsec-epoch.tv_nsec)/1e9;}
int glfwWindowShouldClose(GLFWwindow*w){return w?w->should_close:1;} void glfwSetWindowShouldClose(GLFWwindow*w,int v){if(w)w->should_close=v;} void glfwShowWindow(GLFWwindow*w){(void)w;}
void glfwGetWindowSize(GLFWwindow*w,int*a,int*b){if(a)*a=w?w->width:0;if(b)*b=w?w->height:0;} void glfwGetFramebufferSize(GLFWwindow*w,int*a,int*b){glfwGetWindowSize(w,a,b);}
void glfwSetWindowSize(GLFWwindow*w,int a,int b){if(!w)return;w->width=a;w->height=b;if(w->size_cb)w->size_cb(w,a,b);if(w->fb_cb)w->fb_cb(w,a,b);} void glfwSetWindowSizeLimits(GLFWwindow*w,int a,int b,int c,int d){(void)w;(void)a;(void)b;(void)c;(void)d;}
void glfwGetWindowPos(GLFWwindow*w,int*a,int*b){if(a)*a=w?w->x:0;if(b)*b=w?w->y:0;} void glfwSetWindowPos(GLFWwindow*w,int a,int b){if(w){w->x=a;w->y=b;}}
int glfwGetWindowAttrib(GLFWwindow*w,int a){if(!w)return 0;if(a==GLFW_MAXIMIZED)return w->maximized;return 0;} void glfwMaximizeWindow(GLFWwindow*w){if(w)w->maximized=1;} void glfwRestoreWindow(GLFWwindow*w){if(w)w->maximized=0;} void glfwIconifyWindow(GLFWwindow*w){(void)w;} void glfwSetWindowTitle(GLFWwindow*w,const char*t){(void)w;(void)t;}
void glfwGetWindowContentScale(GLFWwindow*w,float*a,float*b){(void)w;if(a)*a=1;if(b)*b=1;} void glfwGetCursorPos(GLFWwindow*w,double*a,double*b){(void)w;if(a)*a=0;if(b)*b=0;} int glfwGetMouseButton(GLFWwindow*w,int b){(void)w;(void)b;return GLFW_RELEASE;}
GLFWcursor* glfwCreateStandardCursor(int s){GLFWcursor*c=calloc(1,sizeof(*c));if(c)c->shape=s;return c;}void glfwDestroyCursor(GLFWcursor*c){free(c);}void glfwSetCursor(GLFWwindow*w,GLFWcursor*c){(void)w;(void)c;}
void glfwSetClipboardString(GLFWwindow*w,const char*s){(void)w;free(clipboard);clipboard=strdup(s?s:"");}const char*glfwGetClipboardString(GLFWwindow*w){(void)w;return clipboard?clipboard:"";}
#define SETCB(name,field,type) type name(GLFWwindow*w,type cb){type old=w?w->field:NULL;if(w)w->field=cb;return old;}
SETCB(glfwSetCursorPosCallback,cursor_cb,GLFWcursorposfun) SETCB(glfwSetMouseButtonCallback,mouse_cb,GLFWmousebuttonfun) SETCB(glfwSetScrollCallback,scroll_cb,GLFWscrollfun) SETCB(glfwSetKeyCallback,key_cb,GLFWkeyfun) SETCB(glfwSetCharCallback,char_cb,GLFWcharfun) SETCB(glfwSetWindowSizeCallback,size_cb,GLFWwindowsizefun) SETCB(glfwSetFramebufferSizeCallback,fb_cb,GLFWframebuffersizefun) SETCB(glfwSetWindowContentScaleCallback,scale_cb,GLFWwindowcontentscalefun)
#undef SETCB
