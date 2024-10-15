#ifndef PHYSENGINE_H
#define PHYSENGINE_H

#include <swivel_chair/phys/physObject.h>
#include <swivel_chair/phys/physMath.h>

#include <nlohmann/json.hpp>
#include <vector>

glm::vec3 vec3FromJSON(nlohmann::json json) {
    glm::vec3 vector = glm::vec3(0.0f);
    vector.x = json[0];
    vector.y = json[1];
    vector.z = json[2];

    return vector;
}

class PhysEngine {
public:
    PhysEngine() {

    }

    void draw(Shader &shader, float dt) {
        shader.use();
        update(dt);

        for (auto& object : objects) {
            object.draw(shader);
        }
    }

    // The optimization begins
    void update(float dt) {
        // n^2, yay
        glm::vec3 acceleration;
        for (PhysObject &target : objects) {
            for(PhysObject &acting : objects) {
                if (target.id == acting.id) { continue; }

                acceleration = PhysMath::calculateAttraction(target, acting);
                target.accelerate(acceleration);

                if (PhysMath::haveCollided(target, acting)) {
                    PhysMath::collisionRedirection(target, acting);
                    //target.update(dt);
                }
                target.update(dt);
            }
        }
    }

    void addPhysObject(PhysObject obj) {
        objects.insert(objects.end(), obj);
    }

    void loadObjectsFromJSON(std::string const path) {
        std::ifstream file(path, std::ifstream::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file: " + path);
        }
        nlohmann::json data = nlohmann::json::parse(file);
        
        this->loadObjectsFromJSON(data);

        file.close();
    }

    void loadObjectsFromJSON(nlohmann::json data) {
        for (int i = 1; i <= data.size(); i++) {
            nlohmann::json object = data["Particle" + std::to_string(i)];

            int idNum = objects.size();
            std::string id = "Particle" + std::to_string(idNum);

            nlohmann::json scale = object["scale"];
            nlohmann::json position = object["position"];
            nlohmann::json velocity = object["velocity"];

            objects.insert(objects.end(), PhysObject(id, object["mass"], vec3FromJSON(position), vec3FromJSON(velocity), vec3FromJSON(scale)));
        }
    }
private:
    std::vector<PhysObject> objects;
};

#endif