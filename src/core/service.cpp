#include "service.hpp"

void ServiceManager::startUp()
{
	m_callback = std::bind(&ServiceManager::addService<ServiceManager>, this, std::placeholders::_1);
}

void ServiceManager::shutDown()
{
	for (auto it : m_services)
		it->shutDown();
	m_callback = nullptr;
}
