#include "audio.hpp"

namespace YUNIK_GTC {
    /* Initialization */
    AudioEngine* AudioEngine::instance = nullptr;
    SoLoud::Soloud* AudioEngine::engine = nullptr;

    AudioEngine::AudioEngine (void) {
        engine = new SoLoud::Soloud();
        engine->init();
    }

    AudioEngine::~AudioEngine (void) {
        delete engine;
        engine = nullptr;
        instance = nullptr;
    }

    AudioEngine* AudioEngine::Instance (void) {
        if (instance == nullptr) {
            instance = new AudioEngine();
        }
        return instance;
    }

    void AudioEngine::purgeInstance (void) {
        delete instance;
        instance = nullptr;
    }
}
