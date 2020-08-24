#ifndef _MATERIAL_
#define _MATERIAL_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

    Material(glm::vec3 amb,
        glm::vec3 diff,
        glm::vec3 spec,
        float shin){
        ambient = amb;
        diffuse = diff;
        specular = spec;
        shininess = shin;
    }
};

#endif // !_MATERIAL_
