#version 420 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D bottom;
uniform sampler2D chicken;
uniform float h_chicken;

void main()
{
    vec4 texColor_bottom = texture(bottom, TexCoords);
    vec4 texColor_chicken = texture(chicken, TexCoords);
    if((texColor_chicken.a>0.1) && (TexCoords.x<(h_chicken*0.75+0.125)))
        FragColor=texColor_chicken;
    else if(texColor_bottom.a>0.1)
        FragColor=texColor_bottom;
    else
        discard;
}