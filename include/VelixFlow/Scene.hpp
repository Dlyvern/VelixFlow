#ifndef SCENE_HPP
#define SCENE_HPP

#include "VelixFlow/GameObject.hpp"
// #include "VelixFlow/Skybox.hpp"
#include "VelixFlow/AssetsCache.hpp"
#include "VelixFlow/Light.hpp"
#include "VelixFlow/UI/UIElement.hpp"
#include "VelixFlow/DefaultMacros.hpp"

ELIX_NAMESPACE_BEGIN

class Scene
{
public:
    void update(float deltaTime);

    void addUIElement(const std::shared_ptr<ui::UIElement>& uiElement);

    const std::vector<std::shared_ptr<ui::UIElement>> getUIElements() const;

    // void setSkybox(const std::shared_ptr<Skybox>& skybox);

    // std::shared_ptr<Skybox> getSkybox() const;

    const std::vector<std::shared_ptr<lighting::Light>>& getLights() const;

    void addLight(const std::shared_ptr<lighting::Light>& light);

    void addGameObject(const std::shared_ptr<GameObject>& gameObject);

    void setGameObjects(const std::vector<std::shared_ptr<GameObject>>& gameObjects);

    bool deleteGameObject(GameObject* gameObject);

    const std::vector<std::shared_ptr<GameObject>>& getGameObjects();

    void saveSceneToFile(const std::string& filePath);
    void loadSceneFromFile(const std::string& filePath, AssetsCache& cache);
private:
    std::vector<std::shared_ptr<ui::UIElement>> m_uiElements;
    std::vector<std::shared_ptr<lighting::Light>> m_lights;   
    std::vector<std::shared_ptr<GameObject>> m_objects;
    // std::shared_ptr<Skybox> m_skybox;
};

ELIX_NAMESPACE_END

#endif //SCENE_HPP
