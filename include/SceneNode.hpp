#ifndef SCENENODE_HPP
#define SCENENODE_HPP

// SceneNode helps organize a large 3D graphics scene.
// The traversal of the tree takes place starting from root.

#include <vector>
#include <memory>
#include <string>
#include <iostream>

#include "Object.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"

#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Renderer;

class SceneNode{
public:
    // A SceneNode is created by taking a pointer to an object
    SceneNode(Object* ob, std::string vertShader, std::string fragShader);
    // Destructor destroys all of the children within the node
    virtual ~SceneNode();
    // Adds a child node to our current node
    void AddChild(SceneNode* n);
    // Draws the current SceneNode
    virtual void Draw();
    // Updates the current SceneNode
    virtual void Update(glm::mat4 projectionMatrix, Camera* camera, Renderer* renderer);
    // Returns the local transformation transform
    // Local is local to an object, where it's center is the origin
    Transform& GetLocalTransform();
    // Returns a SceneNode's world transform
    Transform& GetWorldTransform();
    // one shader per Node
    Shader m_shader;
    
protected:
    // Parent
    SceneNode* m_parent;
    // Children holds all a pointer to all of the descendents of a particular SceneNode
    std::vector<SceneNode*> m_children;
    // The object stored in the scene graph
    Object* m_object;
    // Each SceneNode nodes locals transform
    Transform m_localTransform;
    // store the world transform
    Transform m_worldTransform;
};

#endif
