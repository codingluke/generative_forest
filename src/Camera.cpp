#include "Camera.h"
#include "glm/ext.hpp"



Camera::Camera(GLFWwindow *window, glm::vec3 position, double window_width, double window_height)
{
    this->window = window;
    initialize(position, window_width, window_height);
}

void Camera::initialize(glm::vec3 position, double window_width, double window_height)
{
    this->position = position;
    this->direction = glm::vec3(0.0, 0.0, 0.0);
    this->right = glm::vec3(0.0, 0.0, 0.0);
    this->up = glm::vec3(0.0, 0.0, 0.0);
    this->window_width = window_width;
    this->window_height = window_height;
    this->window_mid_x = window_width / 2.0;
    this->window_mid_y = window_height / 2.0;
    this->key_up = false;
    this->key_down = false;
    this->key_left = false;
    this->key_right = false;
    this->horiz_angle = 3.14f;
    this->vert_angle = 0.0f;
    this->mouseSpeed = 0.05f;
    this->deltaTime = 1.0f;

    glfwSetCursorPos(window, window_mid_x, window_mid_y);
}

Camera::~Camera()
{

}

void Camera::handleMouseMove(GLFWwindow *window, double mouse_x, double mouse_y)
{
    // glfwGetCursorPos(window, &mouse_x, &mouse_y);
    std::cout << horiz_angle << " " << vert_angle << std::endl;
    double horiz_add = mouseSpeed * deltaTime * (window_mid_x - mouse_x);
    double vert_add = mouseSpeed * deltaTime * (window_mid_y - mouse_y); 
    horiz_angle += horiz_add; 
    vert_angle += vert_add;
    std::cout << "res " << double(window_mid_x - mouse_x) << " " << double(window_mid_y - mouse_y) << std::endl;
    direction = glm::vec3(
        cos(vert_angle) * sin(horiz_angle),
        sin(vert_angle),
        cos(vert_angle) * cos(horiz_angle)
    );

    
    // std::cout << "direction:" << glm::to_string(direction) << std::endl;

    right = glm::vec3(
        sin(horiz_angle - 3.14f/2.0f),
        0,
        cos(horiz_angle - 3.14f/2.0f)
    );

    up = glm::cross(right, direction);

    // if (direction[0] < -90)
    // {
    //     direction[0]= -90;
    // }
    // if (direction[0] > 90)
    // {
    //     direction[0] = 90;
    // }
    // if (direction[1] < 0)
    // {
    //     direction[1] = 360;
    // }
    // if (direction[1] > 360)
    // {
    //     direction[1] = -360;
    // }

    glfwSetCursorPos(window, window_mid_x, window_mid_y);
}
const double Camera::toRads(const double &angleInDegrees) const
{
    return angleInDegrees * ( 3.14592654 / 180.);
}

void Camera::move(double deltaTime)
{
    glm::vec3 movement = glm::vec3(0.0, 0.0, 0.0);

    if (key_up) 
        movement += direction * float(deltaTime);
    if (key_down)
        movement -= direction * float(deltaTime);
    if (key_left)
        movement -= right * float(deltaTime);
    if (key_right)
        movement += right * float(deltaTime);

    position += movement; 
    // double sin_x_rot = sin(toRads(direction[0]));
    // double cos_x_rot = cos(toRads(direction[0]));
    // double sin_y_rot = sin(toRads(direction[1]));
    // double cos_y_rot = cos(toRads(direction[1]));

    // double pitchLimitFactor = cos_x_rot; // This cancels out moving on the Z axis when we're looking up or down
    // bool moved = false;

    // if (key_up)
    // {
    //     movement[0] += sin_y_rot * pitchLimitFactor;
    //     movement[1] += -sin_x_rot;
    //     movement[2] += -cos_y_rot * pitchLimitFactor;
    //     moved = true;
    // }
 
    // if (key_down)
    // {
    //     movement[0] += -sin_y_rot * pitchLimitFactor;
    //     movement[1] += sin_x_rot;
    //     movement[2] += cos_y_rot * pitchLimitFactor;
    //     moved = true;
    // }
 
    // if (key_left)
    // {
    //     movement[0] += -cos_y_rot;
    //     movement[2] += -sin_y_rot;
    //     moved = true;
    // }
 
    // if (key_right)
    // {
    //     movement[0] += cos_y_rot;
    //     movement[2] += sin_y_rot;
    //     moved = true;
    // }
    
    // if (moved)
    //     movement = glm::normalize(movement);

    
    // movement *= deltaTime;
    // position += movement;
    // std::cout << "direction:" << glm::to_string(direction) << std::endl;
    // std::cout << "movement:" << glm::to_string(movement) << std::endl;
    // std::cout << "position:" << glm::to_string(position) << std::endl;   
}

glm::vec3 Camera::getPosition() const
{
    return position;
}
double Camera::getXPos() const
{
    return position[0];
}
double Camera::getYPos() const
{   
    return position[1];
}
double Camera::getZPos() const
{
    return position[2];
}

glm::vec3 Camera::getDirection() const
{
    return direction;
}
double Camera::getXDir() const
{
    return direction[0];
}
double Camera::getYDir() const
{
    return direction[1];
}
double Camera::getZDir() const
{
    return direction[2];
}

glm::vec3 Camera::getUp() const 
{
    return up;
}