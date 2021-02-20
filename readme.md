# Ray marching rendering

![normal demo](https://randacek.dev/dl/rendering/demo.png)  
![blending demo](https://randacek.dev/dl/rendering/blending.png)  
![Cool glow](https://randacek.dev/dl/rendering/glow.png)  

Current rendering capabilities:  
- Sphere, Cube, Tourus, Prism and Cylinder rendering
- Transforming between shapes over time
- Multiple different object in scene
- Modify camera position, FOV, direction
- Checkerboard floor on y=0
- Very simple soft shadows
- Render real-time (60 fps) or export each frame as image (~15 fps)  

TODO:
- Shape rotating
- Shape transformation control from C (rn hardcoded in GLSL)
- Maybe better animation control from C
- `#import`s seems to be messy
- Split fragment shader into multiple parts

see `archtecture.md` for explanation what every file does.  

## Demos
[Transform demo](https://randacek.dev/dl/rendering/transform_demo.mp4) - (Tourus to Cube transition, rendering to file, Multiple object in scene)  
[Move demo](https://randacek.dev/dl/rendering/move_demo.mp4) - (WASD & Mouse movement, Multiple object in scene)  

## Build:
`make run`

### Deps:
- make
- glfw3
- glew  

(`yay -S glew glfw-x11 --needed`)
