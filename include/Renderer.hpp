#ifndef RENDERER_HPP
#define RENDERER_HPP

// Renderer is responsible for drawing everything.
// It contains a scenegraph node and a camera.

#if defined(LINUX) || defined(MINGW)
    #include <SDL2/SDL.h>
#else
    #include <SDL.h>
#endif

#include <glad/glad.h>

#include <vector>
#include <iostream>

#include "Camera.hpp"

class SceneNode;

class Renderer{
public:
    // The constructor	
    // Sets the width and height of the rendererer draws to
    Renderer(unsigned int w, unsigned int h);
    // Destructor
    ~Renderer();
    // Update the scene
    void Update();
    // Render the scene
    void Render();
    // Sets the root of our renderer to some node to draw an entire scene graph
    void setRoot(SceneNode* startingNode);
    // Setters for time and mouse position
    void SetStartTime(Uint32 time) { startTime = time; }
    void SetMousePosition(int x, int y) { mouseX = x; mouseY = y; }
    // Getters for time and mouse position
    Uint32 GetStartTime() const { return startTime; }
    int GetMouseX() const { return mouseX; }
    int GetMouseY() const { return mouseY; }
    // Getters for screen dimensions
    unsigned int GetScreenWidth() const { return m_screenWidth; }
    unsigned int GetScreenHeight() const { return m_screenHeight; }
    // Returns the camera at an index
    Camera*& GetCamera(unsigned int index){
        if(index > m_cameras.size()-1){
            std::cerr << "Error: Camera index out of bounds." << std::endl;
        }
        return m_cameras[index];
    }

protected:
    // One or more cameras camera per Renderer
    std::vector<Camera*> m_cameras;
    // Root scene node
    SceneNode* m_root;
    // Store the projection matrix for our camera.
    glm::mat4 m_projectionMatrix;

private:
    // Screen dimension constants
    int m_screenHeight;
    int m_screenWidth;
    // Time and mouse variables
    Uint32 startTime;
    int mouseX;
    int mouseY;
};

#endif
