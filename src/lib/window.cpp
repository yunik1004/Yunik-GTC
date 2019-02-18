#include "window.hpp"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include "projectManager.hpp"

namespace YUNIK_GTC {
    /*****************************************************************/
    /*                    Window initialization                      */
    /*****************************************************************/

    void Window_init (void) {
        /* Initialize glfw */
        glfwSetErrorCallback([](int errorCode, const char* errorDescription) {
            std::cerr << "Error: " << errorDescription << std::endl;
        });

        if (!glfwInit()) {
            std::cerr << "Error: Fail to initialize GLFW" << std::endl;
            exit(EXIT_FAILURE);
        }

        /* OpenGL version */
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, YUNIK_GTC_OPENGL_VERSION_MAJOR);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, YUNIK_GTC_OPENGL_VERSION_MINOR);

        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    }

    void Window_deinit (void) {
        glfwTerminate();
    }

    /*****************************************************************/
    /*                            Window                             */
    /*****************************************************************/

    Window::Window (const char* name, int width, int height, bool resizable) {
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
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        // Set OpenGL options
        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    Window::~Window (void) {
        if (window != nullptr) {
            glfwDestroyWindow(window);
        }

        if (windowIconImage != nullptr) {
            delete windowIconImage;
        }
    }

    void Window::windowSizeCallback (GLFWwindow* window, int w, int h) {
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

    bool Window::setWindowPos (int xpos, int ypos) {
        if(window == nullptr) {
            return false;
        }
        glfwSetWindowPos(window, xpos, ypos);
        return true;
    }

    bool Window::setWindowPos_middle (void) {
        if(window == nullptr) {
            return false;
        }

        const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        
        int width_interval = (int) floor((mode->width - YUNIK_GTC_DEFAULT_WINDOW_WIDTH) / 2.0 + 0.5);
        int height_interval = (int) floor((mode->height - YUNIK_GTC_DEFAULT_WINDOW_HEIGHT) / 2.0 + 0.5);

        glfwSetWindowPos(window, width_interval, height_interval);
        return true;
    }

    bool Window::setWindowIcon (const char* filePath) {
        windowIconImage = new Image(filePath);
        windowIcon.width = windowIconImage->getWidth();
        windowIcon.height = windowIconImage->getHeight();
        windowIcon.pixels = windowIconImage->getImage();
        glfwSetWindowIcon(window, 1, &windowIcon);
        return true;
    }

    bool Window::setKeyCallback (GLFWkeyfun cbfun) {
        if(window == nullptr) {
            return false;
        }
        glfwSetKeyCallback(window, cbfun);
        return true;
    }

    bool Window::render (Scene* scene) {
        if(window == nullptr || scene == nullptr) {
            return false;
        }

        GLint m_viewport[4];

        while (!glfwWindowShouldClose(window)) {
            glGetIntegerv(GL_VIEWPORT, m_viewport);
            glScissor(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
            glEnable(GL_SCISSOR_TEST);

            scene->update();
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        return true;
    }
}
