#pragma once

#include <Core/Window.hpp>
#include <Input/Input.hpp>
#include <glm/glm.hpp>

const glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

class Camera {
public:
    Camera();
    Camera(glm::vec3 position = glm::vec3(0.0f), bool orbiting = false);
    void toggleRelativeMouse();
    void centerMousePosition();
    void updateCamera(float deltaTime);
    glm::vec3 getPosition();
    glm::vec3 getDirection();
    glm::mat4 getPerspective();
    glm::mat4 getView();

    bool orbiting;
    float offsetMultiplier = 1.0f;

private:
    void updateDirection();
    glm::ivec2 getOrbitalOffset();
    glm::ivec2 getFpsOffset();

    glm::vec3 position;
    glm::vec3 direction;

    float sensitivity = 0.2f;
    float yaw = 270.0f;
    float pitch = 0.0f;
    MousePosition previousMousePosition;
};