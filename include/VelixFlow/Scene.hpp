#ifndef SCENE_HPP
#define SCENE_HPP

#include "Drawable.hpp"
#include "GameObject.hpp"
#include "Skybox.hpp"
#include "AssetsCache.hpp"
#include "Light.hpp"
#include "UIElement.hpp"

class Scene
{
public:
    void update(float deltaTime);

    void addUIElement(const std::shared_ptr<elix::ui::UIElement>& uiElement);

    const std::vector<std::shared_ptr<elix::ui::UIElement>> getUIElements() const;

    void setSkybox(const std::shared_ptr<elix::Skybox>& skybox);

    std::shared_ptr<elix::Skybox> getSkybox() const;

    const std::vector<std::shared_ptr<lighting::Light>>& getLights() const;

    void addLight(const std::shared_ptr<lighting::Light>& light);

    void addGameObject(const std::shared_ptr<GameObject>& gameObject);

    void addDrawable(const std::shared_ptr<Drawable>& drawable);

    void setGameObjects(const std::vector<std::shared_ptr<GameObject>>& gameObjects);

    bool deleteGameObject(GameObject* gameObject);

    const std::vector<std::shared_ptr<GameObject>>& getGameObjects();

    const std::vector<std::shared_ptr<Drawable>>& getDrawables();

    void saveSceneToFile(const std::string& filePath);
    void loadSceneFromFile(const std::string& filePath, elix::AssetsCache& cache);
private:
    std::vector<std::shared_ptr<elix::ui::UIElement>> m_uiElements;
    std::vector<std::shared_ptr<lighting::Light>> m_lights;   
    std::vector<std::shared_ptr<GameObject>> m_objects;
    std::vector<std::shared_ptr<Drawable>> m_drawables;
    std::shared_ptr<elix::Skybox> m_skybox;
};

#endif //SCENE_HPP
