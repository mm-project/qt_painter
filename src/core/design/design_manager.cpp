#include "design_manager.hpp"
#include <memory>
#include <fstream>

DesignManager::~DesignManager()
{
    // Clear all designs when the manager is destroyed
    m_designs.clear();
    m_activeDesign.reset();
}

DesignPtr DesignManager::getActiveDesign() const
{
    return m_activeDesign;
}

void DesignManager::setActiveDesign(int tabIndex)
{
    if (m_designs.find(tabIndex) == m_designs.end())
    {
        throw std::runtime_error("Design not found");
    }
    m_activeDesign = m_designs[tabIndex];
}

DesignPtr DesignManager::createDesign(int tabIndex)
{
    // Create a new Design instance
    auto design = std::make_shared<Design>();
    // Store it in the designs map
    m_designs[tabIndex] = design;
    // Set it as the active design
    return design;
}

DesignPtr DesignManager::openDesign(const std::string& path, int tabIndex)
{
    try 
    {
        // Create a new Design instance
        auto design = std::make_shared<Design>();
        // Load it from file
        design->loadFromFile(path);
        // Store it in the designs map
        m_designs[tabIndex] = design;
        // Set it as the active design
        setActiveDesign(tabIndex);
        return design;
    }
    catch (const std::exception& e) 
    {
        // Handle file loading errors gracefully
        // Could throw or log error depending on requirements
        throw std::runtime_error("Failed to open design from path: " + path + ". Error: " + e.what());
    }
    return nullptr;
}

void DesignManager::closeDesign(int tabIndex)
{
    auto it = m_designs.find(tabIndex);
    if (it == m_designs.end())
        return;

    // If we're closing the active design, clear the active design pointer
    if (m_activeDesign == it->second)
    {
        m_activeDesign.reset();
    }
    m_designs.erase(it);
}