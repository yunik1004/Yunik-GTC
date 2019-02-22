#pragma once

#include <string>
#include <vector>
#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>
#include <assimp/material.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include "filesys.hpp"
#include "shader.hpp"
#include "texture.hpp"

namespace YUNIK_GTC {
    class ModelIOStream : public Assimp::IOStream {
    friend class ModelIOSystem;
    protected:
        ModelIOStream (void);
        ArchiveFile* aFile;
    public:
        ~ModelIOStream (void);
        size_t Read (void* pvBuffer, size_t pSize, size_t pCount);
        size_t Write (const void* pvBuffer, size_t pSize, size_t pCount);
        aiReturn Seek (size_t pOffset, aiOrigin pOrigin);
        size_t Tell (void) const;
        size_t FileSize (void) const;
        void Flush (void);
    };

    class ModelIOSystem : public Assimp::IOSystem {
    public:
        ModelIOSystem (void);
        ~ModelIOSystem (void);
        bool Exists (const char* pFile) const;
        char getOsSeparator (void) const;
        Assimp::IOStream* Open (const char* pFile, const char* pMode);
        void Close (Assimp::IOStream* pFile);
    };

    namespace MeshData {
        struct Vertex {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec2 texCoords;
        };

        struct Texture {
            YUNIK_GTC::Texture* tex;
            aiTextureType type;
        };
    }

    class Mesh {
    private:
        /* Mesh data */
        std::vector<MeshData::Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<MeshData::Texture> textures;

        /* Render data */
        unsigned int VAO, VBO, EBO;

        void setupMesh (void);
    public:
        Mesh (std::vector<MeshData::Vertex> vertices, std::vector<unsigned int> indices, std::vector<MeshData::Texture> textures);
        ~Mesh (void);
        void draw (ShaderProgram* sp);
    };

    class Model {
    private:
        std::vector<Mesh> meshes;

        std::string directory;

        void processNode (aiNode* node, const aiScene* scene);
        Mesh processMesh (aiMesh* mesh, const aiScene* scene);

        std::vector<MeshData::Texture> loadMaterialTextures (aiMaterial* mat, aiTextureType type);
    public:
        Model (const char* aFilePath);
        ~Model (void);

        void draw (ShaderProgram* sp);
    };
}
