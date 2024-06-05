#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

class OpenGlShader {
public:
    OpenGlShader(const std::string& filePath);
    ~OpenGlShader();
    void useShader();
    void setInt(const std::string& name, int value);
    void setVec3(const std::string& name, glm::vec3 vec3);
    void setMat4(const std::string& name, const glm::mat4& mat);

private:
    GLuint createShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
    GLuint compileShader(GLuint type, const std::string& source);
    bool hasCompileErrors(GLuint shaderId);
    bool hasLinkErrors(GLuint programId);

    GLuint shaderId;
    int width, height, channels;
};