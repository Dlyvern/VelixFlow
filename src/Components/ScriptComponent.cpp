#include "VelixFlow/Components/ScriptComponent.hpp"
#include "VelixFlow/Logger.hpp"
#include "VelixFlow/Scripting/ScriptsRegister.hpp"
#include "VelixFlow/GameObject.hpp"

ELIX_NAMESPACE_BEGIN

namespace components
{
    void ScriptComponent::addScript(const std::string &name)
    {
        auto script = scripting::ScriptsRegister::instance().createScript(name);

        if (!script)
        {
            ELIX_LOG_WARN("Script ", name, " not found");
            return;
        }

        script->setOwner(this->getOwner());

        m_scripts[name] = script;
    }

    void ScriptComponent::addScript(std::shared_ptr<scripting::Script> script)
    {
        script->setOwner(this->getOwner());

        m_scripts[script->getScriptName()] = script;
    }


    void ScriptComponent::update(float deltaTime)
    {
        if (!m_updateScripts)
            return;

        for (auto& [name, script] : m_scripts)
            if (script) script->onUpdate(deltaTime);
    }

    bool ScriptComponent::getUpdateScripts() const
    {
        return m_updateScripts;
    }

    void ScriptComponent::setUpdateScripts(bool flag)
    {
        m_updateScripts = flag;
    }

    const std::unordered_map<std::string, std::shared_ptr<scripting::Script>>& ScriptComponent::getScripts() const
    {
        return m_scripts;
    }

    ScriptComponent::~ScriptComponent() = default;

} //namespace components

ELIX_NAMESPACE_END
