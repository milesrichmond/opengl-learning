#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <glm/glm.hpp>

#include <custom/model/model.hpp>
#include <custom/fileManager.hpp>
#include <custom/state/gameOptions.hpp>

namespace Phys {

class Object {
public:
    // An identifier that should be unique to each instance of PhysObject
    std::string id;
    
    // Initial velocity will be 0
    Object(std::string id, float mass = 1.0f, glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(0.01f));
    
    // Dude, it calls the model's draw method. What else do you want?
    void draw(Shader &shader);
    
    // Adjusts the model's position based upon the calculated current velocity
    // (including the acceleration based on the net force experienced)
    void updateForTime(float dt);
    
    // Adds a force to the net force experienced by this object during this
    // update cycle.
    // This will be reset to zero upon an update call.
    void applyForce(glm::vec3 force);
    
    // Handle the collision interaction with the acting object.
    // This will update both objects' forces, meaning the reaction will occur
    // during the next update call.
    void collideWith(Object &acting);
    
    // Accessor methods
    float getMass() { return m_Mass; };
    float getRadius() { return m_Radius; };
    glm::vec3 getPosition() { return m_Position; };
    glm::vec3 getVelocity() { return m_Position - m_LastPosition; };
    glm::vec3 getForce() { return m_Force; };
    glm::vec3 getAcceleration() { return m_Force / m_Mass; };
private:
    Model m_Model;
    float m_Radius; // The maximum radius of the model (bounding sphere)
    float m_Mass;
    glm::vec3 m_Position;
    glm::vec3 m_LastPosition;
    glm::vec3 m_Force;
    
    void setupModelPosition();
    
    void updateModel(float dt);
};

}

#endif