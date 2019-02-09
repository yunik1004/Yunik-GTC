#ifdef _WIN32
#ifndef _DEBUG
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif
#endif

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "settings.hpp"
#include "lib/audio.hpp"
#include "lib/projectManager.hpp"
#include "lib/window.hpp"
#include "scene/mainScene.hpp"

int main (int argc, char* argv[]) {
    glfwSetErrorCallback([](int errorCode, const char* errorDescription) {
        std::cerr << "Error: " << errorDescription << std::endl;
    });

    /* Initialize glfw */
    if (!glfwInit()) {
        std::cerr << "Error: Fail to initialize GLFW" << std::endl;
        return EXIT_FAILURE;
    }

    /* OpenGL version */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, YUNIK_GTC_OPENGL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, YUNIK_GTC_OPENGL_VERSION_MINOR);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    /* Create window */
    YUNIK_GTC::Window* window = new YUNIK_GTC::Window();
    window->setWindowPos_middle();
    YUNIK_GTC::ProjectManager::setWindow(window);

    /* Initialize audio engine */
    YUNIK_GTC::AudioEngine::Instance();
    
    /* Rendering session */
    YUNIK_GTC::ProjectManager::setScene(new YUNIK_GTC::mainScene());
    YUNIK_GTC::ProjectManager::render();

    /* Terminate session */
    YUNIK_GTC::AudioEngine::purgeInstance();
    YUNIK_GTC::ProjectManager::purgeInstance();
    glfwTerminate();
    return EXIT_SUCCESS;
}
