#pragma once

struct GLFWwindow;
struct Rect;
struct Color;

#include "shader.h"
#include "texture.h"

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
    void useShader(Shader& s) {
        currentShader = s;
        s.use();
    }
    void setColor(Color c);
    void fillRect(Rect r);
    void fillTextureRect(Rect r, Texture& t);

    Shader shapeShader = Shader("shaders/shape.vert", "shaders/shape.frag");
    Shader textureShader = Shader("shaders/texture.vert", "shaders/texture.frag");

   private:
    GLFWwindow* window = nullptr;

    // GL objects (kept as plain unsigned ints to avoid GL headers here)
    unsigned int VAO = 0, VBO = 0, EBO = 0;

    // shader

    std::reference_wrapper<Shader> currentShader = shapeShader;

    // Main loop helpers
    void processInput();

    // GPU resources
    void loadBuffers();
};
