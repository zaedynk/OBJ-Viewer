// Shader.cpp

#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    // Load and compile the vertex and fragment shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);
    std::stringstream vShaderStream, fShaderStream;

    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    vShaderFile.close();
    fShaderFile.close();

    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    glShaderSource(vertexShader, 1, &vShaderCode, nullptr);
    glShaderSource(fragmentShader, 1, &fShaderCode, nullptr);

    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, "VERTEX");

    glCompileShader(fragmentShader);
    checkCompileErrors(fragmentShader, "FRAGMENT");

    // Link the vertex and fragment shader into a shader program
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    // Delete the vertex and fragment shaders, as they're no longer needed
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(ID);
}

// Function to set int uniform
void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

// Function to activate the shader
void Shader::use() {
    glUseProgram(ID);
}

// Function to set a mat4 uniform
void Shader::setMat4(const std::string& name, const glm::mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}


void Shader::checkCompileErrors(GLuint shader, const std::string& type) const {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}
