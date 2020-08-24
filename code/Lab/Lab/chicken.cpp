#include "Chicken.h"

glm::mat4 chicken::glmGetModel() {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	model = glm::translate(model, glm::vec3(0, -4.5, 0));
	float rotate = glm::dot(glm::normalize(front), glm::vec3(0, 0, 1));
	float theta = glm::acos(rotate);
	if (front.x >= 0)
		model = glm::rotate(model, theta, glm::vec3(0.0, 1.0, 0.0));
	else
		model = glm::rotate(model, theta, glm::vec3(0.0, -1.0, 0.0));
	
	return model;
}


void chicken::update(glm::vec3 pos, float deltatime)
{
	front = pos - position;
	position += front * glm::vec3(0.2f, 0.2f, 0.2f) * deltatime;
}

void chicken::checkShooted(std::vector<bullet>& v_bullet)
{
    auto iter = v_bullet.begin();
    while (iter != v_bullet.end())
    {
		if (getShoted(iter->Position)==true)
		{
			health -= 1;
			iter = v_bullet.erase(iter);
			if (health <= 0)
				alive = false;
		}
		else
			iter++;
    }
}

bool chicken::getShoted(glm::vec3 bull_pos)
{
	glm::vec3 dis = bull_pos - position;
	float distance = glm::dot(dis, dis);
	if (distance < 0.5)
	{
		return true;
	}
	return false;
}


void chicken::shoot(std::vector<bullet>& bullets, float deltatime)
{
	shoot_time += deltatime;
	if (shoot_time >= 1)
	{
		bullets.push_back(bullet(position, front, true));
		shoot_time = 0;
	}
}