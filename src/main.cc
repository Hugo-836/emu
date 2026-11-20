#include <SDL.h>
#include <array>
#include <cstdint>

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "emu",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        640, 320,
        0);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                             SDL_TEXTUREACCESS_STREAMING, 64, 32);

    // Framebuffer de test : un damier 64x32
    std::array<uint32_t, 64 * 32> framebuffer{};
    for (int y = 0; y < 32; ++y)
        for (int x = 0; x < 64; ++x)
            framebuffer[y * 64 + x] = ((x + y) % 2 == 0) ? 0xFFFFFFFF : 0xFF000000;

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        SDL_UpdateTexture(texture, nullptr, framebuffer.data(), 64 * sizeof(uint32_t));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}