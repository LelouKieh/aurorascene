#if defined(LINUX) || defined(MINGW)
    #include <SDL2/SDL.h>
#else
    #include <SDL.h>
#endif

#include "Texture.hpp"
#include <fstream>
#include <iostream>
#include <glad/glad.h>
#include <sstream>
#include <vector>


// Default Constructor: Initializes the Texture object
Texture::Texture() {}

// Destructor: Cleans up texture data from the GPU and memory
Texture::~Texture() {
    glDeleteTextures(1, &m_textureID); // Delete texture from GPU
    if (m_image != nullptr) {
        delete m_image; // Free image memory
    }
}


// Loads a texture from a file and sends it to the GPU
// @param filepath: Path to the texture file
void Texture::LoadTexture(const std::string filepath) {
    m_filepath = filepath; // Store the file path
    m_image = new Image(filepath); // Load image data
    m_image->LoadPPM(true); // Load the PPM file and optionally flip vertically
    std::cout << "Loading texture: " << filepath << std::endl;

    glEnable(GL_TEXTURE_2D);

    // Generate a texture ID and bind it
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    // Set texture parameters for filtering and wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Upload pixel data to the GPU
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 m_image->GetWidth(), m_image->GetHeight(),
                 0, GL_RGB, GL_UNSIGNED_BYTE, m_image->GetPixelDataPtr());

    // Generate mipmaps for the texture
    glGenerateMipmap(GL_TEXTURE_2D);

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);
}


// Binds the texture to a specified slot (default slot is 0)
// @param slot: Texture slot to bind to
void Texture::Bind(unsigned int slot) const {
    glEnable(GL_TEXTURE_2D);       // Enable 2D textures
    glActiveTexture(GL_TEXTURE0 + slot); // Set the active texture slot
    glBindTexture(GL_TEXTURE_2D, m_textureID); // Bind the texture
}

// Unbinds the texture from the current slot
void Texture::Unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}


// Loads a PPM image file and uploads it as a texture
// @param filepath: Path to the PPM file
// @return true if the PPM file was successfully loaded
bool Texture::LoadPPM(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to load PPM file: " << filepath << std::endl;
        return false;
    }

    std::string line;

    // Read the PPM header
    std::getline(file, line);
    if (line != "P3") { // Ensure the format is P3
        std::cerr << "Invalid PPM format (must be P3): " << filepath << std::endl;
        return false;
    }

    // Skip comments
    while (std::getline(file, line)) {
        if (line[0] != '#') break;
    }

    // Parse width, height, and max color value
    std::stringstream ss(line);
    int width, height, maxColor;
    ss >> width >> height;
    file >> maxColor;

    // Read pixel data
    std::vector<unsigned char> data;
    int r, g, b;
    while (file >> r >> g >> b) {
        data.push_back(static_cast<unsigned char>(r));
        data.push_back(static_cast<unsigned char>(g));
        data.push_back(static_cast<unsigned char>(b));
    }

    // Generate and bind the texture
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload texture data to the GPU
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}
