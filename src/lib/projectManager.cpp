#include "projectManager.hpp"

namespace YUNIK_GTC {
    /* Initialization */
    ProjectManager* ProjectManager::instance = nullptr;
    Scene* ProjectManager::scene = nullptr;
    Window* ProjectManager::window = nullptr;

    ProjectManager::ProjectManager (void) {
        //
    }
    
    ProjectManager::~ProjectManager (void) {
        purgeScene();
        purgeWindow();
        instance = nullptr;
    }

    ProjectManager* ProjectManager::Instance (void) {
        if (instance == nullptr) {
            instance = new ProjectManager();
        }
        return instance;
    }

    void ProjectManager::purgeInstance (void) {
        delete instance;
        instance = nullptr;
    }

    void ProjectManager::setWindow (Window* newWindow) {
        if (window != nullptr) {
            delete window;
        }
        window = newWindow;
    }

    void ProjectManager::purgeWindow (void) {
        delete window;
        window = nullptr;
    }

    void ProjectManager::setScene (Scene* newScene) {
        if (scene != nullptr) {
            delete scene;
        }
        scene = newScene;
    }

    bool ProjectManager::updateScene (void) {
        if (scene == nullptr) {
            return false;
        }
        Scene* nextScene = scene->update();
        if (nextScene != scene) {
            delete scene;
            scene = nextScene;
        }
        return true;
    }

    void ProjectManager::purgeScene (void) {
        delete scene;
        scene = nullptr;
    }

    bool ProjectManager::render (void) {
        if (window == nullptr || scene == nullptr) {
            return false;
        }
        return window->render(scene);
    }
}
