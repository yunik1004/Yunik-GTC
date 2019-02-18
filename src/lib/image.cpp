#include "image.hpp"
#include <assert.h>
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include <stb_image.h>

namespace YUNIK_GTC {
    static int ArchiveFileRead (void* user, char* data, int size) {
        ArchiveFile* aFile = (ArchiveFile*)user;
        return aFile->readBytes(data, size);
    }

    static void ArchiveFileSkip (void* user, int offset) {
        ArchiveFile* aFile = (ArchiveFile*)user;
        const int currentPosition = aFile->tell();
        assert(currentPosition >= 0);
        const int newPosition = currentPosition + offset;
        const int success = aFile->seek(newPosition);
        assert(success);
    }

    static int ArchiveFileEOF (void* user) {
        ArchiveFile* aFile = (ArchiveFile*)user;
        return aFile->eof();
    }

    static const stbi_io_callbacks ArchiveFileCallbacks = {
        ArchiveFileRead,
        ArchiveFileSkip,
        ArchiveFileEOF
    };

    ImageFile::ImageFile (const char* aFilePath) {
        ArchiveFile* aFile = new ArchiveFile(aFilePath);
        err = aFile->failure();
        if (err == ArchiveFileError::FILE_NO_ERROR) {
            image = (unsigned char*)stbi_load_from_callbacks(&ArchiveFileCallbacks, aFile, &width, &height, &channels, STBI_rgb_alpha);
        }
        delete aFile;
    }

    ImageFile::~ImageFile (void) {
        if (image != nullptr) {
            stbi_image_free(image);
        }
    }

    int ImageFile::getWidth (void) {
        return width;
    }

    int ImageFile::getHeight (void) {
        return height;
    }

    unsigned char* ImageFile::getImage (void) {
        return image;
    }

    ArchiveFileError ImageFile::failure (void) {
        return err;
    }
}
