#pragma once

// Display constants
namespace Display {
    constexpr int WIDTH = 1280;
    constexpr int HEIGHT = 720;
    constexpr int FPS = 60;
    constexpr int FRAME_DELAY_MS = 16;
}

// Log display constants
namespace LogDisplay {
    constexpr size_t MAX_LINES = 300;
    constexpr int LINE_HEIGHT = 22;
    constexpr int FONT_SIZE = 18;
}

// Network refresh settings
namespace Network {
    constexpr uint32_t IP_REFRESH_PERIOD_MS = 2000;
}

// Dropbear process constants
namespace Dropbear {
    constexpr int MAX_WAIT_ATTEMPTS = 20;
    constexpr int WAIT_DELAY_MS = 10;
}
