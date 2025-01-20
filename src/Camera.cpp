#define GLM_ENABLE_EXPERIMENTAL // Enable experimental features in GLM

#include "Camera.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include <iostream>

// Handles mouse movement to update the camera's view direction
void Camera::MouseLook(int mouseX, int mouseY){
    // Record our new position as a vector
    glm::vec2 newMousePosition(mouseX, mouseY);

    // Static flag to handle the first mouse input correctly
    static bool firstLook=true;
    if(true == firstLook){
        firstLook=false;
        m_oldMousePosition = newMousePosition; // Initialize old mouse position
    }

    // Calculate the change (delta) in mouse position
    glm::vec2 delta = m_oldMousePosition - newMousePosition;

    // Rotate the view direction based on the horizontal mouse movement (delta.x)
    m_viewDirection = glm::rotate(m_viewDirection,glm::radians(delta.x),m_upVector);
 
    // Update the old mouse position for the next frame
    m_oldMousePosition = newMousePosition;
}

// Moves the camera forward in the direction of the view
void Camera::MoveForward(float speed){
        m_eyePosition.x += m_viewDirection.x *speed;
        m_eyePosition.y += m_viewDirection.y *speed;
        m_eyePosition.z += m_viewDirection.z *speed;
}

// Moves the camera backward (opposite to the view direction)
void Camera::MoveBackward(float speed){
        m_eyePosition.x -= m_viewDirection.x *speed;
        m_eyePosition.y -= m_viewDirection.y *speed;
        m_eyePosition.z -= m_viewDirection.z *speed;
}

void Camera::MoveLeft(float speed){
    glm::vec3 rightVector = glm::cross(m_viewDirection, m_upVector);
    m_eyePosition -= rightVector * speed; // Move left by negating the right vector
}

void Camera::MoveRight(float speed){
    glm::vec3 rightVector = glm::cross(m_viewDirection, m_upVector);
    m_eyePosition += rightVector * speed; // Move right by adding the right vector
}

// Moves the camera upward along the Y-axis
void Camera::MoveUp(float speed){
    m_eyePosition.y += speed;
}

void Camera::MoveDown(float speed){
    m_eyePosition.y -= speed;
}

// Set the position for the camera
void Camera::SetCameraEyePosition(float x, float y, float z){
    m_eyePosition.x = x;
    m_eyePosition.y = y;
    m_eyePosition.z = z;
}

// Retrieves the X position of the camera
float Camera::GetEyeXPosition(){
    return m_eyePosition.x;
}

float Camera::GetEyeYPosition(){
    return m_eyePosition.y;
}

float Camera::GetEyeZPosition(){
    return m_eyePosition.z;
}

// Retrieves the X component of the camera's view direction
float Camera::GetViewXDirection(){
    return m_viewDirection.x;
}

float Camera::GetViewYDirection(){
    return m_viewDirection.y;
}

float Camera::GetViewZDirection(){
    return m_viewDirection.z;
}


// Constructor: Initializes the camera's position, direction, and up vector
Camera::Camera(){
    std::cout << "Camera.cpp: (Constructor) Created a Camera!\n";
	// // Set initial camera position
    m_eyePosition = glm::vec3(0.0f,0.0f, 3.0f);
	// Looking down along the z-axis initially
    m_viewDirection = glm::vec3(0.0f,0.0f, -1.0f);
	// our upVector always points up along the y-axis for now
    m_upVector = glm::vec3(0.0f, 1.0f, 0.0f);
}


// Generates a view matrix for rendering based on camera position and direction
glm::mat4 Camera::GetWorldToViewmatrix() const{
    return glm::lookAt( m_eyePosition, // Camera position
                        m_eyePosition + m_viewDirection, // Target position (view direction)
                        m_upVector // Up vector
                        );
}

