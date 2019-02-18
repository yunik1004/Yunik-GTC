#pragma once

#include <physfs.h>
#include <iostream>
#include <string>

namespace PhysFS {
    typedef enum {
        READ,
        WRITE,
        APPEND
    } mode;

    class base_fstream {
    protected:
        PHYSFS_File* const file;
    public:
        base_fstream (PHYSFS_File* file);
        virtual ~base_fstream (void);
        size_t length (void);
    };

    class ifstream : public base_fstream, public std::istream {
    public:
        ifstream (std::string const& filename);
        virtual ~ifstream (void);
    };
}

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

    class ifstream : public PhysFS::ifstream {
    public:
        ifstream (std::string const& filename);
        virtual ~ifstream (void);
    };
}
