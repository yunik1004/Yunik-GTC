#include "audio.hpp"

namespace YUNIK_GTC {
    /*****************************************************************/
    /*                           AudioWave                           */
    /*****************************************************************/

    AudioWave::AudioWave (const char* filePath) {
        wave.load(filePath);
    }

    AudioWave::~AudioWave (void) {
        //
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

    void AudioEngine::playBackground (AudioWave* audioWave) {
        engine->playBackground(audioWave->getWave());
    }
}
