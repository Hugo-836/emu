#include "platform/Platform.h"
#include <array>
#include <cstdint>

int main() {
    Platform platform{"emu", 64, 32, 10};

    std::array<uint32_t, 64 * 32> framebuffer{};
    for (int y = 0; y < 32; ++y)
        for (int x = 0; x < 64; ++x)
            framebuffer[y * 64 + x] = ((x + y) % 2 == 0) ? 0xFFFFFFFF : 0xFF000000;

    while (platform.processEvents()) {
        platform.draw(framebuffer.data());
    }
    return 0;
}