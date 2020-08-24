#version 420 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT {
    vec3 mod_Pos;
    vec3 FragPos;
} vs_out;

out vec3 FragPos;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vs_out.mod_Pos = aPos;
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
}