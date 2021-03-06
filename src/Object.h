#ifndef OBJECT_H
#define OBJECT_H

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <GL/glew.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

class Object
{
private:
    GLuint vertexArrayId;
    GLuint vertexBufferId;
    GLuint normalBufferId;
    GLuint uvBufferId;
    GLuint texture;
    vector<unsigned short> indices;
    vector<glm::vec3> vertices;
    vector<glm::vec2> uvs;
    vector<glm::vec3> normals;

public:
    Object(const char *filename);
    Object(const char *filename, const char *texFilename);
    ~Object(void);

    void render(glm::mat4 model, glm::mat4 view,
        glm::mat4 projection, GLuint programID);

private:
    void bindVertexBuffer();
    void bindNormalBuffer();
    void bindUvBuffer();
    void sendMVP(glm::mat4 model, glm::mat4 view,
     glm::mat4 projection, GLuint programID);
};

#endif
