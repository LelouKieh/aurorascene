#version 410 core

// Input vertex attributes
layout(location = 0) in vec3 aPos;    // Vertex position
layout(location = 1) in vec3 aColor;  // Vertex color
layout(location = 3) in vec2 aTexCoords; // Texture coordinates

// Uniforms
uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_Projection;

// Outputs to fragment shader
out vec3 fragColor;
out vec3 fragPos;
out vec2 TexCoords;

void main()
{
    fragColor = aColor;
    fragPos = aPos;

    // Map position to texture coordinates (assuming your plane is from -1 to 1)
    TexCoords = aTexCoords;

    gl_Position = u_Projection * u_ViewMatrix * u_ModelMatrix * vec4(aPos, 1.0);
}
