#ifndef _CHICKEN_
#define _CHICKEN_

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include "bullet.h"

class chicken
{
public:
	float getH()
	{
		float result = 1.0 * health / 100;
		return result;
	}
	void update(glm::vec3 pos, float deltatime);
	void checkShooted(std::vector<bullet>& bullets);
	//void draw(float timegap, Shader& shader);
	glm::mat4 glmGetModel();
	bool alive = true;
	void shoot(std::vector<bullet>& bullets, float deltatime);

	int frame = 1;
	float sigma_time = 0;
	float shoot_time = 0;
private:
	glm::vec3 position= glm::vec3(0, 0, -30);
	glm::vec3 front=glm::vec3(0,0,1);
	int health = 100;
	bool getShoted(glm::vec3 bull_pos);
};

#endif // !_CHICKEN_
