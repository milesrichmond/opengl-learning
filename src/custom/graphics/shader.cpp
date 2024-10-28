#include <custom/graphics/shader.hpp>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

void checkCompilation(GLuint shader, int type) {
  GLint success;
  GLchar infoLog[1024];

  if (type == 0) {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);
      std::cout << "Shader compilation error:\n" << infoLog << std::endl;
    }
  } else {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shader, 1024, NULL, infoLog);
      std::cout << "Shader program linking failed:\n" << infoLog << std::endl;
    }
  }
}

// A blank constructor is required due to glCurrentContext conflictions
Shader::Shader() {
  ID = -1;
  name = "Undefined Shader";
}

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
  std::string vertexCodeString, fragmentCodeString, geometryCodeString;
  std::ifstream vShaderFile, fShaderFile, gShaderFile;

  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);
    std::stringstream vShaderStream, fShaderStream;
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    vShaderFile.close();
    fShaderFile.close();

    vertexCodeString = vShaderStream.str();
    fragmentCodeString = fShaderStream.str();
  } catch (std::ifstream::failure &e) {
    std::cout << "Shader file not read successfully:\n"
              << e.what() << std::endl;
  }

  const char *vertexCode = vertexCodeString.c_str();
  const char *fragmentCode = fragmentCodeString.c_str();

  // Attach code & compile individual shaders
  unsigned int vertex, fragment;
  vertex = glCreateShader(GL_VERTEX_SHADER);
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(vertex, 1, &vertexCode, NULL);
  glShaderSource(fragment, 1, &fragmentCode, NULL);
  glCompileShader(vertex);
  checkCompilation(vertex, 0);
  glCompileShader(fragment);
  checkCompilation(fragment, 0);

  // Link into Shader Program
  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);
  checkCompilation(ID, 1);

  // Delete excess
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void Shader::use() { glUseProgram(ID); }
