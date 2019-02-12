#include "projectManager.hpp"
#include "audio.hpp"
#include "filesys.hpp"

namespace YUNIK_GTC {
    /* Initialization */
    ProjectManager* ProjectManager::instance = nullptr;
    Scene* ProjectManager::scene = nullptr;
    Window* ProjectManager::window = nullptr;

    ProjectManager::ProjectManager (char* argv0) {
        /* Initialize filesystem */
        YUNIK_GTC::Filesys_init(argv0);

        /* Initialize window system */
        YUNIK_GTC::Window_init();

        /* Initialize audio engine */
        YUNIK_GTC::AudioEngine::init();
    }
    
    ProjectManager::~ProjectManager (void) {
        purgeScene();
        purgeWindow();

        YUNIK_GTC::AudioEngine::deinit();
        YUNIK_GTC::Window_deinit();
        YUNIK_GTC::Filesys_deinit();

        instance = nullptr;
    }

    void ProjectManager::init (char* argv0) {
        if (instance == nullptr) {
            instance = new ProjectManager(argv0);
        }
    }

    void ProjectManager::deinit (void) {
        delete instance;
        instance = nullptr;
    }

    ProjectManager* ProjectManager::Instance (void) {
        return instance;
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
