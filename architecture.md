# Architecture

## File hiearchy and short description
```
main.c                                   - Code initialization, main render loop, glfw callbacks.
 |
 +-> opengl.c                            - All interactions with OpenGL are here.
 |    |
 |    +-> shader.c                       - Shader reading, compiling, uniforms functions.
 |    |     |
 |    |     +-> glsl/fragmentShader.glsl - The fragment shader. Contains ray marching implementation.
 |    |     |
 |    |     +-> glsl/vertexShader.glsl   - The vertex shader. Nothing fancy here.
 |    |
 |    +-> fileio.c                       - Reads pixel bytes from memory and dumps them into a bmp file
 |
 +-> scene.c                             - Scene and everything in it is defined and updated here.
      |
      +-> camera.c                       - Calculate edge rays from camera position and facing direction. Edge rays are
            |                            - used to interpolate between, when calculating what direction to cast the rays in.
            |
            +-> vector.c                 - Definitions of vectors and vector functions.
```
## Scene objects

### Camera

Has position and 4 edge rays that define the edge of rendered image.  
Use constructor in `camera.c` to calculate edge rays from one direction vector and screen width & height

### Light source

Has only position

### Shapes

#### Sphere

Has xyz position, rgb color and float radius

#### Cube

Has xyz position, rgb color and xyz scale
