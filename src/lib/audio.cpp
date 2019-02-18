#include "audio.hpp"
#include <iostream>

namespace YUNIK_GTC {
    /*****************************************************************/
    /*                           AudioFile                           */
    /*****************************************************************/

    AudioFile::AudioFile (const char* aFilePath) {
        aFile = new ArchiveFile(aFilePath);
    }

    AudioFile::~AudioFile (void) {
        delete aFile;
    }

    int AudioFile::eof (void) {
        return aFile->eof();
    }

    unsigned int AudioFile::read (unsigned char* aDst, unsigned int aBytes) {
        return (unsigned int) aFile->read(aDst, aBytes);
    }

    unsigned int AudioFile::length (void) {
       return (unsigned int) aFile->length();
    }

    void AudioFile::seek (int aOffset) {
        aFile->seek(aOffset);
    }

    unsigned int AudioFile::pos (void) {
        return (unsigned int) aFile->tell();
    }

    ArchiveFileError AudioFile::failure (void) {
        return aFile->failure();
    }

    /*****************************************************************/
    /*                           AudioWave                           */
    /*****************************************************************/

    AudioWave::AudioWave (const char* filePath) {
        fp = new AudioFile(filePath);
        if (fp->failure() == ArchiveFileError::FILE_NO_ERROR) {
            wave.loadFile(fp);
        }
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

    ArchiveFileError AudioWave::failure (void) {
        return fp->failure();
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
