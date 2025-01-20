#version 330 core
layout(location=0)in vec3 position; 
layout(location=1)in vec3 normals; // normals
layout(location=2)in vec2 texCoord; // texture coordinates
layout(location=3)in vec3 tangents; // tangents
layout(location=4)in vec3 bitangents; // bitangents

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 myNormal;
out vec3 FragPos;
out vec2 v_texCoord;


void main()
{

    gl_Position = projection * view * model * vec4(position, 1.0f);

    myNormal = normals;
    // Transform normal into world space
    FragPos = vec3(model* vec4(position,1.0f));

    // Store the texture coordinates
    v_texCoord = texCoord;
}
