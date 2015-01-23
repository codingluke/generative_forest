#include "Object.h"
#include "objloader.hpp"
#include "texture.hpp"

Object::Object(const char *filename)
{
    loadAssImp(filename, indices, vertices, uvs, normals);
    bindVertexBuffer();
    bindNormalBuffer();
    bindUvBuffer();
}

Object::Object(const char *filename, const char *texFilename) : Object(filename)
{
  // Load the texture
  texture = loadBMP_custom(texFilename);

}

void Object::bindUvBuffer()
{
    glGenBuffers(1, &uvBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, uvBufferId);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2),
                 &uvs[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1); // siehe layout im vertex shader
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

void Object::bindNormalBuffer()
{
    GLuint normalBufferId; // Hier alles analog für Normalen in location == 2
    glGenBuffers(1, &normalBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferId);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3),
                 &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2); // siehe layout im vertex shader
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

void Object::bindVertexBuffer()
{
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    glGenBuffers(1, &vertexBufferId); // Kennung erhalten
    // Daten zur Kennung definieren
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    // Buffer zugreifbar für die Shader machen
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
                 &vertices[0], GL_STATIC_DRAW);

    // Erst nach glEnableVertexAttribArray kann DrawArrays auf die Daten zugreifen...
    // Mapping zum Vertex Shader
    glEnableVertexAttribArray(0); // siehe layout im vertex shader: location = 0
    glVertexAttribPointer(0,  // location = 0
            3,  // Datenformat vec3: 3 floats fuer xyz
            GL_FLOAT,
            GL_FALSE, // Fixedpoint data normalisieren ?
            0, // Eckpunkte direkt hintereinander gespeichert
            (void*) 0); // abweichender Datenanfang ?
}

Object::~Object()
{
    glDeleteBuffers(1, &vertexBufferId);
    glDeleteBuffers(1, &normalBufferId);
    glDeleteBuffers(1, &uvBufferId);
    glDeleteTextures(1, &texture);
}

void Object::render(glm::mat4 model, glm::mat4 view,
                    glm::mat4 projection, GLuint programID)
{
    if (texture) {
        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

	      // Set our "myTextureSampler" sampler to user Texture Unit 0
        glUniform1i(glGetUniformLocation(programID, "myTextureSampler"), 0);
    } else {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    sendMVP(model, view, projection, programID);
    glBindVertexArray(vertexArrayId);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

void Object::sendMVP(glm::mat4 model, glm::mat4 view,
                     glm::mat4 projection, GLuint programID)
{
    GLuint id = glGetUniformLocation(programID, "M");
    glUniformMatrix4fv(id, 1, GL_FALSE, &model[0][0]);
    id = glGetUniformLocation(programID, "V");
    glUniformMatrix4fv(id, 1, GL_FALSE, &view[0][0]);
    id = glGetUniformLocation(programID, "P");
    glUniformMatrix4fv(id, 1, GL_FALSE, &projection[0][0]);

    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 MVP = projection * view * model;
    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform, konstant fuer alle Eckpunkte
    id = glGetUniformLocation(programID, "MVP");
    glUniformMatrix4fv(id, 1, GL_FALSE, &MVP[0][0]);
}
