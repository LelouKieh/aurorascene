#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <glad/glad.h>

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <tuple>
#include <iostream>

#include "Shader.hpp"
#include "VertexBufferLayout.hpp"
#include "Texture.hpp"
#include "Transform.hpp"
#include "Geometry.hpp"

#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"

/**
 * @struct VertexKey
 * @brief A struct used to uniquely identify a vertex based on its position, texture, and normal indices.
 */
struct VertexKey {
    unsigned int posIndex;
    unsigned int texIndex;
    unsigned int normIndex;

    /**
     * @brief Overloads the less-than operator to enable VertexKey to be used as a key in std::map.
     * @param other The other VertexKey to compare with.
     * @return True if this VertexKey is less than the other.
     */
    bool operator<(const VertexKey& other) const {
        return std::tie(posIndex, texIndex, normIndex) < std::tie(other.posIndex, other.texIndex, other.normIndex);
    }
};

// An abstraction to create multiple objects
class Object{
public:
    // Object Constructor
    Object();
    // Object destructor
    ~Object();
    // Load a texture
    void LoadTexture(std::string fileName);
    // Load an OBJ model
    void LoadOBJ(std::string filepath);
    // How to draw the object
    virtual void Render();
    VertexBufferLayout getVertexBufferLayout() {return m_vertexBufferLayout;}
    Geometry getGeometry() {return m_geometry;}

protected:
    // one buffer per object.
    VertexBufferLayout m_vertexBufferLayout;
    // one texture per object
    Texture m_textureDiffuse;
    Texture m_normalMap;
    // Store the objects Geometry
	Geometry m_geometry;

    // OBJ loading
    std::string m_filePath;
    std::string m_directory;

    // Parse functions
    void parseOBJ(const std::string& filepath);
    void parseMTL(const std::string& filepath);
    void Bind();
};


#endif
