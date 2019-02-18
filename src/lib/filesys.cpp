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
    /*                          ArchiveFile                          */
    /*****************************************************************/

    ArchiveFile::ArchiveFile (const char* aFilePath) {
        if (PHYSFS_exists(aFilePath) == 0) {
            std::cerr << "Error: '" << aFilePath << "' not found" << std::endl;
            err = ArchiveFileError::FILE_NOT_FOUND;
        }
        mFileHandle = PHYSFS_openRead(aFilePath);
        if (!mFileHandle) {
            std::cerr << "Error: '" << aFilePath << "' load failed" << std::endl;
            err = ArchiveFileError::FILE_LOAD_FAILED;
        }
        err = ArchiveFileError::FILE_NO_ERROR;
    }

    ArchiveFile::~ArchiveFile (void) {
        if (mFileHandle) {
            PHYSFS_close(mFileHandle);
        }
    }

    int ArchiveFile::eof (void) {
        return PHYSFS_eof(mFileHandle);
    }

    int ArchiveFile::read (void* aDst, unsigned int aBytes) {
        return (int) PHYSFS_read(mFileHandle, aDst, aBytes, 1);
    }

    int ArchiveFile::readBytes (void* aDst, unsigned int len) {
        return (int) PHYSFS_readBytes(mFileHandle, aDst, len);
    }

    int ArchiveFile::length (void) {
        return (int) PHYSFS_fileLength(mFileHandle);
    }

    int ArchiveFile::seek (int aOffset) {
        PHYSFS_seek(mFileHandle, aOffset);
    }

    int ArchiveFile::tell (void) {
        return (int) PHYSFS_tell(mFileHandle);
    }

    ArchiveFileError ArchiveFile::failure (void) {
        return err;
    }
}
