#ifndef OBJECT_H
#define OBJECT_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <glm/glm.hpp>

class Frustum
{
    private:
        glm::vec3 cc; // camera position
        glm::vec3 X,Y,Z; // the camera referential
        float nearD, farD, ratio, width, height, tang;

    public:
        Frustum();
        ~Frustum();

        void setCamInternals(float angle, float ratio,
                             float nearD, float farD);
        void setCamDef(glm::vec3 &p, glm::vec3 &l, glm::vec3 &u);
        int pointInFrustum(glm::vec3 &p);
};

#endif

