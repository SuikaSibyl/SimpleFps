#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aText;
layout (location = 3) in vec3 aTangent;

out vec3 WorldPos;  
out vec3 Normal;
out vec2 TexCoord;
out vec3 Tangent;

uniform sampler2D T_Water_N;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;

void main()
{
    WorldPos = vec3 ( model * vec4(aPos, 1.0) );
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    Normal = mat3(transpose(inverse(model))) * aNormal;
    Tangent = mat3(model) * aTangent;
    TexCoord = aText;
}
