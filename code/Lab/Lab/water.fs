#version 420 core
out vec4 FragColor;

in vec3 WorldPos;  
in vec3 Normal;
in vec2 TexCoord;
in vec3 Tangent;

// texture samplers
uniform sampler2D T_Water_N;
uniform sampler2D T_Perlin_Noise_M;

uniform int state;
uniform samplerCube cubeMap;
uniform vec3 cameraPos;
uniform vec3 LightLocation;
uniform float totalTime;

vec2 CalTexcoord(vec2 uv, vec2 scale, vec2 panner)
{
    return uv * scale + panner;
}

vec3 UnpackNormal(vec3 normal)
{
    vec3 N = normalize(Normal);
    vec3 T = vec3(0,0,1);
    //normalize(Tangent - N * Tangent * N);
    vec3 B = cross(N, T);
    mat3 TBN = mat3(T,B,N);
    return normalize(TBN * normal);
}

vec3 BlendAngleCorrectedNormals(vec3 baseNormal, vec3 additionalNormal)
{
    baseNormal.b += 1;
    additionalNormal *= vec3(-1, -1, 1);
    vec3 nnormal =  dot(baseNormal, additionalNormal) * baseNormal - baseNormal.b * additionalNormal;
    return normalize(nnormal);
}

vec3 GetBaseNormal()
{    
    vec2 ttexcoord = CalTexcoord(TexCoord,vec2(0.05, 0.08),totalTime * vec2(-0.03, -0.02));
    vec3 nnormal = texture(T_Water_N,ttexcoord).rgb;
    nnormal = normalize(2 * nnormal - 1);
    return nnormal;
}

vec3 GetAdditionNormal()
{
    float time = sin(WorldPos.x / 150 + 0.4 * totalTime);
    vec2 ttexcoord = CalTexcoord(TexCoord,vec2(0.18, 0.15),time * vec2(-0.06,-0.04));
    vec3 nnormal = texture(T_Water_N,ttexcoord).rgb;
    nnormal = normalize(2 * nnormal - 1);
    return nnormal;
} 

float GetNoiseAlpha()
{
    vec2 ttexcoord = 0.05 * TexCoord;
    vec4 noiseTex = vec4(texture(T_Perlin_Noise_M,ttexcoord).rgb,1);
    float alpha = noiseTex.r;
    return alpha * 0.3;
} 

vec3 GetSmallWaveNormal(float noise)
{
    vec2 ttexcoord = CalTexcoord(TexCoord,vec2(0.75, 0.75),totalTime * vec2(-0.07, -0.07));
    vec3 nnormal = texture(T_Water_N,ttexcoord).rgb;
    nnormal = (1 - noise) * nnormal + noise * vec3(0, 0, 1);
    nnormal = normalize(2 * nnormal - 1);
    return nnormal;
}

void main()
{
    vec3 ViewDir = normalize(cameraPos - WorldPos);
    vec3 lightDir = normalize(LightLocation - WorldPos);

    float noise = GetNoiseAlpha();
    vec3 baseNormal = GetBaseNormal();
    vec3 addNormal = GetAdditionNormal();
    vec3 largeWaveNormal = BlendAngleCorrectedNormals(baseNormal, addNormal);
    vec3 smallWaveNormal = GetSmallWaveNormal(noise);
    vec3 normal = BlendAngleCorrectedNormals(largeWaveNormal, smallWaveNormal);
    normal = UnpackNormal(normal);

    float diffuse = 0.7 * clamp(dot(normal, lightDir), 0, 1);
    float ambient = 0.1;
    vec3 reflectDir = normalize(reflect(-lightDir,normal));
    float specular = pow(clamp(dot(reflectDir,ViewDir),0,1),2.0);

    vec3 R = reflect(-ViewDir,normal);
    vec4 reflectedColor = texture(cubeMap, R);

    vec3 T = refract(-ViewDir, normal, 0.9);
    vec4 refractedColor = texture(cubeMap, T);

    float fresnel = 0.4 + 0.6 * pow(1.0 - dot(ViewDir, normal), 6.0);

    FragColor = (mix(refractedColor, reflectedColor, fresnel) );

    FragColor = FragColor *( specular + diffuse + ambient);

    FragColor = FragColor * vec4(1,1,1,0.9f);

    if(state == 1 )
    {
        float gray = 0.257 * FragColor.r + 0.504 * FragColor.g + 0.098 * FragColor.b ;
     }
}
