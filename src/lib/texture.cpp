#include "texture.hpp"
#include <GL/glew.h>
#include "image.hpp"

namespace YUNIK_GTC {
    Texture::Texture (const char* aFilePath) {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        // Texture wrapping/filtering options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Image* image = new Image(aFilePath);
        err = image->failure();
        if (err == ArchiveFileError::FILE_NO_ERROR) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->getWidth(), image->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image->getImage());
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        delete image;
    }

    Texture::~Texture (void) {
        glDeleteTextures(1, &texture);
    }

    ArchiveFileError Texture::failure (void) {
        return err;
    }

    void Texture::bind (int textureUnit) {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_2D, texture);
    }

    void Texture::unbind (int textureUnit) {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
