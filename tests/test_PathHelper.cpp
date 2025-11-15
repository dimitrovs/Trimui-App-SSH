#include "test_framework.h"
#include "../src/PathHelper.h"

void registerPathHelperTests(TestRunner& runner) {
    // Test appBaseDir returns non-empty path
    runner.addTest("PathHelper::appBaseDir returns non-empty string", []() {
        std::string baseDir = PathHelper::appBaseDir();
        ASSERT_FALSE(baseDir.empty());
    });
    
    // Test appBaseDir ends with slash
    runner.addTest("PathHelper::appBaseDir ends with slash", []() {
        std::string baseDir = PathHelper::appBaseDir();
        ASSERT_TRUE(baseDir.back() == '/');
    });
    
    // Test bundledDropbearPath
    runner.addTest("PathHelper::bundledDropbearPath returns valid path", []() {
        std::string path = PathHelper::bundledDropbearPath();
        ASSERT_FALSE(path.empty());
        ASSERT_TRUE(path.find("dropbear") != std::string::npos);
    });
    
    // Test bundledDropbearKeygenPath
    runner.addTest("PathHelper::bundledDropbearKeygenPath returns valid path", []() {
        std::string path = PathHelper::bundledDropbearKeygenPath();
        ASSERT_FALSE(path.empty());
        ASSERT_TRUE(path.find("dropbearkey") != std::string::npos);
    });
    
    // Test hostKeyPath
    runner.addTest("PathHelper::hostKeyPath returns valid path", []() {
        std::string path = PathHelper::hostKeyPath();
        ASSERT_FALSE(path.empty());
        ASSERT_TRUE(path.find("dropbear_rsa_host_key") != std::string::npos);
    });
    
    // Test paths are consistent
    runner.addTest("PathHelper paths use same base directory", []() {
        std::string baseDir = PathHelper::appBaseDir();
        std::string dropbearPath = PathHelper::bundledDropbearPath();
        std::string keygenPath = PathHelper::bundledDropbearKeygenPath();
        std::string hostKeyPath = PathHelper::hostKeyPath();
        
        ASSERT_TRUE(dropbearPath.find(baseDir) == 0);
        ASSERT_TRUE(keygenPath.find(baseDir) == 0);
        ASSERT_TRUE(hostKeyPath.find(baseDir) == 0);
    });
    
    // Test paths don't have double slashes
    runner.addTest("PathHelper paths don't contain double slashes", []() {
        std::string dropbearPath = PathHelper::bundledDropbearPath();
        std::string keygenPath = PathHelper::bundledDropbearKeygenPath();
        std::string hostKeyPath = PathHelper::hostKeyPath();
        
        ASSERT_TRUE(dropbearPath.find("//") == std::string::npos);
        ASSERT_TRUE(keygenPath.find("//") == std::string::npos);
        ASSERT_TRUE(hostKeyPath.find("//") == std::string::npos);
    });
}
