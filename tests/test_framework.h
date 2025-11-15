#pragma once

#include "test_helpers.h"
#include <iostream>
#include <vector>
#include <string>
#include <functional>

// Test framework
class TestRunner {
public:
    using TestFunc = std::function<void()>;
    
    void addTest(const std::string& name, TestFunc func) {
        tests_.push_back({name, func});
    }
    
    int run() {
        std::cout << "Running " << tests_.size() << " tests...\n\n";
        
        int passed = 0;
        int failed = 0;
        
        for (const auto& test : tests_) {
            try {
                std::cout << "[ RUN  ] " << test.name << std::endl;
                test.func();
                std::cout << "[ PASS ] " << test.name << std::endl;
                passed++;
            } catch (const std::exception& e) {
                std::cout << "[ FAIL ] " << test.name << std::endl;
                std::cout << "         " << e.what() << std::endl;
                failed++;
            }
        }
        
        std::cout << "\n========================================\n";
        std::cout << "Tests passed: " << passed << "/" << tests_.size() << std::endl;
        std::cout << "Tests failed: " << failed << "/" << tests_.size() << std::endl;
        std::cout << "========================================\n";
        
        return failed == 0 ? 0 : 1;
    }
    
private:
    struct Test {
        std::string name;
        TestFunc func;
    };
    
    std::vector<Test> tests_;
};
