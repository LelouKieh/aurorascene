#include "SceneNode.hpp"
#include "Renderer.hpp"

#include <string>
#include <iostream>

// Constructor: Initializes a SceneNode with an object and shader programs
// @param ob: Pointer to the object this node manages
// @param vertShader: Path to the vertex shader file
// @param fragShader: Path to the fragment shader file
SceneNode::SceneNode(Object* ob, std::string vertShader, std::string fragShader) {
    std::cout << "(SceneNode.cpp) Constructor called\n";
    m_object = ob;           // Assign the object to this node
    m_parent = nullptr;      // By default, the node has no parent

    // Load and compile shaders for this node
    std::string vertexShader = m_shader.LoadShader(vertShader);
    std::string fragmentShader = m_shader.LoadShader(fragShader);

    // Create the shader program
    m_shader.CreateShader(vertexShader, fragmentShader);
}

// Destructor: Cleans up the SceneNode and its children
SceneNode::~SceneNode() {
    // Recursively delete all child nodes
    for (int i = 0; i < m_children.size(); i++) {
        delete m_children[i];
    }
}

// Adds a child node to the current SceneNode
// @param n: Pointer to the child node to add
void SceneNode::AddChild(SceneNode* n) {
    // Set the parent of the child node to the current node
    n->m_parent = this;

    // Add the child node to the list of children
    m_children.push_back(n);
}

// Draws the current node's object and recursively draws all child nodes
void SceneNode::Draw() {
    // Bind the shader for this node or series of nodes
    m_shader.Bind();

    // Render the object associated with this node
    if (m_object != nullptr) {
        m_object->Render();

        // Recursively draw all child nodes
        for (int i = 0; i < m_children.size(); ++i) {
            m_children[i]->Draw();
        }
    }
}

// Updates the current node's transform and recursively updates all child nodes
// @param projectionMatrix: The projection matrix for rendering
// @param camera: Pointer to the camera used for rendering
// @param renderer: Pointer to the renderer managing the scene
void SceneNode::Update(glm::mat4 projectionMatrix, Camera* camera, Renderer* renderer) {
    // Compute the world transform based on the parent's world transform
    if (m_parent != nullptr) {
        m_worldTransform = m_parent->GetWorldTransform() * m_localTransform;
    } else {
        m_worldTransform = m_localTransform;
    }

    // Recursively update all child nodes
    if (m_object != nullptr) {
        for (int i = 0; i < m_children.size(); ++i) {
            m_children[i]->Update(projectionMatrix, camera, renderer);
        }
    }
}

// Returns a reference to the local transform of this node
// Allows for modification of the node's local transform
Transform& SceneNode::GetLocalTransform() {
    return m_localTransform;
}

// Returns a reference to the world transform of this node
// Allows for modification of the node's world transform
Transform& SceneNode::GetWorldTransform() {
    return m_worldTransform;
}
