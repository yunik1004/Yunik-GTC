#include "audio.hpp"
#include "filesys.hpp"
#include <iostream>

namespace YUNIK_GTC {
    /*****************************************************************/
    /*                           AudioWave                           */
    /*****************************************************************/

    AudioWave::AudioWave (const char* filePath) {
        PhysfsFile* fp = new PhysfsFile(filePath);
        wave.loadFile(fp);
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
