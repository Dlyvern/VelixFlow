#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>

#include "Common.hpp"
#include "Component.hpp"
#include "Signal.hpp"


template<typename T>
struct IsMultiComponent {
    static constexpr bool value = false;
};

template<>
struct IsMultiComponent<class AudioComponent> {
    static constexpr bool value = true;
};

class GameObject
{
public:
    explicit GameObject(const std::string&name);

    virtual void setLayerMask(const common::LayerMask& layerMask);
    virtual void setPosition(const glm::vec3& position);
    virtual void setScale(const glm::vec3& scale);
    virtual void setRotation(const glm::vec3 &axis);
    virtual void setName(const std::string& name);

    elix::Signal<const glm::vec3&> positionChanged;
    elix::Signal<const glm::mat4&> transformationChanged;

    [[nodiscard]] glm::vec3 getPosition() const;
    [[nodiscard]] glm::vec3 getScale() const;
    [[nodiscard]] glm::vec3 getRotation() const;
    [[nodiscard]] const common::LayerMask& getLayerMask() const;
    [[nodiscard]] const std::string& getName() const;
    glm::mat4 getTransformMatrix();
    void setTransformMatrix(const glm::mat4& transformMatrix);

    virtual void destroy();
    virtual void update(float deltaTime);

    template<typename T, typename... Args>
    T* addComponent(Args&&... args)
    {
        static_assert(!std::is_abstract_v<T>, "GameObject::addComponent() Cannot add abstract component!");
        static_assert(std::is_base_of_v<Component, T>, "GameObject::addComponent() T must derive from Component class");

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

    virtual ~GameObject();
private:
    glm::mat4 m_transformMatrix;
    bool m_isTransformMatrixDirty{true};
    std::unordered_map<std::type_index, std::shared_ptr<Component>> m_components;
    std::unordered_map<std::type_index, std::vector<std::shared_ptr<Component>>> m_multiComponents;
    common::LayerMask m_layerMask{common::LayerMask::DEFAULT};
    glm::vec3 m_position{glm::vec3(0.0f, 0.0f, 0.0f)};
    glm::vec3 m_scale{glm::vec3(1.0f, 1.0f, 1.0f)};
    glm::vec3 m_rotation{0.0f};
    std::string m_name;
};

#endif //GAME_OBJECT_HPP
