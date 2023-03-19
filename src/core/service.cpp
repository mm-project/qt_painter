#include "service.hpp"

void ServiceManager::shutDown()
{
    for (auto it : m_services)
        if (it != this)
            it->shutDown();
    m_services.clear();
    m_callback = nullptr;
}
