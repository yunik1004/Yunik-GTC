#ifdef _WIN32
#ifndef _DEBUG
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif
#endif

#include <cstdlib>
#include "lib/projectManager.hpp"
#include "lib/window.hpp"
#include "scene/mainScene.hpp"

int main (int argc, char* argv[]) {
    /* Initialize project manager */
    YUNIK_GTC::ProjectManager::init(argv[0]);

    /* Create window */
    YUNIK_GTC::Window* window = new YUNIK_GTC::Window();
    window->setWindowPos_middle();
    window->setWindowIcon("icon/yunik.png");
    YUNIK_GTC::ProjectManager::setWindow(window);
    
    /* Rendering session */
    YUNIK_GTC::ProjectManager::setScene(new YUNIK_GTC::mainScene());
    YUNIK_GTC::ProjectManager::render();

    /* Terminate session */
    YUNIK_GTC::ProjectManager::deinit();
    return EXIT_SUCCESS;
}
