#include "service.hpp"

void ServiceManager::startUp()
{

}

void ServiceManager::shutDown()
{
	for (auto it : m_services)
		it->shutDown();
}
