# Ray marching rendering

![normal demo](https://randacek.dev/dl/rendering/demo.png)  
![blending demo](https://randacek.dev/dl/rendering/blending.png)  
![Cool glow](https://randacek.dev/dl/rendering/glow.png)  

Current rendering capabilities:  
- Sphere and Cube rendering
- Transforming between shapes over time
- Multiple different object in scene
- Modify camera position, FOV, direction
- Checkerboard floor (position hard coded in GLSL for now)
- Very simple soft shadows
- Render real-time (60 fps) or export each frame as image (~15 fps on my pc)  

TODO:
- Sort shapes in scene into a btree instead of array.
- Shape combination operation between pairs of shapes (pairs from the btree)
- Split fragment shader into multiple parts
- Shape transformation control from C (rn hard coded in GLSL)
- Shape rotating
- I probably can use `#import`s correctly. (But it works so whatever i guess)

See [`archtecture.md`](architecture.md) for explanation what every file does.  

## Demos
[Transform demo](https://randacek.dev/dl/rendering/transform_demo.mp4) - (Tourus to Cube transition, rendering to file, Multiple object in scene)  
[Move demo](https://randacek.dev/dl/rendering/move_demo.mp4) - (WASD & Mouse movement, Multiple object in scene)  

## Build:
`make run` to (re)build and run the binary.  
`make` to build the binary (in `./build/`; `*.glsl` files from `./build/` need to be in the `$PWD` folder when executing the program for them to be found)
(experimental) `make winbuild` to (re)build .exe file in `./winbuild/` same rules apply for `*.glsl` files as when building for Linux.

### Deps:
- make
- glfw3
- glew  

(`yay -S glew glfw-x11 --needed`)
