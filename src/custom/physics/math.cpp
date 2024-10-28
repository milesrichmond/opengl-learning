#include <custom/physics/math.hpp>

#include <glm/glm.hpp>
#include <custom/physics/object.hpp>
#include <custom/state/gameOptions.hpp>

namespace Phys {

glm::vec3 gravAttraction(Object &target, Object &acting) {
    glm::vec3 direction = glm::normalize(acting.getPosition() - target.getPosition());
    float distance = glm::distance(target.getPosition(), acting.getPosition());
    float accelMag = ((target.getMass() + acting.getMass()) / pow(distance, 2)) / target.getMass();

    return direction * accelMag * Opt::Parameters::gravityCoef;
}

}