#pragma once

#include <string>

class Resource {
public:
    virtual ~Resource() = default;
    
    // Pure virtual function that all resources must implement
    virtual bool loadFromFile(const std::string& path) = 0;
    
    // Get the path from which the resource was loaded
    const std::string& getPath() const { return path; }

protected:
    std::string path;
};
