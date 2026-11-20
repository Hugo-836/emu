#include "Platform.h"

Platform::Platform(const char* title, int width, int height, int scale)
    : width_{width},
      sdl_{},
      window_{SDL_CreateWindow(
          title,
          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
          width * scale, height * scale,
          0)},
      renderer_{SDL_CreateRenderer(window_.get(), -1, SDL_RENDERER_PRESENTVSYNC)},
      texture_{SDL_CreateTexture(renderer_.get(), SDL_PIXELFORMAT_ARGB8888,
                                 SDL_TEXTUREACCESS_STREAMING, width, height)}
{
}

bool Platform::processEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return false;
        }
    }
    return true;
}

void Platform::draw(const uint32_t* pixels) {
    SDL_UpdateTexture(texture_.get(), nullptr, pixels, width_ * sizeof(uint32_t));
    SDL_RenderClear(renderer_.get());
    SDL_RenderCopy(renderer_.get(), texture_.get(), nullptr, nullptr);
    SDL_RenderPresent(renderer_.get());
}