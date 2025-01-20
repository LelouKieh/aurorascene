#include "Shader.hpp"
#include <iostream>
#include <fstream>


// Constructor: Initializes the Shader object
Shader::Shader() {}

// Destructor: Cleans up shader resources by deleting the program
Shader::~Shader() {
    glDeleteProgram(m_shaderID);
}

// Activates the shader for use in rendering
void Shader::Bind() const {
    glUseProgram(m_shaderID);
}

// Deactivates the currently bound shader
void Shader::Unbind() const {
    glUseProgram(0);
}

// Logs messages with a system tag for easier debugging
void Shader::Log(const char* system, const char* message) {
    std::cout << "[" << system << "] " << message << "\n";
}


// Loads a shader source file and returns its content as a string
// @param fname: Path to the shader source file
// @return The shader source code as a string
std::string Shader::LoadShader(const std::string& fname) {
    std::string result;
    std::ifstream myFile(fname.c_str());

    if (myFile.is_open()) {
        std::string line;
        while (getline(myFile, line)) {
            result += line + '\n';
        }
        myFile.close();
    } else {
        Log("LoadShader", "File not found. Try an absolute file path to verify the file exists.");
    }

    return result;
}

// Creates and links a shader program using the given vertex and fragment shader source codes
// @param vertexShaderSource: The source code for the vertex shader
// @param fragmentShaderSource: The source code for the fragment shader
void Shader::CreateShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) {
    unsigned int program = glCreateProgram();

    // Compile the vertex and fragment shaders
    unsigned int myVertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    // Attach shaders to the program and link
    glAttachShader(program, myVertexShader);
    glAttachShader(program, myFragmentShader);
    glLinkProgram(program);
    glValidateProgram(program);

    // Detach and delete shaders after linking
    glDetachShader(program, myVertexShader);
    glDetachShader(program, myFragmentShader);
    glDeleteShader(myVertexShader);
    glDeleteShader(myFragmentShader);

    if (!CheckLinkStatus(program)) {
        Log("CreateShader", "ERROR: Shader did not link! Check for compile errors.");
    }

    m_shaderID = program;
}

// Compiles a shader from its source code
// @param type: The type of shader (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
// @param source: The source code of the shader
// @return The compiled shader ID
unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // Check for compilation errors
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* errorMessages = new char[length];
        glGetShaderInfoLog(id, length, &length, errorMessages);

        if (type == GL_VERTEX_SHADER) {
            Log("CompileShader ERROR", "GL_VERTEX_SHADER compilation failed!");
        } else if (type == GL_FRAGMENT_SHADER) {
            Log("CompileShader ERROR", "GL_FRAGMENT_SHADER compilation failed!");
        }
        Log("CompileShader ERROR", errorMessages);

        delete[] errorMessages;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

// Checks if the shader program linked successfully
// @param programID: The shader program ID
// @return true if the program linked successfully, false otherwise
bool Shader::CheckLinkStatus(GLuint programID) {
    int result;
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &length);
        char* errorMessages = new char[length];
        glGetProgramInfoLog(programID, length, &length, errorMessages);

        SDL_Log("ERROR in linking process\n");
        SDL_Log("%s\n", errorMessages);
        delete[] errorMessages;
        return false;
    }

    return true;
}


// Returns the shader program ID
GLuint Shader::GetID() const {
    return m_shaderID;
}

// Sets a uniform 4x4 matrix in the shader
// @param name: Name of the uniform variable
// @param value: Pointer to the 4x4 matrix
void Shader::SetUniformMatrix4fv(const GLchar* name, const GLfloat* value) {
    GLint location = glGetUniformLocation(m_shaderID, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, value);
}

// Sets a vec4 uniform in the shader
// @param name: Name of the uniform variable
// @param v0, v1, v2, v3: Values of the vec4
void Shader::SetUniform4f(const GLchar* name, float v0, float v1, float v2, float v3) {
    GLint location = glGetUniformLocation(m_shaderID, name);
    glUniform4f(location, v0, v1, v2, v3);
}

// Sets a vec3 uniform in the shader
// @param name: Name of the uniform variable
// @param v0, v1, v2: Values of the vec3
void Shader::SetUniform3f(const GLchar* name, float v0, float v1, float v2) {
    GLint location = glGetUniformLocation(m_shaderID, name);
    glUniform3f(location, v0, v1, v2);
}

// Sets a single integer uniform in the shader
// @param name: Name of the uniform variable
// @param value: The integer value
void Shader::SetUniform1i(const GLchar* name, int value) {
    GLint location = glGetUniformLocation(m_shaderID, name);
    glUniform1i(location, value);
}

// Sets a single float uniform in the shader
// @param name: Name of the uniform variable
// @param value: The float value
void Shader::SetUniform1f(const GLchar* name, float value) {
    GLint location = glGetUniformLocation(m_shaderID, name);
    glUniform1f(location, value);
}
