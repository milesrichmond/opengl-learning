#ifndef TEXTRENDERER_HPP
#define TEXTRENDERER_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <custom/fileManager.hpp>
#include <custom/graphics/shader.hpp>
#include <custom/state/gameOptions.hpp>

#include <map>

struct Character {
  unsigned int TextureID;
  glm::ivec2 size;
  glm::ivec2 bearing;
  unsigned int advance;
};

class TextRenderer {
public:
  bool valid;

  TextRenderer();
  TextRenderer(const char *vertexPath, const char *fragmentSource,
               const char *fontSource, int SCR_WIDTH, int SCR_HEIGHT);

  void RenderText(std::string text, float x, float y, float scale,
                  glm::vec3 color, int SCR_WIDTH, int SCR_HEIGHT);

  void resize(int SCR_WIDTH, int SCR_HEIGHT);

private:
  Shader shader;
  std::map<GLchar, Character> characters;

  unsigned int VAO, VBO;
};

#endif
