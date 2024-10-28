#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <custom/fileManager.hpp>
#include <string>

namespace Opt {
static std::string optionsPath;
static int targetScene;

namespace Font {
static std::string fontPath;
static float scale;
} // namespace Font

static float gameSpeed;
static bool isPaused;

namespace Parameters {
static float gravityCoef;
static float restitutionCoef;
} // namespace Parameters

namespace Controls {
static float zoomSpeed = 1.0f;
}
} // namespace Opt

namespace Stock {
static const std::string scenes[] = {
    FileSystem::getPath("resources/scenes/scene0.json"),
    FileSystem::getPath("resources/scenes/scene1.json"),
    FileSystem::getPath("resources/scenes/scene2.json")};

namespace Models {
static const std::string sphere =
    FileSystem::getPath("resources/sphere/sphere.obj");
}
} // namespace Stock

#endif
