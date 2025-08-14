#include "VelixFlow/Scene.hpp"
#include <fstream>
#include <algorithm>
#include <json/json.hpp>
#include "VelixFlow/Components/AnimatorComponent.hpp"
#include "VelixFlow/Light.hpp"
#include "VelixFlow/Components/LightComponent.hpp"
#include "VelixFlow/Logger.hpp"
#include "VelixFlow/Components/MeshComponent.hpp"
#include "VelixFlow/Components/RigidbodyComponent.hpp"
#include "VelixFlow/Components/ScriptComponent.hpp"
#include "VelixFlow/Scripting/ScriptSystem.hpp"
#include "VelixFlow/Components/TransformComponent.hpp"

ELIX_NAMESPACE_BEGIN

void Scene::update(float deltaTime)
{
    for (const auto& object : m_objects)
        object->update(deltaTime);

    for(const auto& uiElement : m_uiElements)
        uiElement->update(deltaTime);
}

void Scene::addUIElement(const std::shared_ptr<ui::UIWidget>& uiElement)
{
    m_uiElements.push_back(uiElement);
}

const std::vector<std::shared_ptr<ui::UIWidget>> Scene::getUIElements() const
{
    return m_uiElements;
}


const std::vector<std::shared_ptr<lighting::Light>>& Scene::getLights() const
{
    return m_lights;
}

void Scene::addLight(const std::shared_ptr<lighting::Light>& light)
{
    m_lights.push_back(light);
}

// void Scene::setSkybox(const std::shared_ptr<Skybox> &skybox)
// {
//     m_skybox = skybox;
// }

// std::shared_ptr<Skybox> Scene::getSkybox() const
// {
//     return m_skybox;
// }

void Scene::addGameObject(const std::shared_ptr<GameObject>& gameObject)
{
    m_objects.push_back(gameObject);
}

void Scene::setGameObjects(const std::vector<std::shared_ptr<GameObject>> &gameObjects)
{
    m_objects = gameObjects;
}

void Scene::deleteUIElement(const std::shared_ptr<ui::UIWidget>& uiWidget)
{
    if(!uiWidget)
        return;

    
}

void Scene::deleteAllUIElements()
{
    m_uiElements.clear();
}

bool Scene::deleteGameObject(GameObject *gameObject)
{
    if (!gameObject)
        return false;

    const auto it = std::find_if(m_objects.begin(), m_objects.end(), [&gameObject](const std::shared_ptr<GameObject>& gO)
    {
        return gO->getName() == gameObject->getName();
    });

    if (it == m_objects.end())
        return false;

    it->get()->destroy();
    m_objects.erase(it);
    
    return true;
}

const std::vector<std::shared_ptr<GameObject>>& Scene::getGameObjects()
{
    return m_objects;
}


