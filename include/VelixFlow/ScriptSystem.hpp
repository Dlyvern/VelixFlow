#ifndef SCRIPT_SYSTEM_HPP
#define SCRIPT_SYSTEM_HPP

#include "DefaultMacros.hpp"
#include "ScriptsRegister.hpp"
#include <string>
#include <vector>
#include <memory>

ELIX_NAMESPACE_BEGIN

class ScriptSystem
{
public:
    static bool loadLibrary(const std::string& path);
    static void unloadLibrary();
    static std::vector<std::string> getAvailableScripts();
    static std::shared_ptr<Script> createScript(const std::string& name);
    static ScriptsRegister* getRegister();
    static void* getLibrary();
};

ELIX_NAMESPACE_END

#endif //SCRIPT_SYSTEM_HPP