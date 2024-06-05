#include "Camera.hpp"
#include <Core/Application.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() {
    direction = glm::vec3(0.0f, 0.0f, -1.0f);
    previousMousePosition = { 0, 0 };
}

Camera::Camera(glm::vec3 position, bool orbiting) : orbiting(orbiting) {
    this->position = position;
    direction = glm::vec3(0.0f, 0.0f, -1.0f);
    previousMousePosition = { 0, 0 };

    if (orbiting) {
        offsetMultiplier = glm::length(position);
    } else {
        toggleRelativeMouse();
    }
}

void Camera::toggleRelativeMouse() {
    SDL_bool state = SDL_GetRelativeMouseMode();
    SDL_SetRelativeMouseMode((SDL_bool) !state);
    centerMousePosition();
}

void Camera::centerMousePosition() {
    Window& window = Application::Get().getWindow();
    SDL_WarpMouseInWindow(
        window.getNativeWindow(),
        (int) (window.getWidth() / 2),
        (int) (window.getHeight() / 2)
    );
}

void Camera::updateCamera(float deltaTime) {
    if (orbiting) {
        if (Input::IsRightMouseDown()) {
            updateDirection();
        }
        previousMousePosition = Input::GetMousePosition();
        offsetMultiplier = offsetMultiplier < 0.0f ? 0.0f : offsetMultiplier;
        glm::vec3 origin = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 offset = glm::normalize(direction) * offsetMultiplier;
        position = origin - offset;
    } else if (!orbiting) {
        if (SDL_GetRelativeMouseMode()) {
            updateDirection();
            centerMousePosition();
        }
        glm::vec3 cameraRight = glm::normalize(glm::cross(cameraUp, direction));
        glm::vec2 moveInput = glm::vec2(
            Input::IsKeyDown(KeyCode::D) - Input::IsKeyDown(KeyCode::A),
            Input::IsKeyDown(KeyCode::W) - Input::IsKeyDown(KeyCode::S)
        );
        int zInput = Input::IsKeyDown(KeyCode::SPACE) - Input::IsKeyDown(KeyCode::ALT);

        if (glm::length(moveInput) != 0.0f) {
            moveInput = glm::normalize(moveInput) * deltaTime;
            position += glm::normalize(glm::cross(cameraRight, cameraUp)) * moveInput.y;
            position += glm::normalize(glm::cross(direction, cameraUp)) * moveInput.x;
        }

        if (zInput != 0) {
            position += cameraUp * (float) zInput * deltaTime;
        }
    }
}

void Camera::updateDirection() {
    glm::ivec2 offset;

    if (orbiting) {
        offset = getOrbitalOffset();
    } else {
        offset = getFpsOffset();
    }

    yaw += (float) offset.x * sensitivity;
    pitch += (float) offset.y * sensitivity;
    yaw = (float) std::fmod(yaw, 360);
    yaw = (yaw < 0.0f) ? yaw + 360 : yaw;
    pitch = (pitch < -89.9f) ? -89.9f : (pitch > 89.9f) ? 89.9f : pitch;

    direction = glm::vec3(0);
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
}

glm::ivec2 Camera::getOrbitalOffset() {
    MousePosition mousePosition = Input::GetMousePosition();
    int xOffset = mousePosition.x - previousMousePosition.x;
    int yOffset = previousMousePosition.y - mousePosition.y;
    return glm::ivec2(xOffset, yOffset);
}

glm::ivec2 Camera::getFpsOffset() {
    MousePosition mousePosition = Input::GetMousePosition();

    Window& window = Application::Get().getWindow();
    int xOffset = mousePosition.x - (window.getWidth() / 2);
    int yOffset = (window.getHeight() / 2) - mousePosition.y;
    return glm::ivec2(xOffset, yOffset);
}

glm::vec3 Camera::getPosition() { return position; }

glm::vec3 Camera::getDirection() { return direction; }

glm::mat4 Camera::getPerspective() {
    Window& window = Application::Get().getWindow();
    return glm::perspective(glm::radians(70.0f), (float) window.getWidth() / (float) window.getHeight(), 0.1f, 100.0f);
}

glm::mat4 Camera::getView() { return glm::lookAt(position, position + direction, cameraUp); }