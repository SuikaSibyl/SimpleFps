#version 420 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec4 FragPosLightSpace;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform int state;
//Material
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

uniform Material material;
//Light
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform float rate;
uniform vec3 viewPos;

void main()
{    
    vec3 normal = Normal+texture(texture3,TexCoord).rgb*vec3(20,20,20);
    //vec3 normal = texture(texture3,TexCoord).rgb;
    vec3 nnormal = normalize(Normal);
    vec3 material_specular = ((1-rate) * texture(texture1,TexCoord).rgb + rate * texture(texture2,TexCoord).rgb) *vec3(2)* texture(texture3,TexCoord).rgb;
    vec3 material_ambient = material_specular * vec3(0.2);
    vec3 material_diffuse = material_specular * vec3(0.5);

    vec3 lightPos   = light.position;
    vec3 lightDir   = normalize(lightPos - FragPos);
    vec3 viewDir    = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float distance    = length(light.position - FragPos);
    float attenuation = 1.0/(1+0.09f*distance+0.032f*distance*distance);

    vec3 l_ambient  = light.ambient * attenuation; 
    vec3 l_diffuse  = light.diffuse * attenuation;
    vec3 l_specular = light.specular * attenuation;

    // ambient
    vec3 ambient = l_ambient * material_ambient;
  	
    // diffuse 
    vec3 norm = normalize(normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = l_diffuse * (diff * material_diffuse);

    // specular
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    vec3 specular = l_specular * (spec * material_specular);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result,1.0);

    if(state==1)
    {
        float gray=result.x*0.257+result.y*0.504+result.z*0.098;
        FragColor = vec4( gray,gray,gray, 1.0);
    }
    else if(state==2)
    {
        FragColor = vec4( 1,0.4,0.4, 1.0);
    }
    else if(state==3)
    {
        FragColor = vec4( 0.4,0.4,1, 1.0);
    }
    else if(state==4)
    {
        FragColor = vec4(1, 0.4, 0.4, 1.0);
    }
}