#pragma once

#include <soloud.h>
#include <soloud_wav.h>

namespace YUNIK_GTC {
    class AudioWave {
    private:
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
        static AudioEngine* Instance (void);
        static void purgeInstance (void);

        static void playBackground (AudioWave* audioWave);
    };
}
