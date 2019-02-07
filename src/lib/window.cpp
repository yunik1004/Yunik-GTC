#include <cmath>
#include <iostream>
#include <sstream>
#include "../settings.hpp"
#include "window.hpp"

namespace YUNIK_GTC {
    /* Initialization */
    Window* Window::instance = nullptr;
    GLFWwindow* Window::window = nullptr;
    Scene* Window::scene = nullptr;
    /* Initialization end */

    Window::Window(const char* name, int width, int height, bool resizable) {
        /* Resizability */
        glfwWindowHint(GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);

        /* Anti-aliasing */
        glfwWindowHint(GLFW_SAMPLES, 4);

        /* Window size, name */
        window = glfwCreateWindow(width, height, name, nullptr, nullptr);
        if(window == nullptr) {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        glfwSetWindowAspectRatio(window, width, height);
        if (resizable) {
            /* Size callback */
            glfwSetWindowSizeCallback(window, &windowSizeCallback);
        }

        glfwMakeContextCurrent(window);
        glClearColor(0, 0, 0, 1);

        /* FPS limit: 60 */
        glfwSwapInterval(1);

        /* Initialize glew */
        GLenum errorCode = glewInit();
        if (errorCode != GLEW_OK) {
            std::cerr << "Error: Fail to initialize GLEW - " << glewGetErrorString(errorCode) << std::endl;
            glfwDestroyWindow(window);
            window = nullptr;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        /* OpenGL version check */
        std::stringstream gl_version;
        gl_version << "GL_VERSION_" << YUNIK_GTC_OPENGL_VERSION_MAJOR << "_" << YUNIK_GTC_OPENGL_VERSION_MINOR;
        const char* gl_version_char = gl_version.str().c_str();

        if (!glewIsSupported(gl_version_char)) {
            std::cerr << gl_version_char << " is not avaliable." << std::endl;
            glfwDestroyWindow(window);
            window = nullptr;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        // Set OpenGL options
        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    Window::~Window(void) {
        if(window != nullptr) {
            glfwDestroyWindow(window);
            window = nullptr;
        }
        
        delete scene;
        scene = nullptr;
    }

    void Window::windowSizeCallback(GLFWwindow* window, int w, int h) {
        GLfloat widthFactor = (GLfloat)w / (GLfloat)YUNIK_GTC_DEFAULT_WINDOW_WIDTH;
        GLfloat heightFactor = (GLfloat)h / (GLfloat)YUNIK_GTC_DEFAULT_WINDOW_HEIGHT;

        GLfloat sizeFactor = widthFactor < heightFactor ? widthFactor : heightFactor;

        GLint modifiedWidth =  (int) ceil(YUNIK_GTC_DEFAULT_WINDOW_WIDTH * sizeFactor) + 1;
        GLint modifiedHeight = (int) ceil(YUNIK_GTC_DEFAULT_WINDOW_HEIGHT * sizeFactor) + 1;

        glViewport((int)floor((w - modifiedWidth) / 2.0), (int)floor((h - modifiedHeight) / 2.0), modifiedWidth, modifiedHeight);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glOrtho(-1.0 * sizeFactor, 1.0 * sizeFactor, -1.0 * sizeFactor, 1.0 * sizeFactor, 0.0, 1.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    Window* Window::Instance(void) {
        if(instance == nullptr) {
            setInstance();
        }
        return instance;
    }

    void Window::setInstance(const char* name, int width, int height, bool resizable) {
        if(instance == nullptr) {
            instance = new Window(name, width, height, resizable);
        }
    }

    void Window::purgeInstance(void) {
        delete instance;
        instance = nullptr;
    }

    void Window::setWindowPos(int xpos, int ypos) {
        if(window == nullptr) {
            return;
        }
        glfwSetWindowPos(window, xpos, ypos);
    }

    void Window::setWindowPos_middle(void) {
        if(window == nullptr) {
            return;
        }

        const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        
        int width_interval = (int) floor((mode->width - YUNIK_GTC_DEFAULT_WINDOW_WIDTH) / 2.0 + 0.5);
        int height_interval = (int) floor((mode->height - YUNIK_GTC_DEFAULT_WINDOW_HEIGHT) / 2.0 + 0.5);

        glfwSetWindowPos(window, width_interval, height_interval);
    }

    void Window::setKeyCallback(GLFWkeyfun cbfun) {
        if(window == nullptr) {
            return;
        }
        glfwSetKeyCallback(window, cbfun);
    }

    void Window::setScene(Scene* newScene) {
        if (scene != nullptr) {
            delete scene;
        }
        scene = newScene;
    }

    void Window::updateScene(void) {
        if(scene == nullptr) {
            return;
        }

        Scene* nextScene = scene->update();
        if(nextScene != scene) {
            delete scene;
            scene = nextScene;
        }
    }

    void Window::render(void) {
        if(window == nullptr) {
            return;
        }

        GLint m_viewport[4];

        while (!glfwWindowShouldClose(window)) {
            glGetIntegerv(GL_VIEWPORT, m_viewport);
            glScissor(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
            glEnable(GL_SCISSOR_TEST);

            updateScene();
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
}
