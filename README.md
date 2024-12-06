# Aurora Scene Playground
My Graphics Playground with Aurora Simulation Using C++ and OpenGL

## Final Visual Effect
![image](https://github.com/user-attachments/assets/630aa805-072a-4e10-b654-09a3126be4a2)
![image](https://github.com/user-attachments/assets/8a7e837f-6087-47f9-a314-8bb9639de404)
![image](https://github.com/user-attachments/assets/1d00c1b2-34cc-4098-85ab-41bedbd8c191)
![image](https://github.com/user-attachments/assets/2e19d326-9262-4f48-b6d6-86b8d8d5ce94)



## Project Structure
1. ./common
   - /objects
   - /textures
2. ./include
   - /glad: our Multi-Language GL/GLES/EGL/GLX/WGL Loader-Generator
   - /glm: a header only C++ mathematics library for graphics software based on the OpenGL
   - /KHR: khrplatform header
   - Camera.hpp: set up an OpenGL camera, store camera information and handle camera movement
   - Error.hpp: error handling in OpenGL
   - Geometry.hpp: store vertice and triangle information
   - globals.hpp(TBD): globals should be separated to an independent header
   - Image.hpp: load, manipulate, and retrieve pixel data from images
   - Object.hpp: an abstraction to create multiple objects
   - ObjectManager.hpp(TBD): class to manage creation of objects 
   - Renderer.hpp: responsible for drawing everything, contains a scenegraph node and a camera
   - SceneNode.hpp: helps organize a large 3D graphics scene
   - SDLGraphicsProgram.hpp: set up a full graphics program using SDL
   - Shader.hpp: an abstraction for creating, compiling, linking, and managing OpenGL shaders
   - Skybox.hpp(TBD): some SkyboxNode's logic should be moved and implemented here
   - SkyboxNode.hpp(TBD): this will be replaced by Skybox.hpp later
   - Terrain.hpp(TBD): create and set up a terrain
   - Texture.hpp: set up, load, manage, and bind textures in OpenGL
   - Transform.hpp: responsible for holding matrix operations in model, view, and projection space
   - VertexBufferLayout.hpp: set up a variety of Vertex Buffer Object (VBO) layouts
3. ./shaders
   - skybox_vert.glsl
   - skybox_frag.glsl
   - ... other shaders for different objects in the scene(TBD)
4. ./src
   - Camera.cpp
   - Geometry.cpp
   - glad.cpp
   - globals.cpp
   - Image.cpp
   - main.cpp
   - Object.cpp
   - ObjectManager.cpp(TBD)
   - Renderer.cpp
   - SceneNode.cpp
   - SDLGraphicsProgram.cpp
   - Shader.cpp
   - Skybox.cpp(TBD)
   - SkyboxNode.cpp(TBD)
   - Terrain.cpp(TBD)
   - Texture.cpp
   - Transform.cpp
   - VertexBufferLayout.cpp
5. Build.py: build the executable
## UML Diagram

## How the Scene Works

## How Aurora is Simulated
  
## Acknowledgement
