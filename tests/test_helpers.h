#pragma once

#include <stdexcept>
#include <string>

// Test assertion helpers
class AssertionError : public std::runtime_error {
public:
    explicit AssertionError(const std::string& msg) : std::runtime_error(msg) {}
};

#define ASSERT_TRUE(condition) \
    if (!(condition)) throw AssertionError("Assertion failed: " #condition)

#define ASSERT_FALSE(condition) \
    if (condition) throw AssertionError("Assertion failed: !(" #condition ")")

#define ASSERT_EQ(expected, actual) \
    if ((expected) != (actual)) { \
        throw AssertionError("Expected: " + std::to_string(expected) + ", Got: " + std::to_string(actual)); \
    }

#define ASSERT_STR_EQ(expected, actual) \
    if ((expected) != (actual)) { \
        throw AssertionError("Expected: \"" + std::string(expected) + "\", Got: \"" + std::string(actual) + "\""); \
    }
