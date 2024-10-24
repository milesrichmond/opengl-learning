#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    unsigned int ID;
    std::string name = "bob";

    // Dummy constructor to appease the computer gods?
    Shader() {
        ID = 0;
    }

    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr) {
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

            if (geometryPath != nullptr) {
                gShaderFile.open(geometryPath);
                std::stringstream gShaderStream;
                gShaderStream << gShaderFile.rdbuf();
                gShaderFile.close();
                geometryCodeString = gShaderStream.str();
            } 
        } catch (std::ifstream::failure& e) {
            std::cout << "Shader file not read successfully:\n" << e.what() << std::endl; 
        }

        const char* vertexCode = vertexCodeString.c_str();
        const char* fragmentCode = fragmentCodeString.c_str();

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

        unsigned int geometry;
        if (geometryPath != nullptr) {
            const char * geometryCode = geometryCodeString.c_str();
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &geometryCode, NULL);
            glCompileShader(geometry);
            checkCompilation(geometry, 0);
        }

        // Link into Shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        if (geometryPath != nullptr) {
            glAttachShader(ID, geometry);
        }
        glLinkProgram(ID);
        checkCompilation(ID, 1);

        // Delete excess
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if (geometryPath != nullptr) {
            glDeleteShader(geometry);
        }
    }

    void use() {
        glUseProgram(ID);
    }

    // The following set__ methods are taken directly from:
    // https://github.com/JoeyDeVries/LearnOpenGL/blob/master/includes/learnopengl/shader.h 
    void setBool(const std::string &name, bool value) const {         
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const { 
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const { 
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string &name, const glm::vec2 &value) const { 
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec2(const std::string &name, float x, float y) const { 
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); 
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string &name, const glm::vec3 &value) const { 
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec3(const std::string &name, float x, float y, float z) const { 
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string &name, const glm::vec4 &value) const { 
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec4(const std::string &name, float x, float y, float z, float w) { 
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); 
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string &name, const glm::mat2 &mat) const {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string &name, const glm::mat3 &mat) const {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string &name, const glm::mat4 &mat) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

private:
    void checkCompilation(GLuint shader, int type) {
        GLint success;
        GLchar infoLog[1024];

        if(type == 0) {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "Shader compilation error:\n" << infoLog << std::endl;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if(!success) {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "Shader program linking failed:\n" << infoLog << std::endl;
            }
        }
    }
};

#endif