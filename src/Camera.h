#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <math.h>
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

class Camera
{
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 right;
    glm::vec3 up;

    double window_width;
    double window_height;
    double window_mid_x;
    double window_mid_y;

    double horiz_angle;
    double vert_angle; 

    double mouseSpeed;
    double deltaTime;

public:
    bool key_up;
    bool key_down;
    bool key_left;
    bool key_right;

    GLFWwindow *window;

public:

    Camera(GLFWwindow *window, glm::vec3 position, double window_width, double window_height);
    ~Camera();

    void handleMouseMove(GLFWwindow *window, double mouse_x, double mouse_y);
    const double toRads(const double &angleInDegrees) const;
    void move();
    void moveOnPlaneXY();

    glm::vec3 getPosition() const;
    double getXPos() const;
    double getYPos() const;
    double getZPos() const;

    glm::vec3 getDirection() const;
    double getXDir() const;
    double getYDir() const;
    double getZDir() const;

    glm::vec3 getUp() const;

    float getDistanceTo(glm::vec3 worldCoords) const;

    void setDeltaTime(float delta);
    float getDeltaTime() const;

    void setDirection(glm::vec3 newDirection);
    void setDirection(float vert, float hor);
private:
    void initialize(glm::vec3 position, double window_width, double window_height);

};

#endif //CAMERA_H