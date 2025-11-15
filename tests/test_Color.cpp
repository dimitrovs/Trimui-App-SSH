#include "test_framework.h"
#include "../src/Color.h"

void registerColorTests(TestRunner& runner) {
    // Test default constructor
    runner.addTest("Color default constructor creates black", []() {
        Color c;
        ASSERT_EQ(0, c.r);
        ASSERT_EQ(0, c.g);
        ASSERT_EQ(0, c.b);
        ASSERT_EQ(255, c.a);
    });
    
    // Test parameterized constructor
    runner.addTest("Color constructor sets RGBA values", []() {
        Color c(100, 150, 200, 128);
        ASSERT_EQ(100, c.r);
        ASSERT_EQ(150, c.g);
        ASSERT_EQ(200, c.b);
        ASSERT_EQ(128, c.a);
    });
    
    // Test White predefined color
    runner.addTest("Color::White returns white color", []() {
        Color c = Color::White();
        ASSERT_EQ(255, c.r);
        ASSERT_EQ(255, c.g);
        ASSERT_EQ(255, c.b);
        ASSERT_EQ(255, c.a);
    });
    
    // Test Gray predefined color
    runner.addTest("Color::Gray returns gray color", []() {
        Color c = Color::Gray();
        ASSERT_EQ(200, c.r);
        ASSERT_EQ(200, c.g);
        ASSERT_EQ(200, c.b);
        ASSERT_EQ(255, c.a);
    });
    
    // Test LightGreen predefined color
    runner.addTest("Color::LightGreen returns light green color", []() {
        Color c = Color::LightGreen();
        ASSERT_EQ(180, c.r);
        ASSERT_EQ(255, c.g);
        ASSERT_EQ(180, c.b);
        ASSERT_EQ(255, c.a);
    });
    
    // Test LightBlue predefined color
    runner.addTest("Color::LightBlue returns light blue color", []() {
        Color c = Color::LightBlue();
        ASSERT_EQ(200, c.r);
        ASSERT_EQ(200, c.g);
        ASSERT_EQ(255, c.b);
        ASSERT_EQ(255, c.a);
    });
    
    // Test DarkBackground predefined color
    runner.addTest("Color::DarkBackground returns dark background color", []() {
        Color c = Color::DarkBackground();
        ASSERT_EQ(40, c.r);
        ASSERT_EQ(40, c.g);
        ASSERT_EQ(60, c.b);
        ASSERT_EQ(255, c.a);
    });
    
    // Test toSDLColor conversion
    runner.addTest("Color::toSDLColor converts correctly", []() {
        Color c(100, 150, 200, 128);
        SDL_Color sdl = c.toSDLColor();
        ASSERT_EQ(100, sdl.r);
        ASSERT_EQ(150, sdl.g);
        ASSERT_EQ(200, sdl.b);
        ASSERT_EQ(128, sdl.a);
    });
    
    // Test constexpr colors
    runner.addTest("Color predefined colors are compile-time constants", []() {
        constexpr Color white = Color::White();
        constexpr Color gray = Color::Gray();
        // If this compiles, constexpr works
        ASSERT_TRUE(true);
    });
    
    // Test color component ranges
    runner.addTest("Color components accept full uint8_t range", []() {
        Color c1(0, 0, 0, 0);
        Color c2(255, 255, 255, 255);
        
        ASSERT_EQ(0, c1.r);
        ASSERT_EQ(255, c2.r);
        ASSERT_EQ(0, c1.a);
        ASSERT_EQ(255, c2.a);
    });
}
