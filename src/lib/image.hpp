#pragma once

#include "filesys.hpp"

namespace YUNIK_GTC {
    class ImageFile {
    private:
        int width;
        int height;
        int channels;
        unsigned char* image = nullptr;
        ArchiveFileError err;
    public:
        ImageFile (const char* aFilePath);
        ~ImageFile (void);

        int getWidth (void);
        int getHeight (void);
        unsigned char* getImage (void);
        
        ArchiveFileError failure (void);
    };
}
