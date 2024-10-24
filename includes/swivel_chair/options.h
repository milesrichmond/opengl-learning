#ifndef OPTIONS_H
#define OPTIONS_H

#include <string>
#include <swivel_chair/fileManager.h>

// Loaded options from an options.json
namespace Options {
    std::string optionsPath;
    int targetScene;

    float gameSpeed;
    bool isPaused;
    float gravityModifier;
    float restitution = 0.95; // Value should not be > 0.95 or we start making momentum from nothing

    namespace Controls {
        float zoomSpeed = 1.0f;
    }

    namespace Font {
        std::string file;
        float scale;
    }
}

// Stock scenes from files
namespace Stock {
    const std::string scenes[] = {
        FileSystem::getPath("resources/scenes/scene0.json"),
        FileSystem::getPath("resources/scenes/scene1.json"),
        FileSystem::getPath("resources/scenes/scene2.json")
    };

    namespace Models {
        const std::string sphere = FileSystem::getPath("resources/sphere/sphere.obj");
    }
}

#endif