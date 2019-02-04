#pragma once

#include "../lib/scene.hpp"

namespace YUNIK_GTC {
    class mainScene : public Scene {
    private:
        static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    public:
        mainScene(void);
        ~mainScene(void);
        Scene* update(void);
    };
}
