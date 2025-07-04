#ifndef SIGNAL_HPP
#define SIGNAL_HPP

#include "DefaultMacros.hpp"
#include <functional>
#include <memory>
#include <vector>

ELIX_NAMESPACE_BEGIN

template<typename... Args>
class Signal
{
public:
    using Slot = std::function<void(Args...)>;
    using Connection = std::shared_ptr<void>;

    Connection connect(const Slot& slot)
    {
        auto id = std::make_shared<int>(m_nextId++);
        m_slots.emplace_back(id, slot);
        return id;
    }

    void disconnect(const Connection& connection)
    {
        m_slots.erase(std::remove_if(m_slots.begin(), m_slots.end(), [&connection](const auto& pair) {return pair.first == connection; }), m_slots.end());
    }

    void emit(Args... args)
    {
        for(const auto& [connection, slot] : m_slots)
            if(slot) slot(args...);
    }

private:
    int m_nextId{0};
    std::vector<std::pair<Connection, Slot>> m_slots;
};

ELIX_NAMESPACE_END

#endif //SIGNAL_HPP
