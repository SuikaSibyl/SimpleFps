#ifndef _BULLET_
#define _BULLET_

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

class bullet
{
public:
	bullet(glm::vec3 Pos, glm::vec3 Fro, bool ene) :Position(Pos),Front(Fro), enemy(ene){}
	glm::vec3 Position;
	glm::vec3 Front;
	void update(float deltatime);
	bool out_of_range = false;
	bool enemy;
};

#endif // !_BULLET_
