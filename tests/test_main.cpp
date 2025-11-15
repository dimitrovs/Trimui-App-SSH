#include "test_framework.h"

// External test declarations
void registerPathHelperTests(TestRunner& runner);
void registerNetworkManagerTests(TestRunner& runner);
void registerColorTests(TestRunner& runner);

int main() {
    TestRunner runner;
    
    // Register all test suites
    registerPathHelperTests(runner);
    registerNetworkManagerTests(runner);
    registerColorTests(runner);
    
    return runner.run();
}
