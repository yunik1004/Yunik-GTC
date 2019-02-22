#include "model.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <GL/glew.h>

namespace YUNIK_GTC {
    /*****************************************************************/
    /*                        ModelIOStream                          */
    /*****************************************************************/

    ModelIOStream::ModelIOStream (void) {
        //
    }

    ModelIOStream::~ModelIOStream (void) {
        delete aFile;
    }

    size_t ModelIOStream::Read (void* pvBuffer, size_t pSize, size_t pCount) {
        return aFile->fread(pvBuffer, pSize, pCount);
    }

    size_t ModelIOStream::Write (const void* pvBuffer, size_t pSize, size_t pCount) {
        return aFile->fwrite(pvBuffer, pSize, pCount);
    }

    aiReturn ModelIOStream::Seek(size_t pOffset, aiOrigin pOrigin) {
        int pos;

        int tellLoc = Tell();
        int endLoc = FileSize();

        switch (pOrigin) {
        case aiOrigin_SET:
            pos = pOffset;
            break;
        case aiOrigin_CUR:
            pos = tellLoc + pOffset;
            break;
        case aiOrigin_END:
            pos = endLoc + pOffset;
            break;
        default:
            return aiReturn_FAILURE;
        }

        if (pos < 0 || pos > endLoc) {
            return aiReturn_FAILURE;
        }
        aFile->seek(pos);
        return aiReturn_SUCCESS;
    }

    size_t ModelIOStream::Tell (void) const {
        return aFile->tell();
    }

    size_t ModelIOStream::FileSize (void) const {
        return aFile->length();
    }

    void ModelIOStream::Flush (void) {
        aFile->flush();
    }

    /*****************************************************************/
    /*                         ModelIOSystem                         */
    /*****************************************************************/
    ModelIOSystem::ModelIOSystem (void) {
        //
    }

    ModelIOSystem::~ModelIOSystem (void) {
        //
    }

    bool ModelIOSystem::Exists (const char* pFile) const {
        return fileExists(pFile);
    }

    char ModelIOSystem::getOsSeparator (void) const {
        return '/';
    }

    Assimp::IOStream* ModelIOSystem::Open (const char* pFile, const char* pMode) {
        ModelIOStream* aModelIOStream = new ModelIOStream();
        aModelIOStream->aFile = new ArchiveFile(pFile);
        return aModelIOStream;
    }

    void ModelIOSystem::Close (Assimp::IOStream* pFile) {
        delete pFile;
    }

    /*****************************************************************/
    /*                             Mesh                              */
    /*****************************************************************/

    Mesh::Mesh (std::vector<MeshData::Vertex> vertices, std::vector<unsigned int> indices, std::vector<MeshData::Texture> textures) {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        setupMesh();
    }

    Mesh::~Mesh (void) {
        //
    }

    void Mesh::setupMesh (void) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(MeshData::Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshData::Vertex), (void*)offsetof(MeshData::Vertex, position));
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshData::Vertex), (void*)offsetof(MeshData::Vertex, normal));
        // vertex texture coordinates
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshData::Vertex), (void*)offsetof(MeshData::Vertex, texCoords));

        glBindVertexArray(0);
    }

    void Mesh::draw (ShaderProgram* sp) {
        unsigned int numDiffuse = 1;
        unsigned int numSpecular = 1;
        unsigned int numAmbient = 1;

        for (unsigned int i = 0; i < textures.size(); ++i) {
            std::string varName;
            if (textures[i].type == aiTextureType_DIFFUSE) {
                varName = "texture_diffuse" + std::to_string(numDiffuse++);
            } else if (textures[i].type == aiTextureType_SPECULAR) {
                varName = "texture_specular" + std::to_string(numSpecular++);
            } else if (textures[i].type == aiTextureType_AMBIENT) {
                varName = "texture_ambient" + std::to_string(numAmbient++);
            }
            
            sp->setFloat(varName, i);
            textures[i].tex->bind();
        }
        Texture::initActiveTexture();

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    /*****************************************************************/
    /*                             Model                             */
    /*****************************************************************/

    Model::Model (const char* aFilePath) {
        Assimp::Importer importer;
        importer.SetIOHandler(new ModelIOSystem());
        const aiScene *scene = importer.ReadFile(aFilePath, aiProcess_Triangulate | aiProcess_FlipUVs);
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cerr << "Error: Assimp - " << importer.GetErrorString() << std::endl;
            return;
        }
        std::string path(aFilePath);
        directory = path.substr(0, path.find_last_of('/') + 1);

        processNode(scene->mRootNode, scene);
    }

    Model::~Model (void) {
        //
    }

    void Model::processNode (aiNode* node, const aiScene* scene) {
        unsigned int i;
        for (i = 0; i < node->mNumMeshes; ++i) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }

        for (i = 0; i < node->mNumChildren; ++i) {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh Model::processMesh (aiMesh* mesh, const aiScene* scene) {
        std::vector<MeshData::Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<MeshData::Texture> textures;

        /* vertex */
        for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
            MeshData::Vertex vertex;
            glm::vec3 vector;
            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.position = vector;
            // normals
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
            // texture coordinates
            if (mesh->mTextureCoords[0]) {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.texCoords = vec;
            } else {
                vertex.texCoords = glm::vec2(0.0f, 0.0f);
            }

            vertices.push_back(vertex);
        }

        /* indices */
        for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; ++j) {
                indices.push_back(face.mIndices[j]);
            }
        }
            
        /* material */
        if (mesh->mMaterialIndex >= 0) {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

            std::vector<MeshData::Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE);
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            std::vector<MeshData::Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR);
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

            std::vector<MeshData::Texture> ambientMaps = loadMaterialTextures(material, aiTextureType_AMBIENT);
            textures.insert(textures.end(), ambientMaps.begin(), ambientMaps.end());
        }

        return Mesh(vertices, indices, textures);
    }

    std::vector<MeshData::Texture> Model::loadMaterialTextures (aiMaterial* mat, aiTextureType type) {
        std::vector<MeshData::Texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i) {
            aiString str;
            mat->GetTexture(type, i, &str);
            std::string filePath(directory);
            filePath += str.C_Str();

            MeshData::Texture texture;
            texture.tex = new Texture(filePath.c_str());
            texture.type = type;
            textures.push_back(texture);
        }
        return textures;
    }

    void Model::draw (ShaderProgram* sp) {
        for (unsigned int i = 0; i < meshes.size(); ++i) {
            meshes[i].draw(sp);
        }
    }
}
