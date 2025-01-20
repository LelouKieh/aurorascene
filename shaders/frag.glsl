#version 330 core

// Our light sources
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform float ambientIntensity;
// specular highlights
uniform mat4 view;
uniform sampler2D u_DiffuseMap; 
uniform vec3 emissiveColor;

in vec3 myNormal; // normal data
in vec2 v_texCoord; // texture coordinates from vertex shader
in vec3 FragPos; // fragment position

// The final output color of each 'fragment' from our fragment shader.
out vec4 FragColor;

// constant for specular strength
float specularStrength = 0.5f;


void main()
{
    // Store final texture color
    vec3 diffuseColor;
    diffuseColor = texture(u_DiffuseMap, v_texCoord).rgb;

    // Compute ambient light
    vec3 ambient = ambientIntensity * lightColor;

    // Compute the normal direction
    vec3 norm = normalize(myNormal);
    // From our lights position and the fragment, we can get a vector indicating direction
    vec3 lightDir = normalize(lightPos - FragPos);
    // Compute the diffuse light impact
    float diffImpact = max(dot(norm, lightDir), 0.0);
    vec3 diffuseLight = diffImpact * lightColor;

    // Compute Specular lighting
    vec3 viewPos = vec3(0.0,0.0,0.0);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // Our final color is based on the texture
    vec3 Lighting = diffuseLight + ambient + specular;
    vec3 finalColor = (diffuseColor * Lighting) + emissiveColor;

    if(gl_FrontFacing){
        FragColor = vec4(finalColor, 1.0);
    }else{
        // Additionally color the back side the same color
        FragColor = vec4(finalColor, 1.0);
    }
}
