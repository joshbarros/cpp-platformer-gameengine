#include <gtest/gtest.h>
#include "core/ResourceManager.hpp"
#include "TestResource.hpp"
#include <filesystem>
#include <fstream> // Added necessary header for std::ofstream

namespace {

class ResourceManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a test file
        std::ofstream testFile("test_resource.txt");
        testFile << "Test content";
        testFile.close();
    }

    void TearDown() override {
        // Clean up test file
        std::filesystem::remove("test_resource.txt");
        ResourceManager::getInstance().clearAllResources();
    }
};

TEST_F(ResourceManagerTest, LoadResource) {
    auto& manager = ResourceManager::getInstance();
    
    // Test loading a resource
    manager.loadResource<TestResource>("test1", "test_resource.txt");
    EXPECT_TRUE(manager.hasResource<TestResource>("test1"));
    
    // Test loading same resource again (should skip)
    manager.loadResource<TestResource>("test1", "test_resource.txt");
    EXPECT_TRUE(manager.hasResource<TestResource>("test1"));
    
    // Test loading non-existent resource
    manager.loadResource<TestResource>("test2", "nonexistent.txt");
    EXPECT_FALSE(manager.hasResource<TestResource>("test2"));
}

TEST_F(ResourceManagerTest, GetResource) {
    auto& manager = ResourceManager::getInstance();
    
    // Load a resource
    manager.loadResource<TestResource>("test1", "test_resource.txt");
    
    // Test getting existing resource
    auto resource = manager.getResource<TestResource>("test1");
    EXPECT_NE(resource, nullptr);
    
    // Test getting non-existent resource
    auto nullResource = manager.getResource<TestResource>("nonexistent");
    EXPECT_EQ(nullResource, nullptr);
}

TEST_F(ResourceManagerTest, RemoveResource) {
    auto& manager = ResourceManager::getInstance();
    
    // Load and then remove a resource
    manager.loadResource<TestResource>("test1", "test_resource.txt");
    EXPECT_TRUE(manager.hasResource<TestResource>("test1"));
    
    manager.removeResource<TestResource>("test1");
    EXPECT_FALSE(manager.hasResource<TestResource>("test1"));
    
    // Try to remove non-existent resource (should not crash)
    manager.removeResource<TestResource>("nonexistent");
}

TEST_F(ResourceManagerTest, ClearResources) {
    auto& manager = ResourceManager::getInstance();
    
    // Load multiple resources
    manager.loadResource<TestResource>("test1", "test_resource.txt");
    manager.loadResource<TestResource>("test2", "test_resource.txt");
    
    EXPECT_TRUE(manager.hasResource<TestResource>("test1"));
    EXPECT_TRUE(manager.hasResource<TestResource>("test2"));
    
    // Clear resources of TestResource type
    manager.clearResources<TestResource>();
    
    EXPECT_FALSE(manager.hasResource<TestResource>("test1"));
    EXPECT_FALSE(manager.hasResource<TestResource>("test2"));
}

TEST_F(ResourceManagerTest, ResourceDataPersistence) {
    auto& manager = ResourceManager::getInstance();
    
    // Load a resource
    manager.loadResource<TestResource>("test1", "test_resource.txt");
    
    // Get the resource and set some test data
    auto resource = manager.getResource<TestResource>("test1");
    ASSERT_NE(resource, nullptr);
    resource->setTestData("test data");
    
    // Get the same resource again and verify data persists
    auto sameResource = manager.getResource<TestResource>("test1");
    ASSERT_NE(sameResource, nullptr);
    EXPECT_EQ(sameResource->getTestData(), "test data");
}

} // namespace
