#pragma once

#include "design.hpp"
#include "../service.hpp"

#include <unordered_map>

class DesignManager : public Service<DesignManager>
{
public:
    ~DesignManager();

    DesignPtr getActiveDesign() const;

    void setActiveDesign(int tabIndex);
    void closeDesign(int tabIndex);

    DesignPtr createDesign(int tabIndex); 
    DesignPtr openDesign(const std::string& path, int tabIndex);

private:
    DesignPtr m_activeDesign;
    std::unordered_map<int, DesignPtr> m_designs;
};