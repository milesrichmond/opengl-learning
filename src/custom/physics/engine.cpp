#include <custom/physics/engine.hpp>

#include <custom/physics/math.hpp>
#include <custom/physics/object.hpp>

#include <nlohmann/json.hpp>

#include <fstream>
#include <iostream>
#include <vector>

namespace Phys {

glm::vec3 vec3FromJSON(nlohmann::json json) {
  glm::vec3 vector = glm::vec3(0.0f);
  vector.x = json[0];
  vector.y = json[1];
  vector.z = json[2];

  return vector;
}

void Engine::draw(Shader &shader, float dt) {
  update(dt);
  for (Object &object : objects) {
    object.draw(shader);
  }
}

void Engine::update(float dt) {}

void Engine::addObject(Object &obj) { objects.push_back(obj); }

void Engine::loadObjectsFromJSON(const std::string &path) {
  std::ifstream file(path, std::ifstream::binary);
  if (!file.is_open()) {
    std::cout << "Error reading file: " << path << std::endl;
    return;
  }

  nlohmann::json data = nlohmann::json::parse(file);
  this->loadObjectsFromJSON(data);
  file.close();
}

void Engine::loadObjectsFromJSON(nlohmann::json data) {
  for (int i = 1; i <= data.size(); i++) {
    nlohmann::json object = data["Particle" + std::to_string(i)];

    int idNum = objects.size();
    std::string id = "Particle" + std::to_string(idNum);

    glm::vec3 scale = vec3FromJSON(object["scale"]);
    glm::vec3 position = vec3FromJSON(object["position"]);

    objects.emplace_back(id, object["mass"], position, scale);
  }
}

void Engine::clearObjects() { objects.clear(); }
} // namespace Phys
