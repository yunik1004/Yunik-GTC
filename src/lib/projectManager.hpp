#pragma once

#include "scene.hpp"
#include "window.hpp"

namespace YUNIK_GTC {
    class ProjectManager {
    private:
        ProjectManager (void);
        ~ProjectManager (void);

        static ProjectManager* instance;
        static Scene* scene;
        static Window* window;
    public:
        static ProjectManager* Instance (void);
        static void purgeInstance (void);

        static void setWindow (Window* newWindow);
        static void purgeWindow (void);

        static void setScene (Scene* newScene);
        static bool updateScene (void);
        static void purgeScene (void);

        static bool render (void);
    };
}
