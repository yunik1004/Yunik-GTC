#pragma once

#include <soloud.h>
#include <soloud_wav.h>
#include "filesys.hpp"

namespace YUNIK_GTC {
    class AudioWave {
    private:
        AudioFile* fp;
        SoLoud::Wav wave;
    public:
        AudioWave (const char* filePath);
        ~AudioWave (void);

        SoLoud::Wav& getWave (void);
        void setLooping (bool aLoop);
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
