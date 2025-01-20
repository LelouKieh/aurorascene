#ifndef SDLGRAPHICSPROGRAM_HPP
#define SDLGRAPHICSPROGRAM_HPP

#if defined(LINUX) || defined(MINGW)
    #include <SDL2/SDL.h>
#else
    #include <SDL.h>
#endif

#include <glad/glad.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "SceneNode.hpp"
#include "Renderer.hpp"
#include "Object.hpp"
#include "SkyboxNode.hpp"
#include "Camera.hpp"

// This class sets up a full graphics program using SDL
class SDLGraphicsProgram{
public:

    // Constructor
    SDLGraphicsProgram(int w, int h);
    // Destructor
    ~SDLGraphicsProgram();
    // Setup OpenGL
    bool InitGL();
    // Loop that runs forever
    void Loop();
    // Get Pointer to Window
    SDL_Window* GetSDLWindow();
    // Helper Function to Query OpenGL information.
    void GetOpenGLVersionInfo();
    // Helper Function to control user input
    void Input(bool& quit, float cameraSpeed);
    // Initialize Scene Graph and objects in the Scene Graph
    void InitSceneGraph();
    // Update Objects in Scene Graph
    void UpdateObjectsInScene();

private:
	// The Renderer responsible for drawing objects in OpenGL
	Renderer* m_renderer;
    // The window we'll be rendering to
    SDL_Window* m_window ;
    // OpenGL context
    SDL_GLContext m_openGLContext;
};

#endif
