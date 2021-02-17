# Ray marching rendering

Current rendering capabilities:  
[demo](https://randacek.dev/dl/render_demo.mp4)  
- Sphere, Cube, Tourus, Prism and Cylinder rendering
- Transforming between shapes over time
- Multiple object in scene
- Modify camera position, FOV, direction
- Checkerboard round on y=0
- Very simple soft shadows
- render real-time or export each frame as image (not realtime, ~10 fps)  

TODO:
- Shape rotating
- Multiple *different* shapes
- Shape transformation control from C (rn hardcoded in GLSL)
- Maybe better animation control from C
- Better rotation over time control from C

see `code` for explanation what every file does.  

## Build:
Run the `run` script. The executable will start after the build is done.

### Deps:
- cmake
- glfw3
- glew  

(`yay -S glew glfw-x11 cmake --needed`)
