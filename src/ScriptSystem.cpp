#include "ScriptSystem.hpp"
#include "Logger.hpp"

ELIX_NAMESPACE_BEGIN

static ScriptsRegister* s_register{nullptr};
static LibraryHandle s_library{nullptr};

bool ScriptSystem::loadLibrary(const std::string& path)
{
    if(s_library)
        unloadLibrary();
    
    s_library = elix::LibrariesLoader::loadLibrary(path);

    if(!s_library)
        return false;

    using GetScriptsRegisterFunc = ScriptsRegister*(*)();

    auto getFunction = (GetScriptsRegisterFunc)elix::LibrariesLoader::getFunction("getScriptsRegister", s_library);

    if (!getFunction)
    {
        ELIX_LOG_ERROR("Could not get function 'getScriptsRegister'");
        return false;
    }

    s_register = getFunction();

    return true;
}

void ScriptSystem::unloadLibrary()  
{
    elix::LibrariesLoader::closeLibrary(s_library);
}

std::vector<std::string> ScriptSystem::getAvailableScripts()
{
    if(!s_register)
        return{};

    return s_register->getScriptNames();
}

std::shared_ptr<Script> ScriptSystem::createScript(const std::string& name)
{   
    if(!s_register)
        return nullptr;

    return s_register->createScript(name);
}

ScriptsRegister* ScriptSystem::getRegister()
{
    return s_register;
}

LibraryHandle ScriptSystem::getLibrary()
{
    return s_library;
}


ELIX_NAMESPACE_END