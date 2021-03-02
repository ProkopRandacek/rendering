# Architecture

## File hiearchy and short description
```
main.c                                      - Code initialization, main render loop, glfw callbacks.
 |
 +-> opengl.c                               - All interactions with OpenGL are here.
 |    |
 |    +-> shader.c                          - Shader reading, compiling, uniforms functions.
 |    |     |                               
 |    |     +-> glsl/fragmentShader.glsl    - The fragment shader. Contains ray marching implementation.
 |    |     |                               
 |    |     +-> glsl/vertexShader.glsl      - The vertex shader. Nothing fancy here.
 |    |                                     
 |    +-> fileio.c                          - Reads pixel bytes from memory and dumps them into a bmp file
 |
 +-> scene.c                                - Scene and everything in it is defined and updated here.
 |    |
 |    +-> camera.c                          - Calculate edge rays from camera position and facing direction. Edge rays are
 |    |     |                               - used to interpolate between, when calculating what direction to cast the rays in.
 |    |     |
 |    |     +-> vector.c                    - Definitions of vectors and vector functions.
 |    |
 |    +-> shapes.c                          - Definitions of shapes and shape functions.
 |    |
 |    +-> shapeSerialization.c              - Functions that export shape structures to float arrays.
 |                                          - Is used to sending data over to the GPU.
 |
 +-> input.c                                - Input handling
```

## Shapes

Are defined in `shapes.h`. When serializing into a float array, following rules apply for all shapes:

- Max length of a shape is 10 floats.
- Floats on positions:
  - 0 1 2 are position (or starting position of a shape, for example cylinder)
  - 3 4 5 are color
  - 9 is radius (radius of a circle || radius of a smooth edge of a cube)

Shape specific rules:
### Sphere

6 7 8 are not used.

#### Cube

7 8 9 is XYZ scale  

#### Cylinder

1 2 3 is XYZ center of the bottom of the cylinder
7 8 9 is XYZ center of the top of the cylinder