void Scene::saveSceneToFile(const std::string &filePath)
{
    nlohmann::json json;

    json["name"] = std::filesystem::path(filePath).filename().string();

    // if (getSkybox())
    //     json["skybox"] = getSkybox()->getAssetPath();

    const auto& objects = getGameObjects();

    for (const auto& object : objects)
    {
        if (object->getName() == "player")
            continue;

        nlohmann::json objectJson;

        objectJson["name"] = object->getName();

        if(const auto& transformation = object->getComponent<components::TransformComponent>())
        {
            objectJson["position"] = {transformation->getPosition().x, transformation->getPosition().y, transformation->getPosition().z};
            objectJson["scale"] = {transformation->getScale().x, transformation->getScale().y, transformation->getScale().z};
            objectJson["rotation"] = {transformation->getRotation().x, transformation->getRotation().y, transformation->getRotation().z};
        }


        if (object->hasComponent<components::MeshComponent>())
        {
            if (auto model = object->getComponent<components::MeshComponent>()->getModel())
            {
                objectJson["model"] = model->getName();

                nlohmann::json materialJson;

                for (int index = 0; index < model->getNumMeshes(); index++)
                {
                    Material* material;

                    if(auto mat = object->getComponent<components::MeshComponent>()->getMaterialOverride(index))
                        material = mat;

                    if (material)
                        materialJson[std::to_string(index)] = material->getName();
                }

                objectJson["materials"] = materialJson;
            }
        }

        if (object->hasComponent<components::LightComponent>())
        {
            nlohmann::json lightJson;

            auto light = object->getComponent<components::LightComponent>()->getLight();

            lightJson["type"] = "LightComponent";
            lightJson["lightType"] = static_cast<int>(light->type);
            lightJson["direction"] = {light->direction.x, light->direction.y, light->direction.z};
            lightJson["color"] = {light->color.r, light->color.g, light->color.b};
            lightJson["strength"] = light->strength;
            lightJson["position"] = {light->position.x, light->position.y, light->position.z};
            lightJson["radius"] = light->radius;

            objectJson["components"].push_back(lightJson);
        }
        if (object->hasComponent<components::AnimatorComponent>())
        {
            nlohmann::json animatorJson;

            animatorJson["type"] = "AnimatorComponent";

            objectJson["components"].push_back(animatorJson);
        }
        if (object->hasComponent<components::ScriptComponent>())
        {
            nlohmann::json scriptArray = nlohmann::json::array();

            for (const auto& [name, _] : object->getComponent<components::ScriptComponent>()->getScripts()) {
                scriptArray.push_back(name);
            }

            nlohmann::json scriptJson;
            scriptJson["type"] = "ScriptsComponent";
            scriptJson["scripts"] = scriptArray;

            objectJson["components"].push_back(scriptJson);
        }

        json["game_objects"].push_back(objectJson);
    }

    auto uiElements = getUIElements();

    for(const auto& uiElement : uiElements)
    {
        nlohmann::json uiObjectJson;

        uiObjectJson["name"] = uiElement->getName();

        uiObjectJson["position"] = {uiElement->getPosition().x, uiElement->getPosition().y};
        uiObjectJson["size"] = {uiElement->getSize().x, uiElement->getSize().y};
        uiObjectJson["color"] = {uiElement->getColor().x, uiElement->getColor().y, uiElement->getColor().z, uiElement->getColor().w};
        uiObjectJson["is_visible"] = uiElement->isVisible();
        uiObjectJson["alpha"] = uiElement->getAlpha();

        if(auto texture = uiElement->getTexture())
        {
            // auto glTexture = dynamic_cast<texture::GLTexture*>(texture);

            // if(glTexture)
            //     uiObjectJson["texture"] = glTexture->getName();
            // else
            //     ELIX_LOG_ERROR("No GL texture");

        }

        json["ui_elements"].push_back(uiObjectJson);
    }

    std::ofstream file(filePath);

    if (file.is_open())
    {
        file << std::setw(4) << json << std::endl;
        file.close();
    }
    else
        ELIX_LOG_ERROR("Failed to open file to save game objects: ", filePath);
}

