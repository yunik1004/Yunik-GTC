#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace YUNIK_GTC {
    class Scene {
    public:
        virtual ~Scene(void) {}
        virtual Scene* update(void) = 0;
    };
}
