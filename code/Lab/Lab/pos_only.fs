#version 420 core
out vec4 FragColor;

in vec3 normal; 

uniform samplerCube depthMap;

in vec3 FragPos;

//Light
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform float far_plane;
uniform vec3 viewPos;

vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);

// Tell whether shadow exists
float ShadowCalculation(vec3 fragPos)
{
    vec3 lightPos = light.position;
    vec3 fragToLight = fragPos - lightPos; 
    float currentDepth = length(fragToLight);
    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float viewDistance = length(viewPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(depthMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= far_plane;   // Undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);
    return shadow;
}

void main()
{
    float shininess = 32;

    vec3 lightPos = light.position;

    vec3 lightDir   = normalize(lightPos - FragPos);
    vec3 viewDir    = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    vec3 fragToLight = FragPos - lightPos; 
    float distance = length(fragToLight);
    float attenuation = 1.0 / (1.0 + 0.14 * distance + 0.07 * (distance * distance));

    // Get color
    vec3 color = vec3(0.5,0.5,0.5); 

    // ambient
    vec3 ambient = light.ambient * vec3(0.2)* color;

    // diffuse 
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * color);

    // specular
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    vec3 specular = spec * light.specular;

    float shadow = ShadowCalculation(FragPos);                      
    vec3 lighting = (ambient + (1.0) * (diffuse + specular ));    

    FragColor = vec4(lighting, 1.0f);
}