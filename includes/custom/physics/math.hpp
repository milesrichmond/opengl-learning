#ifndef PHYSMATH_HPP
#define PHYSMATH_HPP

#include <glm/glm.hpp>
#include <custom/physics/object.hpp>

namespace Phys {
    glm::vec3 calculateAttraction(Object &target, Object &acting);
}

#endif