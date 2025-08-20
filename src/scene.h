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
    void update(float dt) {
        if (!currentScene.empty())
            scenes[currentScene]->update(dt);
    }

    void draw(Renderer& r) {
        if (!currentScene.empty())
            scenes[currentScene]->draw(r);
    }

    template <typename T, typename... Args>
    T& addScene(const std::string& name, Args&&... args) {
        // Emplace a unique_ptr<T> into the map
        auto scene = std::make_unique<T>(std::forward<Args>(args)...);
        T& ref = *scene;
        scenes[name] = std::move(scene);
        return ref;
    }

    void setCurrentScene(const std::string& scene) {
        if (!currentScene.empty())
            scenes[currentScene]->unload();

        currentScene = scene;
        scenes[currentScene]->load();
    }

    std::string getCurrentScene() const { return currentScene; }

   private:
    SceneManager() = default;
    ~SceneManager() = default;

    std::unordered_map<std::string, std::unique_ptr<Scene>> scenes;
    std::string currentScene;
};
