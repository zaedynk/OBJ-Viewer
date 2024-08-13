#pragma once
// Shader.cpp
// Shader.h
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#pragma once



class Shader {
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    void setMat4(const std::string& name, const glm::mat4& mat) const;

    // Function to set int uniform
    void setInt(const std::string& name, int value) const;

    // Function to activate the shader
    void use();

private:

    GLuint ID;
    void checkCompileErrors(GLuint shader, const std::string& type) const;
};
