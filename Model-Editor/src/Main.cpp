#include <Core/Application.hpp>
#include "Game.h"

int main(int argc, char* argv[]) {
    Application app;
    app.start("Model Editor", 1000, 800);
    app.pushScene(new Game());
    app.run();
    return 0;
}