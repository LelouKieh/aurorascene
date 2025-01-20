#include "Object.hpp"
#include "Error.hpp"

// Constructor: Initializes the Object instance
Object::Object() {}

// Destructor: Cleans up resources used by the Object instance
Object::~Object() {}

// Loads a texture file into the object
// @param fileName: Path to the texture file
void Object::LoadTexture(std::string fileName) {
    m_textureDiffuse.LoadTexture(fileName); // Load the texture into the diffuse map
}

// Loads an OBJ file and sets up its geometry and texture
// @param filepath: Path to the OBJ file
void Object::LoadOBJ(std::string filepath) {
    std::cout << "Loading OBJ file: " << filepath << std::endl;
    m_filePath = filepath;

    // Extract the directory from the file path
    size_t lastSlash = filepath.find_last_of("/\\");
    m_directory = (lastSlash != std::string::npos) ? filepath.substr(0, lastSlash + 1) : "";

    parseOBJ(filepath); // Parse the OBJ file for geometry and material data

    // Generate geometry data and set up the buffer layout
    m_geometry.Gen();
    m_vertexBufferLayout.CreateSkyboxBufferLayout(
        m_geometry.GetBufferDataSize(),
        m_geometry.GetIndicesSize(),
        m_geometry.GetBufferDataPtr(),
        m_geometry.GetIndicesDataPtr()
    );
}

// Parses an OBJ file for geometry and material references
// @param filepath: Path to the OBJ file
void Object::parseOBJ(const std::string& filepath) {
    std::ifstream objFile(filepath);
    if (!objFile.is_open()) {
        std::cerr << "Failed to open OBJ file: " << filepath << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string line;
    std::string mtlFilename; // For material file reference

    // Temporary storage for vertex data
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_texcoords;
    std::vector<glm::vec3> temp_normals;

    // Map to avoid duplicating vertices
    std::map<VertexKey, unsigned int> vertexMap;

    // Process the OBJ file line by line
    while (std::getline(objFile, line)) {
        std::stringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "mtllib") {
            // Material library reference
            ss >> mtlFilename;
            parseMTL(m_directory + mtlFilename);
            std::cout << "MTL file found: " << m_directory + mtlFilename << std::endl;
        } else if (prefix == "v") {
            // Vertex position
            glm::vec3 vertex;
            ss >> vertex.x >> vertex.y >> vertex.z;
            temp_vertices.push_back(vertex);
        } else if (prefix == "vt") {
            // Texture coordinates
            glm::vec2 texcoord;
            ss >> texcoord.x >> texcoord.y;
            temp_texcoords.push_back(texcoord);
        } else if (prefix == "vn") {
            // Vertex normals
            glm::vec3 normal;
            ss >> normal.x >> normal.y >> normal.z;
            temp_normals.push_back(normal);
        } else if (prefix == "f") {
            // Face data (triangles or polygons)
            std::vector<unsigned int> faceVertexIndices;
            std::string vertexData;

            while (ss >> vertexData) {
                std::stringstream vertexStream(vertexData);
                std::string value;
                unsigned int posIndex = 0, texIndex = 0, normIndex = 0;
                int index = 0;

                while (std::getline(vertexStream, value, '/')) {
                    if (!value.empty()) {
                        unsigned int idx = std::stoi(value);
                        if (index == 0) posIndex = idx;
                        else if (index == 1) texIndex = idx;
                        else if (index == 2) normIndex = idx;
                    }
                    index++;
                }

                // Adjust indices (OBJ format starts from 1)
                posIndex--;
                texIndex = (texIndex > 0) ? texIndex - 1 : texIndex;
                normIndex = (normIndex > 0) ? normIndex - 1 : normIndex;

                VertexKey key = { posIndex, texIndex, normIndex };

                if (vertexMap.find(key) == vertexMap.end()) {
                    glm::vec3 vertex = temp_vertices[posIndex];
                    glm::vec2 texcoord = (texIndex < temp_texcoords.size()) ? temp_texcoords[texIndex] : glm::vec2(0.0f, 0.0f);
                    glm::vec3 normal = (normIndex < temp_normals.size()) ? temp_normals[normIndex] : glm::vec3(0.0f, 0.0f, 0.0f);
                    glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);

                    m_geometry.AddVertex(vertex, normal, color, texcoord);
                    unsigned int newIndex = static_cast<unsigned int>(m_geometry.GetVertexCount() - 1);
                    vertexMap[key] = newIndex;
                    faceVertexIndices.push_back(newIndex);
                } else {
                    faceVertexIndices.push_back(vertexMap[key]);
                }
            }

            if (faceVertexIndices.size() == 3) {
                m_geometry.MakeTriangle(faceVertexIndices[0], faceVertexIndices[1], faceVertexIndices[2]);
            } else if (faceVertexIndices.size() > 3) {
                for (size_t i = 1; i + 1 < faceVertexIndices.size(); ++i) {
                    m_geometry.MakeTriangle(faceVertexIndices[0], faceVertexIndices[i], faceVertexIndices[i + 1]);
                }
            } else {
                std::cerr << "Face with less than 3 vertices encountered.\n";
            }
        }
    }

    objFile.close();
}

// Parses an MTL file for texture and material information
// @param filepath: Path to the MTL file
void Object::parseMTL(const std::string& filepath) {
    std::ifstream mtlFile(filepath);
    if (!mtlFile.is_open()) {
        std::cerr << "Failed to open MTL file: " << filepath << std::endl;
        return;
    }

    std::string line, prefix;
    while (std::getline(mtlFile, line)) {
        std::stringstream ss(line);
        ss >> prefix;

        if (prefix == "map_Kd") {
            std::string textureFilename;
            ss >> textureFilename;
            m_textureDiffuse.LoadTexture(m_directory + textureFilename);
        } else if (prefix == "map_Bump" || prefix == "bump") {
            std::string normalMapFilename;
            ss >> normalMapFilename;
            m_normalMap.LoadTexture(m_directory + normalMapFilename);
        }
    }

    mtlFile.close();
}

// Binds the object (geometry and textures) for rendering
void Object::Bind() {
    m_vertexBufferLayout.Bind();     // Bind vertex buffer layout
    m_textureDiffuse.Bind(0);       // Bind the diffuse texture to slot 0
}

// Renders the object using OpenGL
void Object::Render() {
    Bind(); // Bind the necessary resources
    glDrawElements(GL_TRIANGLES,   // Draw mode
                   m_geometry.GetIndicesSize(), // Number of indices
                   GL_UNSIGNED_INT,             // Data type of indices
                   nullptr);                    // No offset, use bound buffer
}
