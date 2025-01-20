#include "Renderer.hpp"
#include "SceneNode.hpp"

// Constructor: Initializes the Renderer with the specified width and height
Renderer::Renderer(unsigned int w, unsigned int h) 
    : m_screenWidth(w), m_screenHeight(h), startTime(0), mouseX(0), mouseY(0) {
    
    // By default, create one camera for the renderer
    Camera* defaultCamera = new Camera();
    m_cameras.push_back(defaultCamera); // Add the default camera to the list

    m_root = nullptr; // Initialize the root node to null
    std::cout << "Renderer created with width: " << m_screenWidth 
              << " and height: " << m_screenHeight << std::endl;
}

// Destructor: Cleans up resources used by the Renderer
Renderer::~Renderer() {
    // Delete all camera pointers to avoid memory leaks
    for (int i = 0; i < m_cameras.size(); i++) {
        delete m_cameras[i];
    }
}

// Updates the scene graph and camera view
void Renderer::Update() {
    // Set up the projection matrix for perspective rendering
    // Field of view: 45 degrees
    // Aspect ratio: screen width / screen height
    // Near clipping plane: 0.1f
    // Far clipping plane: 512.0f
    m_projectionMatrix = glm::perspective(
        glm::radians(45.0f),
        ((float)m_screenWidth) / ((float)m_screenHeight),
        0.1f,
        512.0f
    );

    // Update the scene graph starting from the root node
    if (m_root != nullptr) {
        // Currently uses the first camera (index 0) for updates.
        m_root->Update(m_projectionMatrix, m_cameras[0], this);
    }
}

// Renders the scene, setting up the OpenGL state and drawing the scene graph
void Renderer::Render() {
    // Enable depth testing (Z-buffer) to handle 3D object depth
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D); // Enable 2D textures

    // Set the viewport to the full screen dimensions
    glViewport(0, 0, m_screenWidth, m_screenHeight);

    // Set the background color (dark gray)
    glClearColor(0.01f, 0.01f, 0.01f, 1.f);

    // Clear the depth and color buffers for the current frame
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    // Debug: Render in wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Draw the scene graph starting from the root node
    if (m_root != nullptr) {
        m_root->Draw();
    }
}

// Sets the root node of the scene graph
// @param startingNode: The root node of the scene graph
void Renderer::setRoot(SceneNode* startingNode) {
    m_root = startingNode;
}
