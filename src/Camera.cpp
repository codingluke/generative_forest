#include "Camera.h"
#include "glm/ext.hpp"
#include <math.h>

Camera::Camera(GLFWwindow *window, glm::vec3 position, double window_width, double window_height)
{
    this->window = window;
    initialize(position, window_width, window_height);
}

void Camera::initialize(glm::vec3 position, double window_width, double window_height)
{
    glfwSetCursorPos(window, window_mid_x, window_mid_y);
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
    this->horiz_angle = 0.0f;
    this->vert_angle = 0.0f;
    this->mouseSpeed = 0.03f;
    this->deltaTime = 0.0f;
}

Camera::~Camera()
{}

void Camera::handleMouseMove(GLFWwindow *window, double mouse_x, double mouse_y)
{

    double horiz_add = mouseSpeed * deltaTime * (window_mid_x - mouse_x);
    double vert_add = mouseSpeed * deltaTime * (window_mid_y - mouse_y);
    horiz_angle += horiz_add;
    vert_angle += vert_add;
    if (vert_angle > M_PI / 2)
        vert_angle = M_PI / 2;
    else if (vert_angle < M_PI / (-2))
        vert_angle = M_PI / (-2);
    if (horiz_angle > 2 * M_PI)
        horiz_angle = horiz_angle - (2 * M_PI);
    else if (horiz_angle < 0)
        horiz_angle = (2 * M_PI) - horiz_angle;
    direction = glm::vec3(
        cos(vert_angle) * sin(horiz_angle),
        sin(vert_angle),
        cos(vert_angle) * cos(horiz_angle)
        );
    right = glm::vec3(
        sin(horiz_angle - 3.14f/2.0f),
        0,
        cos(horiz_angle - 3.14f/2.0f)
        );
    up = glm::cross(right, direction);
    glfwSetCursorPos(window, window_mid_x, window_mid_y);
}

void Camera::move()
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
}

void Camera::moveOnPlaneXY()
{
    glm::vec3 movement = glm::vec3(0.0, 0.0, 0.0);
    glm::vec3 adjustedDirection = direction;
    adjustedDirection[1] = 0.0;
    glm::vec3 adjustedRight = right;
    adjustedRight[1] = 0.0;
    if (key_up)
        movement += adjustedDirection;
    if (key_down)
        movement -= adjustedDirection;
    if (key_left)
        movement -= adjustedRight;
    if (key_right)
        movement += adjustedRight;

    if (glm::length(movement) > 0) {
        movement = glm::normalize(movement);
        movement *= float(deltaTime);
    }
    position += movement;
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

float Camera::getDistanceTo(glm::vec3 worldCoords) const
{
    glm::vec3 distVec = position - worldCoords;
    float res = 0;
    float length = glm::length(distVec);
    if (length > 0)
        res = length;
    return res;
}

void Camera::setDirection(glm::vec3 newDirection)
{
    this->direction = glm::vec3(newDirection);
}

void Camera::setDirection(float vert, float hor)
{
    this->horiz_angle = hor;
    this->vert_angle = vert;
    handleMouseMove(window, window_mid_x, window_mid_y);
}


void  Camera::setDeltaTime(float delta)
{
    this->deltaTime = delta;
}

float Camera::getDeltaTime() const
{   
    return deltaTime;
}
