#pragma once

#include <Core/Scene.hpp>
#include <Core/Window.hpp>
#include <Renderer/OpenGlShader.hpp>
#include "Camera.hpp"
#include "MeshEditor.hpp"

class Game : public Scene {
public:
    ~Game() override;
    void start() override;
    void handleEvents() override;
    void update(float deltaTime) override;
    void render() override;
    bool shouldRun() override;

private:
    Camera* camera;
    OpenGlShader* shader;
    MeshEditor* editor;

    bool running = 1;
};