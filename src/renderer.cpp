// renderer.cpp
#include "renderer.h"

#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include <cmath>
#include <cstdlib>
#include <print>
#include <string>

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"
#include "scene.h"
#include "shader.h"
#include "util.h"

// ----------------------------- utils ---------------------------------

namespace {
void framebuffer_size_callback(GLFWwindow*, int w, int h) {
    glViewport(0, 0, w, h);
}

// simple, frame-rate independent EMA FPS smoother
struct FpsSmoother {
    double tau = 0.3;    // seconds
    double value = 0.0;  // smoothed fps
    void push(double dt, double fpsInstant) {
        const double a = 1.0 - std::exp(-dt / tau);
        value += a * (fpsInstant - value);
    }
};
}  // namespace

// ----------------------------- sample scene ---------------------------

class GameScene : public Scene {
   public:
    void load() override {}
    void unload() override {}
    void update(float) override {}
    void draw(Renderer& r) override {
        glClearColor(0.573f, 0.953f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // background
        r.useShader(r.textureShader);
        r.setColor({1, 1, 1, 1});
        r.fillTextureRect({0, 0, 800 * 2, 600 * 2}, texture);

        // foreground
        r.useShader(r.shapeShader);
        r.setColor({1, 0, 0, 1});
        r.fillRect({10, 12, 40, 300});

        // reset
        r.setColor({1, 1, 1, 1});
    }

   private:
    Texture texture = Texture("textures/texture_01.png");
};

// ----------------------------- sample scene2 ---------------------------

class GameScene2 : public Scene {
   public:
    void load() override {}
    void unload() override {}
    void update(float) override {}
    void draw(Renderer& r) override {
        glClearColor(0.573f, 0.953f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // background
        r.setColor({1, 0, 1, 1});
        r.fillRect({10, 12, 40, 300});
    }
};

// ----------------------------- renderer --------------------------------

Renderer::Renderer(GLFWwindow* window) : window(window) {
    // initial viewport to framebuffer size
    int fbw = 0, fbh = 0;
    glfwGetFramebufferSize(window, &fbw, &fbh);
    glViewport(0, 0, fbw, fbh);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // shader
    currentShader.get().use();
}

Renderer::~Renderer() {
    // ImGui shutdown first
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    glfwTerminate();
}

void Renderer::run() {
    loadBuffers();

    // scenes
    SceneManager::instance().addScene("game", std::make_unique<GameScene>(/*args*/));
    SceneManager::instance().addScene("game2", std::make_unique<GameScene2>(/*args*/));

    SceneManager::instance().setCurrentScene("game");

    // timing
    double lastTime = glfwGetTime();
    FpsSmoother fps;
    float dt = 0.f;

    while (!glfwWindowShouldClose(window)) {
        const double now = glfwGetTime();
        dt = static_cast<float>(now - lastTime);
        lastTime = now;
        if (dt > 0.25f) dt = 0.25f;  // clamp to avoid huge spikes (pause, etc.)
        fps.push(dt, 1.0 / dt);

        processInput();
        beginFrame();

        // game update/draw
        SceneManager::instance().update(dt);
        SceneManager::instance().draw(*this);

        // debug ui
        ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_NoResize);
        ImGui::Text("Scene: %s", SceneManager::instance().getCurrentScene().c_str());
        ImGui::Separator();
        ImGui::Text("FPS: %.1f", fps.value);
        ImGui::Text("dt:  %.3f ms", dt * 1000.0f);
        ImGui::Separator();
        ImGui::End();

        endFrame();
    }
}

void Renderer::beginFrame() {
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Renderer::endFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Renderer::processInput() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
        SceneManager::instance().setCurrentScene("game");
    if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
        SceneManager::instance().setCurrentScene("game2");
}

void Renderer::loadBuffers() {
    // Unit quad (0..1) so uPos/uScale can place/size in pixel space
    // x, y, u, v
    static constexpr float vertices[] = {
        // pos.x, pos.y,   u, v
        1.f, 1.f, 1.f, 1.f,  // RT
        1.f, 0.f, 1.f, 0.f,  // RB
        0.f, 0.f, 0.f, 0.f,  // LB
        0.f, 1.f, 0.f, 1.f   // LT
    };
    static constexpr unsigned int indices[] = {
        0, 1, 3,  // first tri
        1, 2, 3   // second tri
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // pos
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // uv
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Renderer::setColor(Color c) {
    glUniform4f(currentShader.get().getUniform("uColor"), c.r, c.g, c.b, c.a);
}

void Renderer::fillRect(Rect r) {
    int winW = 0, winH = 0;
    glfwGetWindowSize(window, &winW, &winH);

    // pixel -> NDC via uniforms (shader handles it)
    const float widthGL = (winW > 0) ? (r.w / winW) : 0.f;
    const float heightGL = (winH > 0) ? (r.h / winH) : 0.f;
    const float xGL = -1.0f + (r.x / winW) * 2.0f;
    const float yGL = 1.0f - (r.y / winH) * 2.0f - heightGL;

    glUniform2f(currentShader.get().getUniform("uPos"), xGL, yGL);
    glUniform2f(currentShader.get().getUniform("uScale"), widthGL, heightGL);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Renderer::fillTextureRect(Rect r, Texture& t) {
    int winW = 0, winH = 0;
    glfwGetWindowSize(window, &winW, &winH);

    // pixel -> NDC via uniforms (shader handles it)
    const float widthGL = (winW > 0) ? (r.w / winW) : 0.f;
    const float heightGL = (winH > 0) ? (r.h / winH) : 0.f;
    const float xGL = -1.0f + (r.x / winW) * 2.0f;
    const float yGL = 1.0f - (r.y / winH) * 2.0f - heightGL;

    glUniform2f(currentShader.get().getUniform("uPos"), xGL, yGL);
    glUniform2f(currentShader.get().getUniform("uScale"), widthGL, heightGL);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, t.id());
    glUniform1i(currentShader.get().getUniform("uTex"), 0);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
