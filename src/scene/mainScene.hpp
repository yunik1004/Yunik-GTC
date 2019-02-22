#pragma once

#include "../lib/audio.hpp"
#include "../lib/model.hpp"
#include "../lib/scene.hpp"
#include "../lib/shader.hpp"

namespace YUNIK_GTC {
    class mainScene : public Scene {
    private:
        static void keyCallback (GLFWwindow* window, int key, int scancode, int action, int mods) {
            //
        }
        AudioWave* wave;
        Model* model;
        ShaderProgram* sp;
    public:
        mainScene (void) {
            /* Background color - white */
            glClearColor(1.0, 1.0 , 1.0, 0.0);

            wave = new AudioWave("music/Tenshi ni Fureta yo!(Arrange by yunik1004).mp3");
            wave->setLooping(true);
            if (wave->failure() == ArchiveFileError::FILE_NO_ERROR) {
                AudioEngine::playBackground(wave);
            }

            model = new Model("model/nanosuit.obj");
            sp = new ShaderProgram("shader/model.vert", "shader/model.frag");
        }

        ~mainScene (void) {
            delete wave;
        }
        
        Scene* update (void) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            //
            model->draw(sp);
            return this;
        }
    };
}
