#pragma once

#include <physfs.h>
#include <soloud_file.h>

namespace YUNIK_GTC {
    void Filesys_init (char* argv0);
    void Filesys_deinit (void);

    class AudioFile: public SoLoud::File {
    private:
        PHYSFS_File* mFileHandle;
    public:
        virtual ~AudioFile (void);
        virtual int eof (void);
        virtual unsigned int read (unsigned char* aDst, unsigned int aBytes);
        virtual unsigned int length (void);
        virtual void seek (int aOffset);
        virtual unsigned int pos (void);
        AudioFile (void);
        AudioFile (const char* aFilepath);
        AudioFile (PHYSFS_File* fp);
        SoLoud::result open (const char* aFilename);
        virtual PHYSFS_File* getPhysfsFilePtr (void);
    };

    class ImageFile {
    private:
    public:
        ImageFile (const char* aFilepath);
        ~ImageFile (void);
    };
}
