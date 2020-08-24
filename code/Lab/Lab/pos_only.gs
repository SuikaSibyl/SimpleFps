#version 420 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 normal; 

in VS_OUT {
    vec3 mod_Pos;
    vec3 FragPos;
} gs_in[];

uniform mat4 model;

out vec3 FragPos;

vec3 GetNormal()
{
   vec3 a = gs_in[0].mod_Pos - gs_in[1].mod_Pos;
   vec3 b = gs_in[2].mod_Pos - gs_in[1].mod_Pos;
   return normalize(cross(b, a));
}

void main() {    
    normal = normalize(mat3(transpose(inverse(model))) * GetNormal());

    gl_Position = gl_in[0].gl_Position; 
    FragPos = gs_in[0].FragPos;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position; 
    FragPos = gs_in[1].FragPos;
    EmitVertex();

    gl_Position = gl_in[2].gl_Position; 
    FragPos = gs_in[2].FragPos;
    EmitVertex();

    EndPrimitive();
}