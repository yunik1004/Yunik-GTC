#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../settings.hpp"
#include "image.hpp"
#include "scene.hpp"

namespace YUNIK_GTC {
    void Window_init (void);
    void Window_deinit (void);

    class Window {
    private:
        static void windowSizeCallback (GLFWwindow *window, int w, int h);

        GLFWwindow* window = nullptr;
        GLFWimage windowIcon;
        ImageFile* windowIconImage = nullptr;

    public:
        Window (const char* name = YUNIK_GTC_DEFAULT_WINDOW_NAME,
                int width = YUNIK_GTC_DEFAULT_WINDOW_WIDTH,
                int height = YUNIK_GTC_DEFAULT_WINDOW_HEIGHT,
                bool resizable = false);
        ~Window (void);

        bool setWindowPos (int xpos, int ypos);
        bool setWindowPos_middle (void);

        bool setWindowIcon (const char* filePath);

        bool setKeyCallback (GLFWkeyfun cbfun);

        bool render (Scene* scene);
    };
}
