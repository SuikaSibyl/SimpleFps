#version 420 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    mat3 TBN;
} fs_in;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_specular1;
uniform samplerCube depthMap;

uniform vec3 viewPos;

//Light
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

void main()
{    
    float shininess = 20;

    vec3 lightPos = light.position;

    vec3 lightDir   = normalize(lightPos-fs_in.FragPos);
    vec3 viewDir    = normalize(viewPos - fs_in.FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    vec3 normal = normalize(fs_in.Normal);

    // Get color
    vec3 color = texture(texture_diffuse1, fs_in.TexCoords).rgb; 

    // ambient
    vec3 ambient = light.ambient * color * vec3(0.2);

    // diffuse 
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * color);

    // specular
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    vec3 specular = light.ambient * spec;

    FragColor = vec4(ambient + diffuse + specular,1.0);
}