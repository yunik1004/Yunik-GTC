#pragma once

#include <physfs.h>

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
        int readBytes (void* aDst, unsigned int len);
        int length (void);
        int seek (int aOffset);
        int tell (void);

        ArchiveFileError failure (void);
    };
}
