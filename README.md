## Build & Run
Build the project using `cmake ..` inside the `build` directory. Then run `make` and it should build. Requires `cmake >= 3.0.0`, `OpenGL >= 4.50`, `GLAD`, and `GLFW`.

## Rendering
The rendering is done in batches and is instanced. Rectangle data is sent to a SSBO and the quads are constructed inside the shader itself.

Yes, it is very overkill.
