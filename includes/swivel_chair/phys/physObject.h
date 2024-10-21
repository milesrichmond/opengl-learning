#ifndef PHYSOBJECT_H
#define PHYSOBJECT_H

#include <swivel_chair/model/model.h>
#include <swivel_chair/fileManager.h>
#include <swivel_chair/options.h>

class PhysObject {
public:
    std::string id;

    PhysObject(std::string id, float mass = 1.0f, glm::vec3 position = glm::vec3(0.0f), glm::vec3 velocity = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(0.01f)) : id(id), model(Model(Stock::Models::sphere)), acceleration(glm::vec3(0.0f)) {
        this->mass = mass;
        this->position = position;
        this->velocity = velocity;
        this->scale = scale;

        model.scale(scale);

        radius = max(max(scale.x, scale.y), scale.z); // We're dealing with spheres, so this is a pretty safe assumption for now

        setupModelPosition();
    }

    void draw(Shader &shader) {
        model.Draw(shader);
    }

    void update(float dt) {
        velocity += acceleration * dt;
        position += velocity * dt;
        updateModel(dt);
        acceleration *= 0.0f;
    }

    void accelerate(glm::vec3 &acceleration) {
        this->acceleration += acceleration;
    }

    void collide(glm::vec3 reflection) {
        velocity = reflection;
    }

    void setPosition(glm::vec3 position) {
        this->position = position;
    }

    float getMass() { return mass; }
    glm::vec3 getPosition() { return position; }
    glm::vec3 getVelocity() { return velocity; }
    glm::vec3 getAcceleration() { return acceleration; }
    float getRadius() { return radius; }

private:
    Model model;

    glm::vec3 position;
    glm::vec3 scale;
    //glm::vec3 rotation;
    float mass;
    float radius;

    glm::vec3 velocity;
    glm::vec3 acceleration;

    void setupModelPosition() {
        model.translate(position, 1.0f);
    }

    void updateModel(float dt) {
        model.translate(velocity, dt);
    }
};

#endif