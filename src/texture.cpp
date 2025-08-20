#include "texture.h"

#include <stb_image.h>

#include <stdexcept>

bool Texture::load(const char* path) {
    // (Re)create GL object
    if (id_ == 0) glGenTextures(1, &id_);
    glBindTexture(GL_TEXTURE_2D, id_);

    // Texture params
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Load pixels
    stbi_set_flip_vertically_on_load(1);  // optional, if your UVs expect top-left origin
    unsigned char* pixels = stbi_load(path, &w_, &h_, &channels_, 0);
    if (!pixels) {
        glBindTexture(GL_TEXTURE_2D, 0);
        throw std::runtime_error(std::string("stbi_load failed: ") + path);
    }

    // Pick correct format
    GLenum fmt = (channels_ == 4)   ? GL_RGBA
                 : (channels_ == 3) ? GL_RGB
                 : (channels_ == 2) ? GL_RG
                                    : GL_RED;

    glTexImage2D(GL_TEXTURE_2D, 0, fmt, w_, h_, 0, fmt, GL_UNSIGNED_BYTE, pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
    return true;
}

void Texture::destroy() {
    if (id_) {
        glDeleteTextures(1, &id_);
        id_ = 0;
        w_ = h_ = channels_ = 0;
    }
}