#version 330 core

in vec3 fragColor;
in vec2 fragTexcoord;
in vec3 fragNormal;

out vec4 fragColorOut;

uniform struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
} material;

uniform sampler2D textureSampler;

void main()
{
    // Apply lighting calculations
    vec3 lightDirection = normalize(vec3(0.0, 0.0, 1.0)); // Example light direction
    vec3 normal = normalize(fragNormal);
    float diffuseFactor = max(dot(normal, lightDirection), 0.0);
    
    vec3 ambient = material.ambient * vec3(texture(textureSampler, fragTexcoord));
    vec3 diffuse = material.diffuse * vec3(texture(textureSampler, fragTexcoord)) * diffuseFactor;
    vec3 specular = material.specular * vec3(texture(textureSampler, fragTexcoord)) * pow(max(dot(reflect(-lightDirection, normal), normalize(-fragColor.xyz)), 0.0), material.shininess);
    
    fragColorOut = vec4(ambient + diffuse + specular, 1.0);
}
