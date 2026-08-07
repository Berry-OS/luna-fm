#ifndef GLFW3_H
#define GLFW3_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

/* Minimal OpenGL declarations normally pulled in by GLFW. */
typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef void GLvoid;
typedef signed char GLbyte;
typedef short GLshort;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned int GLuint;
typedef float GLfloat;
typedef ptrdiff_t GLsizeiptr;
typedef ptrdiff_t GLintptr;
typedef char GLchar;

#define GL_FALSE 0
#define GL_TRUE 1
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_DEPTH_BUFFER_BIT 0x00000100
#define GL_STENCIL_BUFFER_BIT 0x00000400
#define GL_POINTS 0x0000
#define GL_LINES 0x0001
#define GL_LINE_LOOP 0x0002
#define GL_LINE_STRIP 0x0003
#define GL_TRIANGLES 0x0004
#define GL_TRIANGLE_STRIP 0x0005
#define GL_TRIANGLE_FAN 0x0006
#define GL_QUADS 0x0007
#define GL_ZERO 0
#define GL_ONE 1
#define GL_SRC_ALPHA 0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_DST_ALPHA 0x0304
#define GL_DST_COLOR 0x0306
#define GL_ONE_MINUS_DST_COLOR 0x0307
#define GL_ONE_MINUS_DST_ALPHA 0x0305
#define GL_BLEND 0x0BE2
#define GL_SCISSOR_TEST 0x0C11
#define GL_TEXTURE_2D 0x0DE1
#define GL_UNSIGNED_BYTE 0x1401
#define GL_FLOAT 0x1406
#define GL_RGBA 0x1908
#define GL_RGB 0x1907
#define GL_ALPHA 0x1906
#define GL_LINEAR 0x2601
#define GL_NEAREST 0x2600
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_UNPACK_ALIGNMENT 0x0CF5
#define GL_PACK_ALIGNMENT 0x0D05

void glViewport(GLint x, GLint y, GLsizei width, GLsizei height);

typedef struct GLFWwindow GLFWwindow;
typedef struct GLFWcursor GLFWcursor;
typedef struct GLFWmonitor GLFWmonitor;
typedef void (*GLFWglproc)(void);
typedef void (*GLFWerrorfun)(int,const char*);
typedef void (*GLFWwindowposfun)(GLFWwindow*,int,int);
typedef void (*GLFWwindowsizefun)(GLFWwindow*,int,int);
typedef void (*GLFWframebuffersizefun)(GLFWwindow*,int,int);
typedef void (*GLFWwindowcontentscalefun)(GLFWwindow*,float,float);
typedef void (*GLFWcursorposfun)(GLFWwindow*,double,double);
typedef void (*GLFWmousebuttonfun)(GLFWwindow*,int,int,int);
typedef void (*GLFWscrollfun)(GLFWwindow*,double,double);
typedef void (*GLFWkeyfun)(GLFWwindow*,int,int,int,int);
typedef void (*GLFWcharfun)(GLFWwindow*,unsigned int);

#define GLFW_VERSION_MAJOR 3
#define GLFW_VERSION_MINOR 3
#define GLFW_TRUE 1
#define GLFW_FALSE 0
#define GLFW_DONT_CARE -1
#define GLFW_RELEASE 0
#define GLFW_PRESS 1
#define GLFW_REPEAT 2
#define GLFW_MOUSE_BUTTON_LEFT 0
#define GLFW_MOUSE_BUTTON_RIGHT 1
#define GLFW_MOUSE_BUTTON_MIDDLE 2
#define GLFW_MOD_SHIFT 0x0001
#define GLFW_MOD_CONTROL 0x0002
#define GLFW_MOD_ALT 0x0004
#define GLFW_MOD_SUPER 0x0008
#define GLFW_KEY_SPACE 32
#define GLFW_KEY_COMMA 44
#define GLFW_KEY_A 65
#define GLFW_KEY_C 67
#define GLFW_KEY_L 76
#define GLFW_KEY_V 86
#define GLFW_KEY_X 88
#define GLFW_KEY_ESCAPE 256
#define GLFW_KEY_ENTER 257
#define GLFW_KEY_TAB 258
#define GLFW_KEY_BACKSPACE 259
#define GLFW_KEY_DELETE 261
#define GLFW_KEY_RIGHT 262
#define GLFW_KEY_LEFT 263
#define GLFW_KEY_DOWN 264
#define GLFW_KEY_UP 265
#define GLFW_KEY_PAGE_UP 266
#define GLFW_KEY_PAGE_DOWN 267
#define GLFW_KEY_HOME 268
#define GLFW_KEY_END 269
#define GLFW_KEY_F2 291
#define GLFW_KEY_F4 293
#define GLFW_KEY_KP_ENTER 335

