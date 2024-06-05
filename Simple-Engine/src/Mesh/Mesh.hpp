#pragma once

#include <GL/glew.h>

class Mesh {
public:
    virtual ~Mesh();
    virtual void render();

protected:
    GLuint vertexArrayObject;
    GLuint indexCount;
};