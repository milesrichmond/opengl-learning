#ifndef PHYSMATH_H
#define PHYSMATH_H

#include <glm/glm.hpp>

#include <swivel_chair/phys/physObject.h>
#include <swivel_chair/options.h>

class PhysMath {
public:
    static glm::vec3 calculateAttraction(PhysObject &target, PhysObject &acting) {
        glm::vec3 direction = glm::normalize(acting.getPosition() - target.getPosition());
        float distance = glm::distance(target.getPosition(), acting.getPosition());
        float accelMag = ((target.getMass() + acting.getMass()) / pow(distance, 2)) / target.getMass();

        return direction * accelMag * Options::gravityModifier;
    }

    // reflection vec = incoming vec - 2(incoming {dot} normal) x normal
    static void collisionRedirection(PhysObject &target, PhysObject &acting) {
        glm::vec3 normal = glm::normalize(acting.getPosition() - target.getPosition());
        glm::vec3 targetVelocity = target.getVelocity();

        glm::vec3 reflection = Options::restitution * (targetVelocity - 2 * (glm::dot(targetVelocity, normal)) * normal);
        target.collide(reflection);
    }

    static bool haveCollided(PhysObject &target, PhysObject &acting) {
        return (glm::distance(target.getPosition(), acting.getPosition())) < (target.getRadius() + acting.getRadius());
    }
};

#endif