void Scene::loadSceneFromFile(const std::string &filePath, AssetsCache& cache)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        ELIX_LOG_ERROR("Failed to open file: %s", filePath);
        return;
    }

    nlohmann::json json;

    try
    {
        file >> json;
    }
    catch (const nlohmann::json::parse_error& e)
    {
        ELIX_LOG_ERROR("Failed to parse scene file %s", e.what());
        return;
    }

    // if (json.contains("skybox"))
    // {
    //     auto skybox = std::make_shared<Skybox>();

    //     skybox->loadFromHDR(json["skybox"]);

    //     setSkybox(skybox);
    // }

    if (json.contains("game_objects"))
    {

        for (const auto& objectJson : json["game_objects"])
        {
            const std::string& name = objectJson.value("name", "undefined");

            auto gameObject = std::make_shared<GameObject>(name);

            if (objectJson.contains("model"))
            {
                const std::string modelName = objectJson["model"];

                if (auto modelAsset = cache.getAsset<AssetModel>(modelName))
                {
                    auto model = modelAsset->getModel();

                    gameObject->addComponent<components::MeshComponent>(model);

                    if (objectJson.contains("materials"))
                    {
                        const auto& materials = objectJson["materials"];

                        for (int i = 0; i < model->getNumMeshes(); ++i)
                        {
                            const std::string indexStr = std::to_string(i);

                            if (!materials.contains(indexStr))
                                continue;

                            const std::string materialName = materials.value(indexStr, "");

                            if (materialName.empty())
                            {
                                ELIX_LOG_WARN("Failed to find material in json with given %s", indexStr.c_str());
                                continue;
                            }

                            if (auto material = cache.getAsset<AssetMaterial>(materialName))
                            {
                                if(auto component = gameObject->getComponent<components::MeshComponent>())
                                    component->setMaterialOverride(i, material->getMaterial());
                            }
                            else
                                ELIX_LOG_WARN("Failed to find material %s", materialName.c_str());
                        }
                    }
                }
                else
                    ELIX_LOG_ERROR("Failed to attach mesh component because missing the model %s", modelName.c_str());
            }
            else
                ELIX_LOG_WARN("Failed to find model in .json. Is this okay?....");

            auto* transformation = gameObject->addComponent<components::TransformComponent>();

            if (objectJson.contains("position"))
            {
                const auto& pos = objectJson["position"];
                transformation->setPosition({ pos[0], pos[1], pos[2] });
            }

            if (objectJson.contains("scale"))
            {
                const auto& scale = objectJson["scale"];
                transformation->setScale({ scale[0], scale[1], scale[2] });
            }

            if (objectJson.contains("rotation"))
            {
                const auto& rot = objectJson["rotation"];
                transformation->setRotation({ rot[0], rot[1], rot[2] });
            }

            gameObject->addComponent<components::RigidbodyComponent>(gameObject);

            // if (isSkinned)
                // physics::PhysicsController::instance().resizeCollider({1.0f, 2.0f, 1.0f}, gameObject);

            if (objectJson.contains("components"))
            {
                for (const auto& componentJson : objectJson["components"])
                {
                    if (!componentJson.contains("type"))
                        continue;

                    //TODO make it more safe, Cause it sucks...
                    if (componentJson["type"] == "LightComponent")
                    {
                        auto light = std::make_shared<lighting::Light>();
                        light->type = static_cast<lighting::LightType>(componentJson["lightType"]);
                        const auto& direction = componentJson["direction"];
                        light->direction = glm::vec3(direction[0], direction[1], direction[2]);
                        const auto& position = componentJson["position"];
                        light->position = glm::vec3(position[0], position[1], position[2]);
                        const auto& color = componentJson["color"];
                        light->color = glm::vec3(color[0], color[1], color[2]);
                        light->strength = componentJson["strength"];
                        light->radius = componentJson["radius"];
                        gameObject->addComponent<components::LightComponent>(light);
                        addLight(light);
                    }
                    else if (componentJson["type"] == "AnimatorComponent")
                    {
                        gameObject->addComponent<components::AnimatorComponent>();
                    }
                    else if (componentJson["type"] == "ScriptsComponent")
                    {
                        auto* scriptComponent = gameObject->addComponent<components::ScriptComponent>();

                        if (componentJson.contains("scripts"))
                        {
                            for (auto& scriptEntry : componentJson["scripts"])
                            {
                                auto script = scripting::ScriptSystem::createScript(scriptEntry.get<std::string>());

                                if(!script)
                                    ELIX_LOG_ERROR("Failed to create script ", scriptEntry.get<std::string>());
                                else
                                    scriptComponent->addScript(script);
                            }
                        }
                    }
                }
            }
                addGameObject(gameObject);
        }
    }



    if (json.contains("ui_elements"))
    {
        std::vector<std::shared_ptr<ui::UIWidget>> uiElements;

        for(const auto& uiElementJson : json["ui_elements"])
        {
            auto uiElement = std::make_shared<ui::UIWidget>();

            uiElement->setName(uiElementJson.value("name", "undefined"));


            if (uiElementJson.contains("position"))
            {
                const auto& pos = uiElementJson["position"];
                uiElement->setPosition({ pos[0], pos[1]});
            }

            if(uiElementJson.contains("size"))
            {
                const auto& size = uiElementJson["size"];
                uiElement->setSize({size[0], size[1]});
            }

            if(uiElementJson.contains("color"))
            {
                const auto& color = uiElementJson["color"];
                uiElement->setColor({color[0], color[1], color[2], color[3]});
            }

            if(uiElementJson.contains("is_visible"))
            {
                const bool& isVisible = uiElementJson["is_visible"];
                uiElement->setVisible(isVisible);
            }

            if(uiElementJson.contains("alpha"))
            {
                const float& alpha = uiElementJson["alpha"];
                uiElement->setAlpha(alpha);
            }

            if(uiElementJson.contains("texture"))
            {
                const std::string& textureName = uiElementJson["texture"];
                auto assetTexture = cache.getAsset<AssetTexture>(textureName);

                if(assetTexture)
                    uiElement->setTexture(assetTexture->getTexture());
                else
                    ELIX_LOG_WARN("Failed to find ", textureName, " texture");
            }

            //TODO maybe use std::move
            uiElements.push_back(uiElement);
            addUIElement(uiElement);
        }

    }


    file.close();

}


ELIX_NAMESPACE_END