#include "OpenGlShader.hpp"
#include <fstream>
#include <sstream>

OpenGlShader::OpenGlShader(const std::string& filePath) {
    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    ShaderType type = ShaderType::VERTEX;
    std::ifstream file(filePath);
    std::string line;
    std::stringstream stringStream[2];
    while (getline(file, line)) {
        if (line.find("# vertex") != std::string::npos) {
            type = ShaderType::VERTEX;

        } else if (line.find("# fragment") != std::string::npos) {
            type = ShaderType::FRAGMENT;

        } else {
            stringStream[(int) type] << line << '\n';
        }
    }
    file.close();
    shaderId = createShaderProgram(stringStream[0].str(), stringStream[1].str());
}

OpenGlShader::~OpenGlShader() {
    glDeleteProgram(shaderId);
    shaderId = NULL;
}

void OpenGlShader::useShader() {
    glUseProgram(shaderId);
}

void OpenGlShader::setInt(const std::string& name, int value) {
    glUniform1i(glGetUniformLocation(shaderId, name.c_str()), value);
}

void OpenGlShader::setVec3(const std::string& name, glm::vec3 vec3) {
    glUniform3fv(glGetUniformLocation(shaderId, name.c_str()), 1, &vec3[0]);
}

void OpenGlShader::setMat4(const std::string& name, const glm::mat4& mat) {
    glUniformMatrix4fv(glGetUniformLocation(shaderId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

GLuint OpenGlShader::createShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    if (vertexShader == 0 || fragmentShader == 0) {
        return 0;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return hasLinkErrors(program) ? 0 : program;
}

GLuint OpenGlShader::compileShader(GLuint type, const std::string& source) {
    GLuint id = glCreateShader(type);
    const GLchar* src = source.c_str();
    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);
    return hasCompileErrors(id) ? 0 : id;
}

bool OpenGlShader::hasCompileErrors(GLuint shaderId) {
    GLint compileSuccess;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileSuccess);
    if (compileSuccess == GL_FALSE) {
        GLchar errorLog[1024];
        glGetShaderInfoLog(shaderId, sizeof(errorLog), NULL, errorLog);
        printf("[ERROR] Failed to compile shader:\n - %s\n", errorLog);
        glDeleteShader(shaderId);
        return true;
    }
    return false;
}

bool OpenGlShader::hasLinkErrors(GLuint programId) {
    GLint linkSuccess;
    glGetProgramiv(programId, GL_LINK_STATUS, &linkSuccess);
    if (linkSuccess == GL_FALSE) {
        GLchar errorLog[1024];
        glGetProgramInfoLog(programId, sizeof(errorLog), NULL, errorLog);
        printf("[ERROR] Failed to link shaders to program:\n - %s\n", errorLog);
        return true;
    }
    return false;
}