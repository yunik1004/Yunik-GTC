#include "mainScene.hpp"

namespace YUNIK_GTC {
    void mainScene::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        //
    }

    mainScene::mainScene(void) {
        /* Background color - white */
        glClearColor(1.0, 1.0 , 1.0, 0.0);
    }

    mainScene::~mainScene(void) {
        //
    }

    Scene* mainScene::update(void) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //
        return this;
    }
}
