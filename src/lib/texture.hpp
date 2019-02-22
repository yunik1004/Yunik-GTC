#pragma once

#include "filesys.hpp"

namespace YUNIK_GTC {
    class Texture {
    private:
        unsigned int texture;
        ArchiveFileError err;
    public:
        Texture (const char* aFilePath);
        ~Texture (void);

        ArchiveFileError failure (void);

        void bind (int textureUnit = 0);
        static void unbind (int textureUnit = 0);
        static void initActiveTexture (void);
    };
}
