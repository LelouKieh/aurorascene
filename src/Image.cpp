#include "Image.hpp"
#include <fstream>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <memory>

// Constructor: Initializes the Image object with the specified file path
Image::Image(std::string filepath) : m_filepath(filepath) {}

// Destructor: Cleans up allocated memory for pixel data
Image::~Image() {
    if (m_pixelData != NULL) {
        delete[] m_pixelData; // Free dynamically allocated memory
    }
}

// Loads a PPM image from the file and optionally flips the image vertically
void Image::LoadPPM(bool flip) {
    // Open the PPM file for reading
    std::ifstream ppmFile(m_filepath.c_str());
    if (ppmFile.is_open()) {
        std::string line;
        std::cout << "Reading in PPM file: " << m_filepath << std::endl;
        unsigned int iteration = 0; // Tracks which part of the file is being read
        unsigned int pos = 0;       // Tracks the position in the pixel data array

        // Read the file line by line
        while (getline(ppmFile, line)) {
            // Ignore comment lines
            if (line[0] == '#') {
                continue;
            }

            // Parse the magic number (e.g., P3 or P6)
            if (line[0] == 'P') {
                magicNumber = line;
            }
            // Parse the width and height of the image
            else if (iteration == 1) {
                char* token = strtok((char*)line.c_str(), " ");
                m_width = atoi(token); // First token: width
                token = strtok(NULL, " ");
                m_height = atoi(token); // Second token: height
                std::cout << "PPM width, height = " << m_width << ", " << m_height << "\n";

                // Allocate memory for pixel data
                if (m_width > 0 && m_height > 0) {
                    m_pixelData = new uint8_t[m_width * m_height * 3]; // RGB per pixel
                    if (m_pixelData == NULL) {
                        std::cout << "Unable to allocate memory for PPM" << std::endl;
                        exit(1);
                    }
                } else {
                    std::cout << "PPM not parsed correctly: width and/or height is 0" << std::endl;
                    exit(1);
                }
            }
            // Parse the max color value (optional, currently ignored)
            else if (iteration == 2) {
                // Max color range (e.g., 255) stored here
            }
            // Parse the pixel data
            else {
                m_pixelData[pos] = (uint8_t)atoi(line.c_str()); // Store pixel value
                ++pos;
            }
            iteration++;
        }
        ppmFile.close(); // Close the file
    } else {
        std::cout << "Unable to open PPM file: " << m_filepath << std::endl;
    }

    // Flip the image vertically if requested
    if (flip) {
        uint8_t* copyData = new uint8_t[m_width * m_height * 3]; // Temporary buffer
        for (int i = 0; i < m_width * m_height * 3; ++i) {
            copyData[i] = m_pixelData[i];
        }

        unsigned int pos = (m_width * m_height * 3) - 1; // Start from the last pixel
        for (int i = 0; i < m_width * m_height * 3; i += 3) {
            m_pixelData[pos] = copyData[i + 2];     // Red
            m_pixelData[pos - 1] = copyData[i + 1]; // Green
            m_pixelData[pos - 2] = copyData[i];     // Blue
            pos -= 3;
        }
        delete[] copyData; // Free temporary buffer
    }
}

// Sets the color of a specific pixel at (x, y) to (r, g, b)
void Image::SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    if (x > m_width || y > m_height) {
        return; // Out of bounds
    } else {
        size_t index = (x * 3) + m_height * (y * 3);
        m_pixelData[index] = r;
        m_pixelData[index + 1] = g;
        m_pixelData[index + 2] = b;
    }
}

// Prints all pixel data in the image to the console
void Image::PrintPixels() {
    for (int x = 0; x < m_width * m_height * 3; ++x) {
        std::cout << " " << (int)m_pixelData[x];
    }
    std::cout << "\n";
}

// Returns a pointer to the pixel data array
uint8_t* Image::GetPixelDataPtr() {
    return m_pixelData;
}
