#include "Geometry.hpp"
#include <assert.h>
#include <iostream>
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/glm.hpp"

// Constructor
Geometry::Geometry(){

}

// Destructor
Geometry::~Geometry(){

}

// Adds a vertex to the geometry with position, normal, color, and texture coordinates
void Geometry::AddVertex(
    const glm::vec3& position,
    const glm::vec3& normal,
    const glm::vec3& color,
    const glm::vec2& texcoord)
{
    m_vertexPositions.push_back(position);
    m_normals.push_back(normal);
    m_colors.push_back(color);
    m_textureCoords.push_back(texcoord);
}

// Adds a single index to the geometry, with bounds checking
void Geometry::AddIndex(unsigned int i){
    // Ensure the index is valid based on the vertex count
    if(i >= 0 && i <= m_vertexPositions.size()/3){
        m_indices.push_back(i);
    }else{
        std::cout << "(Geometry.cpp) ERROR, invalid index\n";
    }
}

// Retrieves a pointer to the vertex buffer data
float* Geometry::GetBufferDataPtr(){
	return m_bufferData.data();
}

// Retrieves the size of the vertex buffer data (in number of floats)
unsigned int Geometry::GetBufferDataSize(){
	return m_bufferData.size();
}

// Retrieves the size of the vertex buffer data (in bytes)
unsigned int Geometry::GetBufferSizeInBytes(){
	return m_bufferData.size()*sizeof(float);
}

// Generates the vertex buffer by consolidating all vertex attributes into a single buffer
void Geometry::Gen(){
    // Ensure all attributes have the same size
    assert((m_vertexPositions.size() == m_colors.size()) &&
           (m_vertexPositions.size() == m_normals.size()) &&
           (m_vertexPositions.size() == m_textureCoords.size()));

    // Pack all vertex attributes into a single vector
    for (size_t i = 0; i < m_vertexPositions.size(); ++i) {
        // Positions
        m_bufferData.push_back(m_vertexPositions[i].x);
        m_bufferData.push_back(m_vertexPositions[i].y);
        m_bufferData.push_back(m_vertexPositions[i].z);

        // Colors
        m_bufferData.push_back(m_colors[i].r);
        m_bufferData.push_back(m_colors[i].g);
        m_bufferData.push_back(m_colors[i].b);

        // Normals
        m_bufferData.push_back(m_normals[i].x);
        m_bufferData.push_back(m_normals[i].y);
        m_bufferData.push_back(m_normals[i].z);

        // Texture Coordinates
        m_bufferData.push_back(m_textureCoords[i].x);
        m_bufferData.push_back(m_textureCoords[i].y);
    }
}

// Creates a triangle using three vertex indices
void Geometry::MakeTriangle(unsigned int vert0, unsigned int vert1, unsigned int vert2){
	std::cout<< vert0 << " " << vert1 << " " << vert2 << std::endl;
    // Validate indices to avoid out-of-bounds errors
    size_t vertexCount = GetVertexCount();
    if (vert0 >= vertexCount || vert1 >= vertexCount || vert2 >= vertexCount) {
        std::cerr << "MakeTriangle: Invalid indices: " << vert0 << ", " << vert1 << ", " << vert2 << std::endl;
        return; // Skip this triangle
    }

	// Add triangle indices
    m_indices.push_back(vert0);	
	m_indices.push_back(vert1);	
	m_indices.push_back(vert2);	
}

// Retrieves the number of indices in the geometry
unsigned int Geometry::GetIndicesSize(){
	return m_indices.size();
}

// Retrieves a pointer to the indices data
unsigned int* Geometry::GetIndicesDataPtr(){
	return m_indices.data();
}

// Retrieves the total number of vertices in the geometry
size_t Geometry::GetVertexCount() const {
    return m_vertexPositions.size();
}
