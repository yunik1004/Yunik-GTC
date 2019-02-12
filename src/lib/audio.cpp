#include "audio.hpp"
#include <iostream>

namespace YUNIK_GTC {
    /*****************************************************************/
    /*                           AudioWave                           */
    /*****************************************************************/

    AudioWave::AudioWave (const char* filePath) {
        fp = new AudioFile(filePath);
        wave.loadFile(fp);
    }

    AudioWave::~AudioWave (void) {
        wave.stop();
        delete fp;
    }

    SoLoud::Wav& AudioWave::getWave (void) {
        return wave;
    }

    void AudioWave::setLooping (bool aLoop) {
        wave.setLooping(aLoop);
    }

    /*****************************************************************/
    /*                         AudioEngine                           */
    /*****************************************************************/

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

    void AudioEngine::init (void) {
        if (instance == nullptr) {
            instance = new AudioEngine();
        }
    }

    void AudioEngine::deinit (void) {
        delete instance;
        instance = nullptr;
    }

    AudioEngine* AudioEngine::Instance (void) {
        return instance;
    }

    void AudioEngine::playBackground (AudioWave* audioWave) {
        engine->playBackground(audioWave->getWave());
    }
}
