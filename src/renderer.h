#pragma once

#include <optional>

struct GLFWwindow;

#include "shader.h"

class Renderer {
   public:
    Renderer(GLFWwindow* window);
    ~Renderer();

    // Non-copyable / non-movable (GL context + window should be unique)
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;
    Renderer& operator=(Renderer&&) = delete;

    void run();

    // Window accessors
    GLFWwindow* windowHandle() const noexcept { return window; }
    void getWindowSizePx(int& w, int& h) const noexcept;

    // Frame boundaries (keeps ImGui/swap/poll out of scenes)
    void beginFrame();
    void endFrame();

    // Draw helpers
    void setColor(float r, float g, float b, float a = 1.0f);
    void fillRect(float x, float y, float w, float h);

   private:
    GLFWwindow* window = nullptr;

    // GL objects (kept as plain unsigned ints to avoid GL headers here)
    unsigned int VAO = 0, VBO = 0, EBO = 0;

    std::optional<Shader> shader;

    // Main loop helpers
    void processInput();

    // GPU resources
    void loadBuffers();
};
