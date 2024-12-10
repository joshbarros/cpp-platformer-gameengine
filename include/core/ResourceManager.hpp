#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <stdexcept>
#include "Logger.hpp"

class ResourceManager {
public:
    static ResourceManager& getInstance() {
        static ResourceManager instance;
        return instance;
    }

    // Generic resource loading
    template<typename T>
    void loadResource(const std::string& name, const std::string& path) {
        if (resources<T>.find(name) != resources<T>.end()) {
            Logger::Warn("Resource '" + name + "' already exists. Skipping load.");
            return;
        }

        try {
            auto resource = std::make_shared<T>();
            if (resource->loadFromFile(path)) {
                resources<T>[name] = resource;
                Logger::Info("Successfully loaded resource: " + name);
            } else {
                Logger::Error("Failed to load resource: " + name);
            }
        } catch (const std::exception& e) {
            Logger::Error("Exception while loading resource '" + name + "': " + e.what());
        }
    }

    // Generic resource getter
    template<typename T>
    std::shared_ptr<T> getResource(const std::string& name) {
        auto it = resources<T>.find(name);
        if (it == resources<T>.end()) {
            Logger::Warn("Resource '" + name + "' not found.");
            return nullptr;
        }
        return it->second;
    }

    // Resource existence check
    template<typename T>
    bool hasResource(const std::string& name) {
        return resources<T>.find(name) != resources<T>.end();
    }

    // Resource removal
    template<typename T>
    void removeResource(const std::string& name) {
        auto it = resources<T>.find(name);
        if (it != resources<T>.end()) {
            resources<T>.erase(it);
            Logger::Info("Resource removed: " + name);
        }
    }

    // Clear all resources of a specific type
    template<typename T>
    void clearResources() {
        resources<T>.clear();
        Logger::Info("Cleared all resources of specified type");
    }

    // Clear all resources
    void clearAllResources() {
        // Implementation will be added as we add specific resource types
        Logger::Info("Cleared all resources");
    }

private:
    ResourceManager() = default;
    ~ResourceManager() = default;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    // Resource storage for different types
    template<typename T>
    static std::unordered_map<std::string, std::shared_ptr<T>> resources;
};

// Static member initialization
template<typename T>
std::unordered_map<std::string, std::shared_ptr<T>> ResourceManager::resources;
