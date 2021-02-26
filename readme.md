# Ray marching rendering

![normal demo](https://randacek.dev/p/demo.png)  
![blending demo](https://randacek.dev/p/blending.png)  
![Cool glow](https://randacek.dev/p/glow.png)  

Current rendering capabilities:  
- Sphere and Cube rendering
- Transforming between shapes over time
- Multiple different object in scene
- Modify camera position, FOV, direction
- Checkerboard floor on y=0
- Very simple soft shadows
- Render real-time (60 fps) or export each frame as image (~15 fps on my pc)  

TODO:
- Shape rotating
- Shape transformation control from C (rn hardcoded in GLSL)
- Maybe better animation control from C
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
