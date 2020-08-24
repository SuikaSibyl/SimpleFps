#include"Camera.h"
#include<iostream>

using namespace std;

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    glm::vec3 tPosition = Position;
    float velocity = MovementSpeed * deltaTime * 2;
    if (mode == "zoom")
    {
        if (direction == FORWARD)
            tPosition += Front * velocity;
        if (direction == BACKWARD)
            tPosition -= Front * velocity;
        if (direction == LEFT)
            tPosition -= Right * velocity;
        if (direction == RIGHT)
            tPosition += Right * velocity;
        Position = tPosition;
    }
    else if (mode == "fps")
    {
        float posy = Position.y;
        if (direction == FORWARD)
            tPosition += velocity * glm::normalize(Front * glm::vec3(1, 0, 1));
        if (direction == BACKWARD)
            tPosition -= velocity * glm::normalize(Front * glm::vec3(1, 0, 1));
        if (direction == LEFT)
            tPosition -= velocity * glm::normalize(Right * glm::vec3(1, 0, 1));
        if (direction == RIGHT)
            tPosition += velocity * glm::normalize(Right * glm::vec3(1, 0, 1));

        if ((direction == SPACE) && (offground == false))
        {
            tPosition.y += deltaTime * 0.3f;
            y_velocity += 0.3f;
            offground = true;
        }

        bool collide = false;
        for (auto iter = v_collider.begin(); iter != v_collider.end(); iter++)
        {
            if (iter->figure_in(tPosition))
            {
                collide = true;
                break;
            }
        }
        if (!collide) 
            Position = tPosition;
    }
}

void Camera::Update(float deltaTime)
{
    glm::vec3 tPosition = Position + glm::vec3(0, y_velocity * 30 * deltaTime, 0);
    if ((x_velocity <= -0.001 || x_velocity >= 0.001 ) || (z_velocity <= -0.001 || z_velocity >= 0.001))
    {
        //cout << x_velocity << " " << z_velocity << endl;
        float newx_velocity;
        float newy_velocity;
        tPosition = Position + glm::vec3(x_velocity,0, z_velocity) * deltaTime * glm::vec3(10);
        tPosition = tPosition + glm::vec3(0, y_velocity * 30 * deltaTime, 0);
        if (x_velocity != 0)
        {
            if (x_velocity < 0)
                newx_velocity = x_velocity + deltaTime * 10;
            else
                newx_velocity = x_velocity - deltaTime * 10;
            if (x_velocity * newx_velocity < 0)
                x_velocity = 0;
            else
                x_velocity = newx_velocity;
        }
        if (z_velocity != 0)
        {
            if (z_velocity < 0)
                newx_velocity = z_velocity + deltaTime * 10;
            else
                newx_velocity = z_velocity - deltaTime * 10;
            if (z_velocity * newx_velocity < 0)
                z_velocity = 0;
            else
                z_velocity = newx_velocity;
        }
    }
    bool collide_on = false;
    for (auto iter = v_collider.begin(); iter != v_collider.end(); iter++)
    {
        if (iter->figure_on(tPosition))
        {
            collide_on = true;
            break;
        }
    }
    //cout << collide_on << " " << tPosition.y << endl;
    if (collide_on == true)
    {
        y_velocity = 0;
        offground = false;
        Position = tPosition;
    }
    else
    {
        if (offground == false)
        {
            offground = true;
            y_velocity = 0;
        }
        y_velocity -= deltaTime;
        Position = tPosition;
        /*if (Position.y + y_velocity >= 0)
        {
            Position.y += y_velocity * 30 * deltaTime;
        }
        else
        {
            y_velocity = 0;
            Position.y = 0;
            offground = false;
        }*/
    }
}


void Camera::Getshooted(glm::vec3 velo)
{
    glm::vec3 nvelo = glm::normalize(velo);
    x_velocity = nvelo.x * 2;
    z_velocity = nvelo.z * 2;
}