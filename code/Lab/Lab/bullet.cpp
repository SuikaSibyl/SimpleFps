#include "bullet.h"

void bullet::update(float velXdeltatime)
{
	if (out_of_range == false)
	{
		Position +=  Front * velXdeltatime;
	}
	if ((!((Position.x <= 5) && ( Position.x >=-5) && (Position.y <= 10) && (Position.y >= -2) && (Position.z <= 10) && (Position.z >= -10))) && 
		(!((Position.x <= 10) && (Position.x >= -10) && (Position.y <= 10) && (Position.y >= -2) && (Position.z <= -10) && (Position.z >= -40))))
		out_of_range = true;
}