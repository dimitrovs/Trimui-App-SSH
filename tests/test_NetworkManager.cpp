#include "test_framework.h"
#include "../src/NetworkManager.h"
#include <algorithm>

void registerNetworkManagerTests(TestRunner& runner) {
    // Test NetworkManager instantiation
    runner.addTest("NetworkManager can be instantiated", []() {
        NetworkManager manager;
        // Should not throw
    });
    
    // Test getIPv4Addresses doesn't crash
    runner.addTest("NetworkManager::getIPv4Addresses doesn't crash", []() {
        NetworkManager manager;
        auto addrs = manager.getIPv4Addresses();
        // Should return successfully (may be empty if no network)
    });
    
    // Test returned addresses format
    runner.addTest("NetworkManager addresses contain interface names", []() {
        NetworkManager manager;
        auto addrs = manager.getIPv4Addresses();
        
        // If we have addresses, they should contain colon separator
        for (const auto& addr : addrs) {
            ASSERT_TRUE(addr.find(':') != std::string::npos);
        }
    });
    
    // Test priority sorting
    runner.addTest("NetworkManager prioritizes wlan over eth", []() {
        NetworkManager manager;
        auto addrs = manager.getIPv4Addresses();
        
        // Find wlan and eth entries
        auto wlan_it = std::find_if(addrs.begin(), addrs.end(),
            [](const std::string& s) { return s.find("wlan") == 0; });
        auto eth_it = std::find_if(addrs.begin(), addrs.end(),
            [](const std::string& s) { return s.find("eth") == 0; });
        
        // If both exist, wlan should come before eth
        if (wlan_it != addrs.end() && eth_it != addrs.end()) {
            ASSERT_TRUE(wlan_it < eth_it);
        }
    });
    
    // Test loopback is excluded
    runner.addTest("NetworkManager excludes loopback addresses", []() {
        NetworkManager manager;
        auto addrs = manager.getIPv4Addresses();
        
        // Should not contain 127.0.0.1 or lo interface
        for (const auto& addr : addrs) {
            ASSERT_FALSE(addr.find("127.0.0.1") != std::string::npos);
            ASSERT_FALSE(addr.find("lo:") == 0);
        }
    });
    
    // Test IPv4 format
    runner.addTest("NetworkManager returns valid IPv4 format", []() {
        NetworkManager manager;
        auto addrs = manager.getIPv4Addresses();
        
        // Check each address contains dot-separated numbers after colon
        for (const auto& addr : addrs) {
            auto colon_pos = addr.find(':');
            if (colon_pos != std::string::npos) {
                std::string ip_part = addr.substr(colon_pos + 1);
                // IP should contain dots
                ASSERT_TRUE(ip_part.find('.') != std::string::npos);
            }
        }
    });
}
