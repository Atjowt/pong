## Build & Run
Build the project using `cmake ..` inside the `build` directory. Then run `make` and it should build. Requires `cmake >= 3.0.0` and `OpenGL >= 4.50`.

## Rendering
The rendering is done in batches by sending rectangle data to an SSBO and constructing the quads on the GPU itself. Yes, it is very overkill.