#define GLFW_RESIZABLE 0x00020003
#define GLFW_VISIBLE 0x00020004
#define GLFW_DECORATED 0x00020005
#define GLFW_MAXIMIZED 0x00020008
#define GLFW_TRANSPARENT_FRAMEBUFFER 0x0002000A
#define GLFW_SCALE_TO_MONITOR 0x0002200C
#define GLFW_DOUBLEBUFFER 0x00021010
#define GLFW_CONTEXT_VERSION_MAJOR 0x00022002
#define GLFW_CONTEXT_VERSION_MINOR 0x00022003
#define GLFW_OPENGL_PROFILE 0x00022008
#define GLFW_OPENGL_CORE_PROFILE 0x00032001
#define GLFW_ARROW_CURSOR 0x00036001
#define GLFW_IBEAM_CURSOR 0x00036002
#define GLFW_CROSSHAIR_CURSOR 0x00036003
#define GLFW_HAND_CURSOR 0x00036004
#define GLFW_HRESIZE_CURSOR 0x00036005
#define GLFW_VRESIZE_CURSOR 0x00036006

GLFWerrorfun glfwSetErrorCallback(GLFWerrorfun cbfun);
int glfwInit(void);
void glfwTerminate(void);
void glfwDefaultWindowHints(void);
void glfwWindowHint(int hint,int value);
GLFWwindow* glfwCreateWindow(int width,int height,const char* title,GLFWmonitor* monitor,GLFWwindow* share);
void glfwDestroyWindow(GLFWwindow* window);
void glfwMakeContextCurrent(GLFWwindow* window);
GLFWglproc glfwGetProcAddress(const char* procname);
void glfwSwapInterval(int interval);
void glfwSwapBuffers(GLFWwindow* window);
void glfwPollEvents(void);
void glfwWaitEventsTimeout(double timeout);
double glfwGetTime(void);
int glfwWindowShouldClose(GLFWwindow* window);
void glfwSetWindowShouldClose(GLFWwindow* window,int value);
void glfwShowWindow(GLFWwindow* window);
void glfwGetWindowSize(GLFWwindow* window,int* width,int* height);
void glfwGetFramebufferSize(GLFWwindow* window,int* width,int* height);
void glfwSetWindowSize(GLFWwindow* window,int width,int height);
void glfwSetWindowSizeLimits(GLFWwindow* window,int minw,int minh,int maxw,int maxh);
void glfwGetWindowPos(GLFWwindow* window,int* xpos,int* ypos);
void glfwSetWindowPos(GLFWwindow* window,int xpos,int ypos);
int glfwGetWindowAttrib(GLFWwindow* window,int attrib);
void glfwMaximizeWindow(GLFWwindow* window);
void glfwRestoreWindow(GLFWwindow* window);
void glfwIconifyWindow(GLFWwindow* window);
void glfwSetWindowTitle(GLFWwindow* window,const char* title);
void glfwGetWindowContentScale(GLFWwindow* window,float* xscale,float* yscale);
void glfwGetCursorPos(GLFWwindow* window,double* xpos,double* ypos);
int glfwGetMouseButton(GLFWwindow* window,int button);
GLFWcursor* glfwCreateStandardCursor(int shape);
void glfwDestroyCursor(GLFWcursor* cursor);
void glfwSetCursor(GLFWwindow* window,GLFWcursor* cursor);
void glfwSetClipboardString(GLFWwindow* window,const char* string);
const char* glfwGetClipboardString(GLFWwindow* window);
GLFWcursorposfun glfwSetCursorPosCallback(GLFWwindow* window,GLFWcursorposfun cbfun);
GLFWmousebuttonfun glfwSetMouseButtonCallback(GLFWwindow* window,GLFWmousebuttonfun cbfun);
GLFWscrollfun glfwSetScrollCallback(GLFWwindow* window,GLFWscrollfun cbfun);
GLFWkeyfun glfwSetKeyCallback(GLFWwindow* window,GLFWkeyfun cbfun);
GLFWcharfun glfwSetCharCallback(GLFWwindow* window,GLFWcharfun cbfun);
GLFWwindowsizefun glfwSetWindowSizeCallback(GLFWwindow* window,GLFWwindowsizefun cbfun);
GLFWframebuffersizefun glfwSetFramebufferSizeCallback(GLFWwindow* window,GLFWframebuffersizefun cbfun);
GLFWwindowcontentscalefun glfwSetWindowContentScaleCallback(GLFWwindow* window,GLFWwindowcontentscalefun cbfun);

#ifdef __cplusplus
}
#endif
#endif
