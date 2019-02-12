#include "filesys.hpp"
#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include "../settings.hpp"

namespace YUNIK_GTC {
    /*****************************************************************/
    /*                            Filesys                            */
    /*****************************************************************/

    void Filesys_init (char* argv0) {
        if (!PHYSFS_init(argv0)) {
            std::cerr << "ERROR: " << PHYSFS_getLastError() << std::endl;
            exit(EXIT_FAILURE);
        }

        /* Mount resource files */
        char path_delimiter = '_';
        std::string path_delimiter_str(1, path_delimiter);
        for (const auto& entry : std::filesystem::directory_iterator(YUNIK_GTC_RESOURCE_DIR)) {
            std::filesystem::path path = entry.path();
            if (path.extension().compare(YUNIK_GTC_RESOURCE_EXTENSION) != 0) {
                continue;
            }

            std::string path_unix = path.string();
            std::replace(path_unix.begin(), path_unix.end(), '\\', '/');

            std::string path_origin = path.stem().string();
            auto start = path_origin.find(path_delimiter_str);
            if (start == std::string::npos) {
                PHYSFS_mount((const char*)path_unix.c_str(), "/", 1);
            } else {
                auto end = path_origin.length() - start;
                std::string path_rel = path_origin.substr(start, end);
                std::replace(path_rel.begin(), path_rel.end(), path_delimiter, '/');

                PHYSFS_mount((const char*)path_unix.c_str(), (const char*)path_rel.c_str(), 1);
            }
        }
    }

    void Filesys_deinit (void) {
        PHYSFS_deinit();
    }

    /*****************************************************************/
    /*                           AudioFile                           */
    /*****************************************************************/

    AudioFile::AudioFile (void) {
        mFileHandle = 0;
    }

    AudioFile::AudioFile (const char* aFilepath) {
        open(aFilepath); // TODO: throw an exception if file does not exist
    }

    AudioFile::AudioFile (PHYSFS_File* fp): mFileHandle(fp) { }

    AudioFile::~AudioFile (void) {
        if (mFileHandle) {
            PHYSFS_close(mFileHandle);
        }
    }

    int AudioFile::eof (void) {
        return PHYSFS_eof(mFileHandle);
    }

    unsigned int AudioFile::read (unsigned char* aDst, unsigned int aBytes) {
        return (unsigned int) PHYSFS_read(mFileHandle, aDst, aBytes, 1);
    }

    unsigned int AudioFile::length (void) {
       return (unsigned int) PHYSFS_fileLength(mFileHandle);
    }

    void AudioFile::seek (int aOffset) {
        PHYSFS_seek(mFileHandle, aOffset);
    }

    unsigned int AudioFile::pos (void) {
        return (unsigned int) PHYSFS_tell(mFileHandle);
    }

    SoLoud::result AudioFile::open (const char* aFilename) {
        if (PHYSFS_exists(aFilename) == 0) {
            std::cerr << "Error: File not found" << std::endl;
            return SoLoud::FILE_NOT_FOUND;
        }
        mFileHandle = PHYSFS_openRead(aFilename);
        if (!mFileHandle) {
            std::cerr << "Error: File load failed" << std::endl;
            return SoLoud::FILE_LOAD_FAILED;
        }
        return SoLoud::SO_NO_ERROR;
    }

    PHYSFS_File* AudioFile::getPhysfsFilePtr (void) {
        return mFileHandle;
    }

    /*****************************************************************/
    /*                           ImageFile                           */
    /*****************************************************************/

    ImageFile::ImageFile (const char* aFilepath) {
        //
    }

    ImageFile::~ImageFile (void) {
        //
    }
}
