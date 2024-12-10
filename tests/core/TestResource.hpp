#pragma once

#include "core/Resource.hpp"
#include <fstream>

// A simple test resource that just checks if a file exists
class TestResource : public Resource {
public:
    bool loadFromFile(const std::string& filePath) override {
        std::ifstream file(filePath);
        bool exists = file.good();
        if (exists) {
            path = filePath;
        }
        return exists;
    }

    // Additional test functionality
    void setTestData(const std::string& data) {
        testData = data;
    }

    std::string getTestData() const {
        return testData;
    }

private:
    std::string testData;
};
