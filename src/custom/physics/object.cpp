#include <custom/physics/object.hpp>

#include <glm/glm.hpp>

#include <custom/model/model.hpp>
#include <custom/state/gameOptions.hpp>
#include <custom/fileManager.hpp>

namespace Phys {

Object::Object(std::string id, float mass, glm::vec3 position, glm::vec3 scale) : m_Model(Model(Stock::Models::sphere)) {
    this->id = id;
    m_Mass = mass;
    m_Position = position;
    m_LastPosition = position;
    m_Force = glm::vec3(0.0f);
    
    m_Model.scale(scale);
    m_Radius = fmax(fmax(scale.x, scale.y), scale.z);
    
    setupModelPosition();
}

void Object::draw(Shader &shader) {
    m_Model.draw(shader);
}

void Object::updateForTime(float dt) {
    glm::vec3 velocity = this->getVelocity() + this->getAcceleration();
    m_LastPosition = m_Position;
    m_Position += velocity;
    
    m_Force = glm::vec3(0.0f);
    
    updateModel(dt);
}

void Object::applyForce(glm::vec3 force) {
    m_Force += force;
}

void Object::collideWith(Object &acting) {
    // Calculate the vector between the two objects
    glm::vec3 delta = acting.m_Position - m_Position;
    float distance = glm::length(delta);
    
    // Check for collision
    if (distance < (m_Radius + acting.m_Radius)) {
        // Normalize the collision vector
        glm::vec3 collisionNormal = glm::normalize(delta);
        
        // Calculate relative velocity
        glm::vec3 relativeVelocity = getVelocity() - acting.getVelocity();
        
        // Calculate the velocity along the collision normal
        float velocityAlongNormal = glm::dot(relativeVelocity, collisionNormal);
        
        // Only process if objects are moving towards each other
        if (velocityAlongNormal < 0) {
            // Calculate restitution (bounciness) factor, for simplicity we'll use 1.0 (perfectly elastic)
            float restitution = 1.0f;

            // Calculate impulse scalar
            float impulseScalar = -(1 + restitution) * velocityAlongNormal;
            impulseScalar /= (1 / m_Mass + 1 / acting.m_Mass);

            // Apply impulse to both objects
            glm::vec3 impulse = impulseScalar * collisionNormal;
            applyForce(impulse);
            acting.applyForce(-impulse);
        }
    }
}

void Object::setupModelPosition() {
    m_Model.translate(m_Position, 1.0f);
}

void Object::updateModel(float dt) {
    glm::vec3 velocity = this->getVelocity();
    m_Model.translate(velocity, dt);
}

}