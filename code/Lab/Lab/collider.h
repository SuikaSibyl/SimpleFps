#ifndef _COLLIDER_
#define _COLLIDER_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

using namespace std;

static double dxz = 0.5;
static double dyu = 1;
static double dyd = 1.9f;
static double odyd = 2;

class Collider
{
public:
	Collider(float a1, float a2, float a3, float a4, float a5, float a6 ):
		x_min(a1), x_max(a2), y_min(a3), y_max(a4), z_min(a5), z_max(a6) {}
	bool figure_in(glm::vec3 point){ 
		//cout << point.y << " " << y_min << " " << y_max << endl;
		return (
		(point.x - dxz <= x_max) && (point.x + dxz >= x_min)
		&& (point.y -2 + 0.01 <= y_max) && (float(point.y) + dxz >= y_min)
		&&(point.z - dxz <= z_max) && (point.z + dxz >= z_min)); 
	}
	bool figure_on(glm::vec3& point) {
		bool result= (
			(point.x - dxz <= x_max) && (point.x + dxz >= x_min)
			&& (point.y - odyd <= y_max)
			&& (point.z - dxz <= z_max) && (point.z + dxz >= z_min));
		if (result == true) point.y = y_max + odyd;
		return result;
	}
private:
	float x_min;
	float x_max;
	float y_min;
	float y_max;
	float z_min;
	float z_max;

};

#endif // !_COLLIDER_
