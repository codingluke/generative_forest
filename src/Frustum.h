#ifndef FRUSTUM_H
#define FRUSTUM_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <glm/glm.hpp>

class Frustum
{
    private:
        glm::vec3 cc; // camera position
        glm::vec3 X,Y,Z; // the camera referential
        float nearD, farD, rat, width, height, tang;

    public:
        Frustum();
        ~Frustum();

        void setCamInternals(float angle, float rat,
                             float nearD, float farD);
        void setCamDef(const glm::vec3 &p, const glm::vec3 &l, const glm::vec3 &u);
        bool pointInFrustum(const glm::vec3 &p);
};

#endif

