#include <SDL.h>
#include <array>
#include <cstdint>
#include <memory>

struct WindowDeleter {
    void operator()(SDL_Window* w) const {
        SDL_DestroyWindow(w);
    }
};

using WindowPtr = std::unique_ptr<SDL_Window, WindowDeleter>;

struct RendererDeleter {
    void operator()(SDL_Renderer* r) const {
        SDL_DestroyRenderer(r);
    }
};

using RendererPtr = std::unique_ptr<SDL_Renderer, RendererDeleter>;

struct TextureDeleter {
    void operator()(SDL_Texture* t) const {
        SDL_DestroyTexture(t);
    }
};

using TexturePtr = std::unique_ptr<SDL_Texture, TextureDeleter>;

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    {
        WindowPtr window{SDL_CreateWindow(
            "emu",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            640, 320,
            0)};

        RendererPtr renderer{SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_PRESENTVSYNC)};
        TexturePtr texture{SDL_CreateTexture(renderer.get(), SDL_PIXELFORMAT_ARGB8888,
                                             SDL_TEXTUREACCESS_STREAMING, 64, 32)};

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

            SDL_UpdateTexture(texture.get(), nullptr, framebuffer.data(), 64 * sizeof(uint32_t));
            SDL_RenderClear(renderer.get());
            SDL_RenderCopy(renderer.get(), texture.get(), nullptr, nullptr);
            SDL_RenderPresent(renderer.get());
        }
    }

    SDL_Quit();
    return 0;
}