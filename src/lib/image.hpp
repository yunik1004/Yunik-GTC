#pragma once

#include "filesys.hpp"

namespace YUNIK_GTC {
    class Image {
    private:
        int width;
        int height;
        int channels;
        unsigned char* image = nullptr;
        ArchiveFileError err;
    public:
        Image (const char* aFilePath);
        ~Image (void);

        int getWidth (void);
        int getHeight (void);
        unsigned char* getImage (void);
        
        ArchiveFileError failure (void);
    };
}
