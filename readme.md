# Ray marching rendering

Current rendering capabilities:  
- Sphere, Cube, Tourus, Prism and Cylinder rendering
- Transforming between shapes over time
- Multiple object in scene
- Modify camera position, FOV, direction
- Checkerboard round on y=0
- Very simple soft shadows
- render real-time (60 fps) or export each frame as image (~15 fps)  
[demo](https://randacek.dev/dl/render_demo.mp4) - (Tourus to Cube transition, rendering to file, Multiple object in scene)

TODO:
- Shape rotating
- Multiple *different* shapes
- Shape transformation control from C (rn hardcoded in GLSL)
- Maybe better animation control from C

see `docs` for explanation what every file does.  

## Build:
Run the `run` script. The executable will start after the build is done.

### Deps:
- cmake
- glfw3
- glew  

(`yay -S glew glfw-x11 cmake --needed`)
