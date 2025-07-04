#ifndef REFLECTED_OBJECT_HPP
#define REFLECTED_OBJECT_HPP

#include "DefaultMacros.hpp"
#include <variant>
#include <unordered_map>
#include <string>
#include <memory>
#include <vector>
#include <functional>

class GameObject;

ELIX_NAMESPACE_BEGIN

using PropertyValue = std::variant<
    bool, 
    int, 
    float, 
    double, 
    std::string,
    std::shared_ptr<GameObject>
>;

class ReflectedObject
{
public:
    ReflectedObject()
    {
        for (auto& registrar : s_registrars)
            registrar(this);
    }

    template<typename T>
    void setProperty(const std::string& name, const T& value)
    {
        m_properties[name] = value;
    }

    template<typename T>
    T getProperty(const std::string& name) const
    {
        return std::get<T>(m_properties.at(name));
    }

    const std::unordered_map<std::string, PropertyValue>& getProperties() const
    {
        return m_properties;
    }

    virtual ~ReflectedObject() = default;

protected:
    template<typename T>
    static void registerStaticField(const std::string& name, T defaultValue)
    {
        s_registrars.push_back([=](ReflectedObject* obj) {
            obj->setProperty(name, defaultValue);
        });
    }

private:
    std::unordered_map<std::string, PropertyValue> m_properties;
    static inline std::vector<std::function<void(ReflectedObject*)>> s_registrars;
};

#define ELIX_FIELD(type, name, defaultValue) \
private: \
    type m_##name = defaultValue; \
public: \
    type get_##name() const { return m_##name; } \
    void set_##name(type value) { \
        m_##name = value; \
        setProperty(#name, value); \
    } \
public: \
    static inline bool _registered_##name = []() { \
        elix::ReflectedObject::registerStaticField<type>(#name, defaultValue); \
        return true; \
    }()

ELIX_NAMESPACE_END

#endif // REFLECTED_OBJECT_HPP