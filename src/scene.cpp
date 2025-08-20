#include "scene.h"

void SceneManager::update(float dt) {
    if (!currentScene.empty())
        scenes[currentScene]->update(dt);
}

void SceneManager::draw(Renderer& r) {
    if (!currentScene.empty())
        scenes[currentScene]->draw(r);
}

Scene& SceneManager::addScene(const std::string& name, std::unique_ptr<Scene> scene) {
    Scene& ref = *scene;
    scenes[name] = std::move(scene);
    return ref;
}

void SceneManager::setCurrentScene(const std::string& scene) {
    if (!currentScene.empty())
        scenes[currentScene]->unload();

    currentScene = scene;
    scenes[currentScene]->load();
}