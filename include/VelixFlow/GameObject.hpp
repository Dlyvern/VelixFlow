#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include "VelixFlow/Components/Component.hpp"
#include "VelixFlow/Signal.hpp"
#include "VelixFlow/DefaultMacros.hpp"
#include "VelixFlow/LayerMask.hpp"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>

#include <string>

template<typename T>
struct IsMultiComponent {
    static constexpr bool value = false;
};

template<>
struct IsMultiComponent<class AudioComponent> {
    static constexpr bool value = true;
};

ELIX_NAMESPACE_BEGIN

class GameObject
{
public:
    explicit GameObject(const std::string&name) : m_name(name) {}

    void setLayerMask(const LayerMask& layerMask)
    {
        m_layerMask= layerMask;
    }

    void setName(const std::string& name)
    {
        m_name = name;
    }

    [[nodiscard]] const LayerMask& getLayerMask() const
    {
        return m_layerMask;
    }

    [[nodiscard]] const std::string& getName() const
    {
        return m_name;
    }

    virtual void destroy()
    {
        for (auto& component : m_components)
            component.second->destroy();
    }

    virtual void update(float deltaTime)
    {
        for (auto& [_, comp] : m_components)
            comp->update(deltaTime);
    }

    template<typename T, typename... Args>
    T* addComponent(Args&&... args)
    {
        static_assert(!std::is_abstract_v<T>, "GameObject::addComponent() Cannot add abstract component!");
        static_assert(std::is_base_of_v<components::IComponent, T>, "GameObject::addComponent() T must derive from IComponent class");

        const auto type = std::type_index(typeid(T));
        auto comp = std::make_shared<T>(std::forward<Args>(args)...);
        T* ptr = comp.get();
        comp->setOwner(this);

        if constexpr (IsMultiComponent<T>::value)
            m_multiComponents[type].emplace_back(std::move(comp));
        else
            m_components[type] = std::move(comp);
        return ptr;
    }

    template<typename T>
    T* getComponent()
    {
        const auto it = m_components.find(std::type_index(typeid(T)));
        return it != m_components.end() ? static_cast<T*>(it->second.get()) : nullptr;
    }

    template<typename T>
    void removeComponent()
    {
        const auto type = std::type_index(typeid(T));
        if constexpr (IsMultiComponent<T>::value)
            m_multiComponents.erase(type);
        else
            m_components.erase(type);
    }

    template<typename T>
    std::vector<T*> getComponents()
    {
        std::vector<T*> result;
        const auto type = std::type_index(typeid(T));

        if constexpr (IsMultiComponent<T>::value) 
        {
            auto it = m_multiComponents.find(type);

            if (it != m_multiComponents.end())
                for (auto& comp : it->second)
                    result.push_back(static_cast<T*>(comp.get()));
        } 
        else 
        {
            auto it = m_components.find(type);

            if (it != m_components.end())
                result.push_back(static_cast<T*>(it->second.get()));
        }

        return result;
    }

    template<typename T>
    bool hasComponent() const
    {
        return m_components.contains(std::type_index(typeid(T)));
    }

    virtual ~GameObject() = default;
private:
    std::unordered_map<std::type_index, std::shared_ptr<components::IComponent>> m_components;
    std::unordered_map<std::type_index, std::vector<std::shared_ptr<components::IComponent>>> m_multiComponents;
    LayerMask m_layerMask{LayerMask::DEFAULT};
    std::string m_name;
};

ELIX_NAMESPACE_END

#endif //GAME_OBJECT_HPP
