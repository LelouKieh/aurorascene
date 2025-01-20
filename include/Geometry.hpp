#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <vector>
#include <glm/glm.hpp>

// store vertice and triangle information
class Geometry{
public:
	// Constructor
	Geometry();
	// Destructor
	~Geometry();
	
	// Functions for working with individual vertices
	unsigned int GetBufferSizeInBytes();
    // Retrieve the 
	unsigned int GetBufferDataSize();
    // Retrieve the Buffer Data Size
	float* GetBufferDataPtr();
	// Add a new vertex
	void AddVertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec3& color, const glm::vec2& texcoord);
    void AddIndex(unsigned int i);
	// gen pushes all attributes into a single vector
	void Gen();
	// Creates a triangle from 3 indices
	void MakeTriangle(unsigned int vert0, unsigned int vert1, unsigned int vert2);  
    // Retrieve how many indices there are
	unsigned int GetIndicesSize();
    // Retrieve the pointer to the indices
	unsigned int* GetIndicesDataPtr();
	// Retrieve the number of vertices
	size_t GetVertexCount() const;

private:
	// m_bufferData stores all of the vertexPositons, coordinates, normals, etc.
	std::vector<float> m_bufferData;

	std::vector<glm::vec3> m_vertexPositions;
	std::vector<glm::vec3> m_colors;
	std::vector<glm::vec3> m_normals;
	std::vector<glm::vec2> m_textureCoords;
	std::vector<float> m_tangents;
	std::vector<float> m_biTangents;

	// The indices for a indexed-triangle mesh
	std::vector<unsigned int> m_indices;
};



#endif
