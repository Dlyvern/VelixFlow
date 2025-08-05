#ifndef SCRIPT_SYSTEM_HPP
#define SCRIPT_SYSTEM_HPP

#include "VelixFlow/DefaultMacros.hpp"
#include "VelixFlow/Scripting/ScriptsRegister.hpp"
#include <string>
#include <vector>
#include <memory>
#include "VelixFlow/Scripting/LibrariesLoader.hpp"

ELIX_NAMESPACE_BEGIN

namespace scripting
{
    class ScriptSystem
    {
    public:
        static bool loadLibrary(const std::string& path);
        static void unloadLibrary();
        static std::vector<std::string> getAvailableScripts();
        static std::shared_ptr<Script> createScript(const std::string& name);
        static ScriptsRegister* getRegister();
        static LibraryHandle getLibrary();
    };
}

ELIX_NAMESPACE_END

#endif //SCRIPT_SYSTEM_HPP