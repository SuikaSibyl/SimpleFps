#version 420 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;
uniform int state;

void main()
{   

    if(state == 1 )
    {
        float gray = 0.257 * texture(skybox, TexCoords).r + 0.504 * texture(skybox, TexCoords).g + 0.098 * texture(skybox, TexCoords).b;
        FragColor = vec4(gray,gray,gray,1);
     }
     else if(state == 2 )
     {
         float gray = 0.257 * texture(skybox, TexCoords).r + 0.504 * texture(skybox, TexCoords).g + 0.098 * texture(skybox, TexCoords).b;
         FragColor = vec4(gray,0,0,1);
      }
    else
        FragColor = texture(skybox, TexCoords);
}