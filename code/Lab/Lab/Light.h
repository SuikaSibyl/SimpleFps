#ifndef _LIGHT_
#define _LIGHT_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Light {
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    Light(glm::vec3 pos,
    glm::vec3 amb,
    glm::vec3 diff,
    glm::vec3 spec):
        position(pos), ambient(amb), diffuse(diff), specular(spec){}
};

#endif // !_LIGHT_
