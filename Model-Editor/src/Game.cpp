#include "Game.h"
#include <Core/Application.hpp>
#include <Core/FileDialog.hpp>
#include <Input/Input.hpp>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>

void ImguiInit() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = NULL;
    io.LogFilename = NULL;

    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.1f, 0.1f, 0.1f, 0.9f);

    Window& window = Application::Get().getWindow();
    ImGui_ImplSDL2_InitForOpenGL(window.getNativeWindow(), window.getContext());
    ImGui_ImplOpenGL3_Init();
}

void ImguiRender(MeshEditor* editor) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Load Model")) {
                std::string filePath;
                if (FileDialog::OpenFile(filePath)) {
                    editor->loadMesh(filePath);
                }
            }
            if (ImGui::MenuItem("Save Model")) {
                std::string filePath;
                if (FileDialog::SaveFile(filePath)) {
                    editor->saveMesh(filePath);
                }
            }
            ImGui::EndMenu();
        }
    }
    ImGui::EndMainMenuBar();

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoSavedSettings;
    windowFlags |= ImGuiWindowFlags_AlwaysAutoResize;
    windowFlags |= ImGuiWindowFlags_NoFocusOnAppearing;
    windowFlags |= ImGuiWindowFlags_NoMove;
    windowFlags |= ImGuiWindowFlags_NoNav;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 position = viewport->WorkPos;
    position.x += viewport->WorkSize.x;
    ImGui::SetNextWindowPos(position, ImGuiCond_Always, { 1.0f, 0.0f });
    ImGui::SetNextWindowBgAlpha(0.75f);

    if (ImGui::Begin("Menu", NULL, windowFlags)) {
        ImGui::Text("%.1f FPS (%.3f ms)", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
        if (editor->hasSelectedVertex()) {
            ImGui::Separator();
            ImGui::Text("X-Offset");
            ImGui::SameLine();
            float offset = 1.0f / 16.0f;
            if (ImGui::Button("-##X")) {
                editor->moveSelectedVertex(glm::vec3(-offset, 0.0f, 0.0f));
            }
            ImGui::SameLine();
            if (ImGui::Button("+##X")) {
                editor->moveSelectedVertex(glm::vec3(offset, 0.0f, 0.0f));
            }
            ImGui::Text("Y-Offset");
            ImGui::SameLine();
            if (ImGui::Button("-##Y")) {
                editor->moveSelectedVertex(glm::vec3(0.0f, -offset, 0.0f));
            }
            ImGui::SameLine();
            if (ImGui::Button("+##Y")) {
                editor->moveSelectedVertex(glm::vec3(0.0f, offset, 0.0f));
            }
            ImGui::Text("Z-Offset");
            ImGui::SameLine();
            if (ImGui::Button("-##Z")) {
                editor->moveSelectedVertex(glm::vec3(0.0f, 0.0f, -offset));
            }
            ImGui::SameLine();
            if (ImGui::Button("+##Z")) {
                editor->moveSelectedVertex(glm::vec3(0.0f, 0.0f, offset));
            }
        }
    }
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

Game::~Game() {
    delete(camera, shader, editor);
}

void Game::start() {
    ImguiInit();
    Input::Init();

    glEnable(GL_CULL_FACE);

    GLenum error = glewInit();
    if (GLEW_OK != error) {
        printf("[ERROR] Failed to initialize GLEW:\n - %s\n", glewGetErrorString(error));
        exit(1);
    }
    printf("[LOG] Using GLEW %s\n", glewGetString(GLEW_VERSION));

    camera = new Camera(glm::vec3(0.0f, 0.0f, 1.5f), true);
    shader = new OpenGlShader("../../../shaders/basicShader.glsl");
    editor = new MeshEditor(camera);
}

void Game::handleEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        switch (event.type) {
            case SDL_WINDOWEVENT:
            {
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_RESIZED:
                    {
                        Window& window = Application::Get().getWindow();
                        window.setWindowSize();
                        glViewport(0, 0, window.getWidth(), window.getHeight());
                        break;
                    }
                }
                break;
            }
            case SDL_QUIT:
            {
                running = 0;
                break;
            }
            case SDL_KEYDOWN:
            {
                switch (event.key.keysym.sym) {
                    case SDLK_F1:
                    {
                        camera->orbiting = !camera->orbiting;
                        camera->toggleRelativeMouse();
                        break;
                    }
                    case SDLK_F2:
                    {
                        GLint  wireFrameMode;
                        glGetIntegerv(GL_POLYGON_MODE, &wireFrameMode);
                        glPolygonMode(GL_FRONT_AND_BACK, wireFrameMode == GL_LINE ? GL_FILL : GL_LINE);
                        if (wireFrameMode == GL_LINE) {
                            glEnable(GL_CULL_FACE);
                        } else {
                            glDisable(GL_CULL_FACE);
                        }

                        break;
                    }
                }
                break;
            }
            case SDL_MOUSEWHEEL:
            {
                if (event.wheel.y > 0) {
                    camera->offsetMultiplier -= 0.1f;
                } else if (event.wheel.y < 0) {
                    camera->offsetMultiplier += 0.1f;
                }
                break;
            }
        }
        ImGuiIO& io = ImGui::GetIO();
        if (!io.WantCaptureMouse) {
            Input::HandleEvent(event);
        }
    }
    Input::UpdateMousePosition();
}

void Game::update(float deltaTime) {
    camera->updateCamera(deltaTime);
    editor->update();
}

void Game::render() {
    glClearColor(35 / 255.0f, 82 / 255.0f, 117 / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    shader->useShader();
    shader->setMat4("projection", camera->getPerspective());
    shader->setMat4("view", camera->getView());
    shader->setMat4("model", glm::mat4(1.0f));

    editor->render();

    ImguiRender(editor);

    Application::Get().getWindow().swapBuffers();
}

bool Game::shouldRun() {
    return running;
}