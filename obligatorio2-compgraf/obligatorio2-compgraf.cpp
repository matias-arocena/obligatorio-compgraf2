#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

const int RED_MASK = 0x00ff0000;
const int GREEN_MASK = 0x0000ff00;
const int BLUE_MASK = 0x000000ff;
const int ALPHA_MASK = 0xff000000;
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Error inicializando SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cout << "Error inicializando SDL Image: " << IMG_GetError() << std::endl;
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Iluminacion Global", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_GetRenderer(window);

    SDL_Surface* sshot = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, RED_MASK, GREEN_MASK, BLUE_MASK, ALPHA_MASK);
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
    IMG_SavePNG(sshot, "image.png");

    SDL_FreeSurface(sshot);
    
    IMG_Quit();
    SDL_Quit();

    return 0;
}