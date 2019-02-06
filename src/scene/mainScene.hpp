#pragma once

#include "../lib/scene.hpp"

namespace YUNIK_GTC {
    class mainScene : public Scene {
    private:
        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
            //
        }
    public:
        mainScene(void) {
            /* Background color - white */
            glClearColor(1.0, 1.0 , 1.0, 0.0);
        }

        ~mainScene(void) {
            //
        }
        
        Scene* update(void) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            //
            return this;
        }
    };
}
