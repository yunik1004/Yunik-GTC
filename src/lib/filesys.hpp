#pragma once

#include <physfs.h>
#include <iostream>
#include <string>

namespace YUNIK_GTC {
    void Filesys_init (char* argv0);
    void Filesys_deinit (void);

    enum class ArchiveFileError {
        FILE_NOT_FOUND,
        FILE_LOAD_FAILED,
        FILE_NO_ERROR
    };

    class ArchiveFile {
    private:
        PHYSFS_File* mFileHandle;
        ArchiveFileError err;
    public:
        ArchiveFile (const char* aFilePath);
        ~ArchiveFile (void);
        int eof (void);
        int read (void* aDst, unsigned int aBytes);
        int fread (void* aDst, unsigned int aBytes, unsigned int aCount);
        int readBytes (void* aDst, unsigned int len);
        int fwrite (const void* aDst, unsigned int aBytes, unsigned int aCount);
        int length (void);
        int seek (int aOffset);
        int tell (void);
        int flush (void);

        ArchiveFileError failure (void);
    };

    bool fileExists (const char* aFilePath);
    char* getFileContents (const char* aFilePath);
}
