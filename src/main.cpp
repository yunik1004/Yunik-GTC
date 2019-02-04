#ifdef _WIN32
#ifndef _DEBUG
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif
#endif

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "lib/window.hpp"
#include "scene/mainScene.hpp"

static void errorCallback(int errorCode, const char *errorDescription);

int main (int argc, char *argv[]) {
	glfwSetErrorCallback(errorCallback);

	/* Initialize glfw */
	if (!glfwInit()) {
        fprintf(stderr, "Error: Fail to initialize GLFW");
		exit(EXIT_FAILURE);
	}

    /* OpenGL version : 4.3 */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    /* Create window */
	YUNIK_GTC::Window::setInstance();
	YUNIK_GTC::Window::setWindowPos_middle();
	
	/* Rendering session */
	YUNIK_GTC::Window::setScene(new YUNIK_GTC::mainScene());
	YUNIK_GTC::Window::render();

	/* Terminate session */
	YUNIK_GTC::Window::purgeInstance();
    glfwTerminate();
    return 0;
}

static void errorCallback(int errorCode, const char *errorDescription) {
	fprintf(stderr, "Error: %s\n", errorDescription);
}
