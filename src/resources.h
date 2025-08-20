#pragma once

#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include <fstream>
#include <print>
#include <sstream>
#include <string>

#include "stb_image.h"

static std::string load_file(const char *path) {
    std::ifstream source(path, std::ios::in);
    std::stringstream buffer;

    if (!source) {
        std::println(stderr, "Could not open file at path {}", path);
        return "";
    }

    buffer << source.rdbuf();

    // close file and return
    source.close();
    return buffer.str();
}

// Make sure to free the returned data
static std::tuple<unsigned char *, int, int, int> *load_texture(const char *path) {
    int width, height, channels;
    unsigned char *data = stbi_load(path, &width, &height, &channels, 0);
    if (!data) {
        std::println(stderr, "Could not load texture at path {}", path);
    }

    return new std::tuple<unsigned char *, int, int, int>(data, width, height, channels);
}