#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "assimp/scene.h"
#include "assimp/mesh.h"

#include <vector>

class Mesh
{
    public :
        struct MeshEntry {
            enum BUFFERS {
                VERTEX_BUFFER, TEXCOORD_BUFFER,
                NORMAL_BUFFER, INDEX_BUFFER
            };
            GLuint vao;
            GLuint vbo[4];

            unsigned int elementCount;

            MeshEntry(aiMesh *mesh);
            ~MeshEntry();

            void load(aiMesh *mesh);
            void render();
        };

        std::vector<MeshEntry*> meshEntries;

    public:
        Mesh(char const *filename);
        ~Mesh(void);

        void render();
};

#endif
