# Ray marching rendering

![normal demo](https://randacek.dev/p/demo.png)  
![blending demo](https://randacek.dev/p/blending.png)  
![CSG demo](https://randacek.dev/p/csg.png)  

Current rendering capabilities:  
- [Constructive solid geometry](https://en.wikipedia.org/wiki/Constructive_solid_geometry)
- Sphere, Cube, Box Frame, Torus, Capped Torus, Link, Plane, Hexagonal Prism, Triangular Prism, Capsule, Round Cone, Pyramid and Cylinder rendering
- Transforming between shapes over time
- WASD + mouse controls
- Checkerboard floor
- Very simple soft shadows
- Works realtime

TODO:
- Reflections
- [Better lighting](https://en.wikipedia.org/wiki/Computer_graphics_lighting)
- Read scene configuration from file
- Bounding box for complicated groups far away from the ray
- Relative position for shapes in subgroups?
- Shape rotating
- `#import`s seems to be messy
- Split fragment shader into multiple parts

see [`archtecture.md`](architecture.md) for explanation what every file does.  

## Demos
[Transform demo](https://randacek.dev/p/transform_demo.mp4) - (Tourus to Cube transition, rendering to file, Multiple object in scene)  
[Move demo](https://randacek.dev/p/move_demo.mp4) - (WASD & Mouse movement, Multiple object in scene)  

## Build:
`make run` or just `make` to build the binary (in `./build/`; `*.glsl` files from `./build/` need to be in the `$PWD` folder when executing the program for them to be found)  
  
Compiled birary can be found [here](https://randacek.dev/p/build.7z); [checksums](https://randacek.dev/p/build.7z.checksum) (you still need glfw3 and glew)

### Deps:
- make
- glfw3
- glew  

(`yay -S glew glfw-x11 --needed`)
