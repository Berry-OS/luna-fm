# Screenshot GLFW compatibility layer

This directory is only for reproducible README screenshots on Linux hosts that
have Mesa/OpenGL runtime libraries but no GLFW development package.

`glfw_capture.c` implements only the GLFW calls used by Luna Files and creates a
surfaceless EGL pbuffer. The application itself is not replaced or mocked:
`luna-fm.c`, `luna-ui/luna-ui.h`, and `luna-ui/luna-window.h` are compiled and
rendered unchanged.

```sh
./tools/screenshot-glfw/capture.sh /tmp/luna-files.ppm "$HOME"
```

Useful environment variables are `LUNA_CAPTURE_WIDTH`, `LUNA_CAPTURE_HEIGHT`,
`LUNA_CAPTURE_FRAME`, and `LUNA_CAPTURE_SCENARIO=settings`.
