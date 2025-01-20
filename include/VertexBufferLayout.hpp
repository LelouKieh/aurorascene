#ifndef VERTEX_BUFFER_LAYOUT_HPP
#define VERTEX_BUFFER_LAYOUT_HPP

#include <glad/glad.h>

// The VertexBufferLayout class is responsible for managing 
// Vertex Buffer Objects (VBO), Index Buffer Objects (IBO), and
// Vertex Array Objects (VAO) for rendering various data layouts.
class VertexBufferLayout{ 
public:
    // Generates a new buffer
    VertexBufferLayout();
    // Destroys all of our buffers.
    ~VertexBufferLayout();
    // Selects the buffer to bind
    void Bind();
    // Unbind our buffers
    void Unbind();

    // Creates a vertex and index buffer object
    // Format is: x,y,z
    // vcount: the number of vertices
    // icount: the number of indices
    // vdata: A pointer to an array of data for vertices
    // idata: A pointer to an array of data for indices
    void CreatePositionBufferLayout(unsigned int vcount,unsigned int icount, float* vdata, unsigned int* idata );

    // Creates a vertex and index buffer object
    // Format is: x,y,z, s,t
    void CreateTextureBufferLayout(unsigned int vcount,unsigned int icount, float* vdata, unsigned int* idata );

    // A normal map layout needs the following attributes
    // positions: x,y,z
    // normals:  x,y,z
    // texcoords: s,t
    // tangent: t_x,t_y,t_z
    // bitangent b_x,b_y,b_z
    void CreateNormalBufferLayout(unsigned int vcount,unsigned int icount, float* vdata, unsigned int* idata );

    // positions: x, y, z
    // colors: r, g, b
    // normals: x, y, z
    // texcoords: s, t
    void CreateSkyboxBufferLayout(unsigned int vcount,unsigned int icount, float* vdata, unsigned int* idata );

private:
    // Vertex Array Object
    GLuint m_VAOId;
    GLuint m_VBOId;
    GLuint m_EBOId;

    // Vertex Buffer
    GLuint m_vertexPositionBuffer;
    // Index Buffer Object
    GLuint m_indexBufferObject;
    // Stride of data (how do I get to the next vertex)
    unsigned int m_stride{0};
};


#endif
