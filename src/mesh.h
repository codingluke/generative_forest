#pragma once

#include "include/GL/glew.h"
#include "include/GL/glfw3.h"

#include "include\assimp\scene.h"
#include "include\assimp\mesh.h"

#include <vector>

class Mesh
{
    public :
        struct MeshEntry {
            static enum BUFFERS {
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
        Mesh(const char *filename);
        ~Mesh(void);

        void render();
};
