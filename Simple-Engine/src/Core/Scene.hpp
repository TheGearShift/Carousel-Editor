#pragma once

class Scene {
public:
    virtual ~Scene() = default;

    virtual void start() {}
    virtual void handleEvents() {};
    virtual void update(float deltaTime) {}
    virtual void render() {}
    virtual bool shouldRun() { return false; }
};