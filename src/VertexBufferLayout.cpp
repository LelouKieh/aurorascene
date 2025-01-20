#include "VertexBufferLayout.hpp"
#include <iostream>


// Default Constructor: Initializes a new VertexBufferLayout object
VertexBufferLayout::VertexBufferLayout() {}

// Destructor: Cleans up GPU buffers associated with the layout
VertexBufferLayout::~VertexBufferLayout() {
    // Delete the vertex and index buffers from the GPU
    glDeleteBuffers(1, &m_vertexPositionBuffer);
    glDeleteBuffers(1, &m_indexBufferObject);
}


// Binds the vertex array and its associated buffers
void VertexBufferLayout::Bind() {
    glBindVertexArray(m_VAOId);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexPositionBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObject);
}

// Unbinds the vertex array and its associated buffers
// Rarely used since binding another buffer automatically unbinds the current one
void VertexBufferLayout::Unbind() {
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Creates a position buffer layout for vertex data (x, y, z)
// @param vcount: Number of vertices
// @param icount: Number of indices
// @param vdata: Pointer to vertex data
// @param idata: Pointer to index data
void VertexBufferLayout::CreatePositionBufferLayout(unsigned int vcount, unsigned int icount, float* vdata, unsigned int* idata) {
    m_stride = 3; // Each vertex has 3 components (x, y, z)

    // Ensure float sizes match OpenGL expectations
    static_assert(sizeof(GLfloat) == sizeof(float), "GLfloat and float sizes do not match");

    // Generate and bind the vertex array
    glGenVertexArrays(1, &m_VAOId);
    glBindVertexArray(m_VAOId);

    // Generate and bind the vertex buffer
    glGenBuffers(1, &m_vertexPositionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexPositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, vcount * sizeof(float), vdata, GL_STATIC_DRAW);

    // Specify the layout for position data
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * m_stride, 0);

    // Generate and bind the index buffer
    glGenBuffers(1, &m_indexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, icount * sizeof(unsigned int), idata, GL_STATIC_DRAW);
}

// Creates a texture buffer layout for vertex data (x, y, z, s, t)
// @param vcount: Number of vertices
// @param icount: Number of indices
// @param vdata: Pointer to vertex data
// @param idata: Pointer to index data
void VertexBufferLayout::CreateTextureBufferLayout(unsigned int vcount, unsigned int icount, float* vdata, unsigned int* idata) {
    m_stride = 5; // Each vertex has 5 components (x, y, z, s, t)

    static_assert(sizeof(GLfloat) == sizeof(float), "GLfloat and float sizes do not match");

    glGenVertexArrays(1, &m_VAOId);
    glBindVertexArray(m_VAOId);

    glGenBuffers(1, &m_vertexPositionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexPositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, vcount * sizeof(float), vdata, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * m_stride, 0);

    glEnableVertexAttribArray(1); // Texture coordinates attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, sizeof(float) * m_stride, (char*)(sizeof(float) * 3));

    glGenBuffers(1, &m_indexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, icount * sizeof(unsigned int), idata, GL_STATIC_DRAW);
}

// Creates a normal buffer layout for vertex data (x, y, z, nx, ny, nz, s, t, tx, ty, tz, bx, by, bz)
// @param vcount: Number of vertices
// @param icount: Number of indices
// @param vdata: Pointer to vertex data
// @param idata: Pointer to index data
void VertexBufferLayout::CreateNormalBufferLayout(unsigned int vcount, unsigned int icount, float* vdata, unsigned int* idata) {
    m_stride = 14; // Each vertex has 14 components (position, normal, texcoord, tangent, bitangent)

    static_assert(sizeof(GLfloat) == sizeof(float), "GLfloat and float sizes do not match");

    glGenVertexArrays(1, &m_VAOId);
    glBindVertexArray(m_VAOId);

    glGenBuffers(1, &m_vertexPositionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexPositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, vcount * sizeof(float), vdata, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * m_stride, 0);

    glEnableVertexAttribArray(1); // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * m_stride, (char*)(sizeof(float) * 3));

    glEnableVertexAttribArray(2); // Texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * m_stride, (char*)(sizeof(float) * 6));

    glEnableVertexAttribArray(3); // Tangent
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(float) * m_stride, (char*)(sizeof(float) * 8));

    glEnableVertexAttribArray(4); // Bitangent
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(float) * m_stride, (char*)(sizeof(float) * 11));

    glGenBuffers(1, &m_indexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, icount * sizeof(unsigned int), idata, GL_STATIC_DRAW);
}

// Creates a skybox buffer layout for vertex data (x, y, z, r, g, b, nx, ny, nz, s, t)
// @param vcount: Number of vertices
// @param icount: Number of indices
// @param vdata: Pointer to vertex data
// @param idata: Pointer to index data
void VertexBufferLayout::CreateSkyboxBufferLayout(unsigned int vcount, unsigned int icount, float* vdata, unsigned int* idata) {
    m_stride = 11; // Each vertex has 11 components (position, color, normal, texcoord)

    static_assert(sizeof(GLfloat) == sizeof(float), "GLfloat and float sizes do not match");

    glGenVertexArrays(1, &m_VAOId);
    glBindVertexArray(m_VAOId);

    glGenBuffers(1, &m_vertexPositionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexPositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, vcount * sizeof(float), vdata, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * m_stride, 0);

    glEnableVertexAttribArray(1); // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * m_stride, (char*)(sizeof(float) * 3));

    glEnableVertexAttribArray(2); // Normal
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * m_stride, (char*)(sizeof(float) * 6));

    glEnableVertexAttribArray(3); // Texture coordinates
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(float) * m_stride, (char*)(sizeof(float) * 9));

    glGenBuffers(1, &m_indexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, icount * sizeof(unsigned int), idata, GL_STATIC_DRAW);

    glBindVertexArray(0); // Unbind vertex array
}
