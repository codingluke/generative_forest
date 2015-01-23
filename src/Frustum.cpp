#include "Frustum.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/type_ptr.hpp>

#define ANG2RAD 3.14159265358979323846/180.0

Frustum::Frustum()
{
}

void Frustum::setCamInternals(float angle, float ratio,
                              float nearD, float farD)
{
    // store the information
    this->ratio = ratio;
    this->nearD = nearD;
    this->farD = farD;

    // compute width and height of the near section
    tang = (float) tan(ANG2RAD * angle * 0.5);
    height = nearD * tang;
    width = height * ratio;
}

void Frustum::setCamDef(glm::vec3 &p, glm::vec3 &l, glm::vec3 &u)
{
    //cc = p.copy();
    cc = p;

    // compute the Z axis of the camera referential
    // this axis points in the same direction from
    // the looking direction
    Z = glm::normalize(l - p);
    //Z.normalize();

    // X axis of camera with given "up" vector and Z axis
    X = glm::normalize(Z * u);
    //X.normalize();

    // the real "up" vector is the dot product of X and Z
    Y = X * Z;
}

int Frustum::pointInFrustum(glm::vec3 &p)
{
    float pcz, pcx, pcy, aux;

    // compute vector from camera position to p
    glm::vec3 v = p - cc;

    // compute and test the Z coordinate
    pcz = glm::dot(v, -Z);
    //pcz = v.innerProduct(-Z);
    if (pcz > farD || pcz < nearD) {
        return(false);
    }

    // compute and test the Y coordinate
    pcy = glm::dot(v, Y);
    //pcy = v.innerProduct(Y);
    aux = pcz * tang;
    if (pcy > aux || pcy < -aux) {
        return(false);
    }

    // compute and test the X coordinate
    pcx = glm::dot(v, X);
    //pcx = v.innerProduct(X);
    aux = aux * ratio;
    if (pcx > aux || pcx < -aux) {
        return(false);
    }
    return(true);
}
