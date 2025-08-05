#ifndef SCRIPT_COMPONENT_HPP
#define SCRIPT_COMPONENT_HPP

#include <memory>
#include <string>
#include <unordered_map>

#include "VelixFlow/DefaultMacros.hpp"
#include "VelixFlow/Components/Component.hpp"
#include "VelixFlow/Scripting/Script.hpp"

ELIX_NAMESPACE_BEGIN

namespace components
{
    class ScriptComponent final : public IComponent
    {
    public:
        void addScript(const std::string& name);

        void addScript(std::shared_ptr<scripting::Script> script);

        void update(float deltaTime) override;

        void setUpdateScripts(bool flag);
        bool getUpdateScripts() const;

        const std::unordered_map<std::string, std::shared_ptr<scripting::Script>>& getScripts() const;

        ~ScriptComponent() override;

    private:
        std::unordered_map<std::string, std::shared_ptr<scripting::Script>> m_scripts;
        bool m_updateScripts{false};
    };
}

ELIX_NAMESPACE_END

#endif //SCRIPT_COMPONENT_HPP
