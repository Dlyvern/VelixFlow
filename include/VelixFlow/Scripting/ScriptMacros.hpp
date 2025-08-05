#ifndef SCRIPT_MACROS_HPP
#define SCRIPT_MACROS_HPP
#include <memory>
#include "ScriptsRegister.hpp"

#ifdef _WIN32
#define ELIXIR_API __declspec(dllexport)
#else
#define ELIXIR_API __attribute__((visibility("default")))
#endif

#define CONCAT_IMPL(x, y) x##y
#define CONCAT(x, y) CONCAT_IMPL(x, y)

#define REGISTER_SCRIPT(ScriptClass)                                  \
class CONCAT(ScriptClass, _AutoRegister) {                        \
public:                                                           \
CONCAT(ScriptClass, _AutoRegister)() {                        \
elix::scripting::ScriptsRegister::instance().registerScript(               \
#ScriptClass,                                         \
[]() -> std::shared_ptr<elix::scripting::Script> {                     \
return std::make_shared<ScriptClass>();           \
}                                                     \
);                                                        \
}                                                             \
};                                                                \
static CONCAT(ScriptClass, _AutoRegister)                         \
CONCAT(_autoRegister_, __COUNTER__);


#define ELIXIR_IMPLEMENT_GAME_MODULE()                                    \
extern "C" ELIXIR_API elix::scripting::ScriptsRegister* getScriptsRegister()               \
{                                                                         \
    return &elix::scripting::ScriptsRegister::instance();                                  \
}

#endif //SCRIPT_MACROS_HPP
