#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

struct Renderer;

class Scene {
   public:
    virtual ~Scene() = default;
    virtual void load() {}
    virtual void update(float dt) {}
    virtual void draw(Renderer& r) {}
    virtual void unload() {}
};

class SceneManager {
   public:
    // Singleton access
    static SceneManager& instance() {
        static SceneManager inst;
        return inst;
    }

    // Delete copy/move
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;
    SceneManager(SceneManager&&) = delete;
    SceneManager& operator=(SceneManager&&) = delete;

    // API
    void update(float dt);
    void draw(Renderer& r);

    Scene& addScene(const std::string& name, std::unique_ptr<Scene> scene);

    void setCurrentScene(const std::string& scene);

    std::string getCurrentScene() const { return currentScene; }

   private:
    SceneManager() = default;
    ~SceneManager() = default;

    std::unordered_map<std::string, std::unique_ptr<Scene>> scenes;
    std::string currentScene;
};
