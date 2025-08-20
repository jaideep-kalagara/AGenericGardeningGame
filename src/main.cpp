#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include <exception>
#include <print>

#include "renderer.h"

int main() {
    if (!glfwInit()) {
        std::println(stderr, "Failed to initialize GLFW");
        std::exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "A Generic Gardening Game", nullptr, nullptr);
    if (!window) {
        std::println(stderr, "Failed to create GLFW window");
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);  // vsync

    // glad
    if (!gladLoadGL(glfwGetProcAddress)) {
        std::println(stderr, "Failed to initialize GLAD");
        glfwDestroyWindow(window);
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    // create renderer
    try {
        Renderer renderer(window);
        renderer.run();
        return EXIT_SUCCESS;
    } catch (const std::exception& e) {
        std::println(stderr, "Fatal error: %s\n", e.what());
        return EXIT_FAILURE;
    } catch (...) {
        std::println(stderr, "Fatal error: unknown exception\n");
        return EXIT_FAILURE;
    }
}
