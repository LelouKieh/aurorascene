#ifndef SKYBOXNODE_HPP
#define SKYBOXNODE_HPP

#include "SceneNode.hpp"
#include "Object.hpp"

// SkyboxNode class inherits from SceneNode to represent a skybox in the scene graph.
class SkyboxNode : public SceneNode {
public:
    // Constructor: Initializes the SkyboxNode with the provided skybox object.
    // @param skyboxObject: A pointer to the Object representing the skybox.
    SkyboxNode(Object* skyboxObject);

    // Initializes the SkyboxNode with the provided skybox object.
    // This method can be used for delayed initialization.
    // @param skyboxObject: A pointer to the Object representing the skybox.
    void Init(Object* skyboxObject);

    // Updates the SkyboxNode.
    // This method is called every frame to update the skybox's state.
    // @param projectionMatrix: The projection matrix for rendering.
    // @param camera: A pointer to the Camera used for view calculations.
    // @param renderer: A pointer to the Renderer for rendering operations.
    void Update(glm::mat4 projectionMatrix, Camera* camera, Renderer* renderer) override;

    // Draws the SkyboxNode.
    // This method is called every frame to render the skybox.
    void Draw() override;
};

#endif