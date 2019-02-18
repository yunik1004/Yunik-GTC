#pragma once

#include <soloud.h>
#include <soloud_file.h>
#include <soloud_wav.h>
#include "filesys.hpp"

namespace YUNIK_GTC {
    class AudioFile: public SoLoud::File {
    private:
        ArchiveFile* aFile;
    public:
        virtual ~AudioFile (void);
        virtual int eof (void);
        virtual unsigned int read (unsigned char* aDst, unsigned int aBytes);
        virtual unsigned int length (void);
        virtual void seek (int aOffset);
        virtual unsigned int pos (void);
        AudioFile (const char* aFilePath);

        ArchiveFileError failure (void);
    };
    
    class AudioWave {
    private:
        AudioFile* fp;
        SoLoud::Wav wave;
    public:
        AudioWave (const char* filePath);
        ~AudioWave (void);

        SoLoud::Wav& getWave (void);
        void setLooping (bool aLoop);

        ArchiveFileError failure (void);
    };
    
    class AudioEngine {
    private:
        AudioEngine (void);
        ~AudioEngine (void);

        static AudioEngine* instance;
        static SoLoud::Soloud* engine;

    public:
        static void init (void);
        static void deinit (void);
        static AudioEngine* Instance (void);

        static void playBackground (AudioWave* audioWave);
    };
}
