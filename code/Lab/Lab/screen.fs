#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform bool bullet_time;

void main()
{
    const float gamma = 2.2;
    const float exposure=1;
    vec3 hdrColor = texture(screenTexture, TexCoords).rgb;

    // Reinhard色调映射
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    // Gamma校正
    mapped = pow(mapped, vec3(1.0 / gamma));

    FragColor = vec4(mapped, 1.0);
    //float gamma = 2.2;

    if(bullet_time == true)
        FragColor = vec4(vec3(1.0 - FragColor), 1.0);
} 