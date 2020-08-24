#version 420 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    mat3 TBN;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    vs_out.FragPos = vec3(model * vec4(position, 1.0));
    vs_out.Normal = transpose(inverse(mat3(model))) * normal;
    vs_out.TexCoords = texCoords;

    
   vec3 T = normalize(vec3(model * vec4(tangent,   0.0)));
   vec3 B = normalize(vec3(model * vec4(bitangent, 0.0)));
   vec3 N = normalize(vec3(model * vec4(normal,    0.0)));
   vs_out.TBN = mat3(T, B, N);
}