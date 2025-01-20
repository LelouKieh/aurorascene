#include "Transform.hpp"


// Constructor: Initializes the Transform object with an identity matrix
Transform::Transform() {
    LoadIdentity();
}

// Destructor
Transform::~Transform() {}


// Resets the transformation matrix to the identity matrix
void Transform::LoadIdentity() {
    m_modelTransformMatrix = glm::mat4(1.0f); // Identity matrix
}

// Applies a translation transformation to the current matrix
// @param x, y, z: Translation values along the X, Y, and Z axes
void Transform::Translate(float x, float y, float z) {
    m_modelTransformMatrix = glm::translate(m_modelTransformMatrix, glm::vec3(x, y, z));
}

// Applies a rotation transformation to the current matrix
// @param radians: Angle of rotation in radians
// @param x, y, z: Axis of rotation
void Transform::Rotate(float radians, float x, float y, float z) {
    m_modelTransformMatrix = glm::rotate(m_modelTransformMatrix, radians, glm::vec3(x, y, z));
}

// Applies a scaling transformation to the current matrix
// @param x, y, z: Scaling factors along the X, Y, and Z axes
void Transform::Scale(float x, float y, float z) {
    m_modelTransformMatrix = glm::scale(m_modelTransformMatrix, glm::vec3(x, y, z));
}


// Returns a pointer to the transformation matrix data
// @return Pointer to the first element of the matrix
GLfloat* Transform::GetTransformMatrix() {
    return &m_modelTransformMatrix[0][0];
}

// Returns the internal transformation matrix
// @return A copy of the internal glm::mat4 matrix
glm::mat4 Transform::GetInternalMatrix() const {
    return m_modelTransformMatrix;
}

// Applies another Transform object's matrix to the current Transform
// @param t: The Transform to apply
void Transform::ApplyTransform(Transform t) {
    m_modelTransformMatrix = t.GetInternalMatrix();
}


// Multiplies the current Transform by another Transform
// @param t: The Transform to multiply with
// @return A reference to the updated Transform
Transform& Transform::operator*=(const Transform& t) {
    m_modelTransformMatrix = m_modelTransformMatrix * t.GetInternalMatrix();
    return *this;
}

// Adds the current Transform matrix with another Transform's matrix
// @param t: The Transform to add
// @return A reference to the updated Transform
Transform& Transform::operator+=(const Transform& t) {
    m_modelTransformMatrix = m_modelTransformMatrix + t.GetInternalMatrix();
    return *this;
}

// Assigns the matrix of another Transform to the current Transform
// @param t: The Transform to assign
// @return A reference to the updated Transform
Transform& Transform::operator=(const Transform& t) {
    m_modelTransformMatrix = t.GetInternalMatrix();
    return *this;
}

// Multiplies two Transforms and returns the result
// @param lhs, rhs: The left-hand and right-hand Transforms
// @return A new Transform that is the result of the multiplication
Transform operator*(const Transform& lhs, const Transform& rhs) {
    Transform result;
    result.m_modelTransformMatrix = lhs.GetInternalMatrix() * rhs.GetInternalMatrix();
    return result;
}

// Adds two Transforms and returns the result
// @param lhs, rhs: The left-hand and right-hand Transforms
// @return A new Transform that is the result of the addition
Transform operator+(const Transform& lhs, const Transform& rhs) {
    Transform result;
    result.m_modelTransformMatrix = lhs.GetInternalMatrix() + rhs.GetInternalMatrix();
    return result;
}
