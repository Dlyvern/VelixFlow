#ifndef SCENE_HPP
#define SCENE_HPP

#include "VelixFlow/GameObject.hpp"
// #include "VelixFlow/Skybox.hpp"
#include "VelixFlow/AssetsCache.hpp"
#include "VelixFlow/Light.hpp"
#include "VelixFlow/UI/UIWidget.hpp"
#include "VelixFlow/DefaultMacros.hpp"

ELIX_NAMESPACE_BEGIN

class Scene
{
public:
    void update(float deltaTime);

    void addUIElement(const std::shared_ptr<ui::UIWidget>& uiWidget);

    const std::vector<std::shared_ptr<ui::UIWidget>> getUIElements() const;

    void deleteUIElement(const std::shared_ptr<ui::UIWidget>& uiWidget);

    void deleteAllUIElements();

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
    std::vector<std::shared_ptr<ui::UIWidget>> m_uiElements;
    std::vector<std::shared_ptr<lighting::Light>> m_lights;   
    std::vector<std::shared_ptr<GameObject>> m_objects;
    // std::shared_ptr<Skybox> m_skybox;
};

ELIX_NAMESPACE_END

#endif //SCENE_HPP
