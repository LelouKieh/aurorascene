#include "SkyboxNode.hpp"


// Constructor: Initializes the SkyboxNode with a skybox object and shaders
// @param skyboxObject: Pointer to the object representing the skybox
SkyboxNode::SkyboxNode(Object* skyboxObject)
    : SceneNode(skyboxObject, "shaders/skybox_vert.glsl", "shaders/skybox_frag.glsl") {}

// Initializes the SkyboxNode by loading the skybox model
// @param skyboxObject: Pointer to the object representing the skybox
void SkyboxNode::Init(Object* skyboxObject) {
    skyboxObject->LoadOBJ("common/objects/skybox_1.obj"); // Load the skybox object
    std::cout << "SkyboxNode Initialized" << std::endl;
}

// Updates the SkyboxNode, applying transformations and updating shader uniforms
// @param projectionMatrix: The projection matrix for rendering
// @param camera: Pointer to the camera used for view calculations
// @param renderer: Pointer to the renderer managing the scene
void SkyboxNode::Update(glm::mat4 projectionMatrix, Camera* camera, Renderer* renderer) {
    if (m_parent != nullptr) {
        // Combine parent's world transform with local transform
        m_worldTransform = m_parent->GetWorldTransform() * GetLocalTransform();
    } else {
        // No parent, so world transform is the local transform
        m_worldTransform = m_localTransform;
    }

    if (m_object != nullptr) {
        m_shader.Bind(); // Bind the shader for the skybox

        // Model matrix (identity matrix as skybox does not scale/rotate)
        glm::mat4 modelMatrix = glm::mat4(1.0f);

        // View matrix: Remove translation to ensure the skybox stays in view
        glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -100.0f, -10.0f));

        // Pass transformation matrices to the shader
        m_shader.SetUniformMatrix4fv("u_ModelMatrix", &modelMatrix[0][0]);
        m_shader.SetUniformMatrix4fv("u_ViewMatrix", &viewMatrix[0][0]);
        m_shader.SetUniformMatrix4fv("u_Projection", &projectionMatrix[0][0]);

        // Pass additional uniforms to the shader
        Uint32 currentTime = SDL_GetTicks();
        float iTime = (currentTime - renderer->GetStartTime()) / 1000.0f; // Elapsed time in seconds
        m_shader.SetUniform1f("iTime", iTime);

        unsigned int screenWidth = renderer->GetScreenWidth();
        unsigned int screenHeight = renderer->GetScreenHeight();
        m_shader.SetUniform3f("iResolution", (float)screenWidth, (float)screenHeight, 1.0f);

        int mouseX = renderer->GetMouseX();
        int mouseY = renderer->GetMouseY();
        m_shader.SetUniform4f("iMouse", (float)mouseX, (float)(screenHeight - mouseY), 0.0f, 0.0f);

        // Recursively update all child nodes
        for (int i = 0; i < m_children.size(); ++i) {
            m_children[i]->Update(projectionMatrix, camera, renderer);
        }
    }
}

// Draws the SkyboxNode and its children
void SkyboxNode::Draw() {
    if (m_object != nullptr) {
        m_shader.Bind();      // Bind the shader for rendering
        m_object->Render();   // Render the skybox object

        // Recursively draw all child nodes
        for (int i = 0; i < m_children.size(); ++i) {
            m_children[i]->Draw();
        }
    }
}
