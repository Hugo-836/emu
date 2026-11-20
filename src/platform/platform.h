#pragma once
#include <SDL.h>
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

// Initialise SDL à la construction et l'arrête à la destruction
struct SdlContext {
    SdlContext()  { SDL_Init(SDL_INIT_VIDEO); }
    ~SdlContext() { SDL_Quit(); }
};

class Platform {
public:
    Platform(const char* title, int width, int height, int scale);

    Platform(const Platform&) = delete;            // pas de copie
    Platform& operator=(const Platform&) = delete;

    bool processEvents();                 // renvoie false si on ferme la fenêtre
    void draw(const uint32_t* pixels);    // affiche un framebuffer

private:
    int width_;
    SdlContext sdl_;          // déclaré EN PREMIER → détruit EN DERNIER
    WindowPtr window_;
    RendererPtr renderer_;
    TexturePtr texture_;
};