#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../settings.hpp"
#include "scene.hpp"

namespace YUNIK_GTC {
    class Window {
    private:
        Window(const char* name, int width, int height, bool resizable);
        ~Window(void);

        static void windowSizeCallback(GLFWwindow *window, int w, int h);

        static Window* instance;
        static GLFWwindow* window;
        static Scene* scene;

    public:
        static Window* Instance(void);
        static void setInstance(const char* name = YUNIK_GTC_DEFAULT_WINDOW_NAME,
                                int width = YUNIK_GTC_DEFAULT_WINDOW_WIDTH,
                                int height = YUNIK_GTC_DEFAULT_WINDOW_HEIGHT,
                                bool resizable = false);
        static void purgeInstance(void);

        static void setWindowPos(int xpos, int ypos);
        static void setWindowPos_middle(void);

        static void setKeyCallback(GLFWkeyfun cbfun);

        static void setScene(Scene* newScene);
        static void updateScene(void);

        static void render(void);
    };
}
