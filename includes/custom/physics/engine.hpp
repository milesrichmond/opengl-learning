#ifndef PHYSENGINE_HPP
#define PHYSENGINE_HPP

#include <custom/physics/object.hpp>
#include <nlohmann/json.hpp>

namespace Phys {

class Engine {
public:
  void draw(Shader &shader, float dt);

  void update(float dt);

  void addObject(Object &obj);

  void loadObjectsFromJSON(const std::string &path);

  void loadObjectsFromJSON(nlohmann::json data);

  void clearObjects();

private:
  std::vector<Object> objects;
};

} // namespace Phys

#endif
