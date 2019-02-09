#pragma once

#include <soloud.h>

namespace YUNIK_GTC {
    class AudioEngine {
    private:
        AudioEngine (void);
        ~AudioEngine (void);

        static AudioEngine* instance;
        static SoLoud::Soloud* engine;

    public:
        static AudioEngine* Instance (void);
        static void purgeInstance (void);
    };
}
