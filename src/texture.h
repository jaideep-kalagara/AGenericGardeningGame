#pragma once

#include <glad/gl.h>

#include <string>
#include <utility>

class Texture {
   public:
    Texture() = default;
    explicit Texture(const char* path) { load(path); }
    explicit Texture(const std::string& path) { load(path.c_str()); }

    ~Texture() { destroy(); }

    // non-copyable, movable
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& other) noexcept { *this = std::move(other); }
    Texture& operator=(Texture&& other) noexcept {
        if (this != &other) {
            destroy();
            id_ = other.id_;
            other.id_ = 0;
            w_ = other.w_;
            h_ = other.h_;
            channels_ = other.channels_;
        }
        return *this;
    }

    bool load(const char* path);
    void destroy();

    GLuint id() const { return id_; }
    int width() const { return w_; }
    int height() const { return h_; }
    int channels() const { return channels_; }
    explicit operator bool() const { return id_ != 0; }

   private:
    GLuint id_ = 0;
    int w_ = 0, h_ = 0, channels_ = 0;
};