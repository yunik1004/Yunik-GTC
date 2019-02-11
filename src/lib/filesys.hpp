#pragma once

#include <physfs.h>
#include <soloud_file.h>

namespace YUNIK_GTC {
    void Filesys_init (char* argv0);
    void Filesys_deinit (void);

    class PhysfsFile: public SoLoud::File {
    private:
        PHYSFS_File* mFileHandle;
    public:
        virtual ~PhysfsFile (void);
        virtual int eof (void);
        virtual unsigned int read (unsigned char* aDst, unsigned int aBytes);
        virtual unsigned int length (void);
        virtual void seek (int aOffset);
        virtual unsigned int pos (void);
        PhysfsFile (void);
        PhysfsFile (const char* aFilepath);
        PhysfsFile (PHYSFS_File* fp);
        SoLoud::result open (const char* aFilename);
        virtual PHYSFS_File* getPhysfsFilePtr (void);
    };
}
