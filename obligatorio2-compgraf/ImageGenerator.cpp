#include "ImageGenerator.h"

#include <ctime>
#include <string>

#include <SDL_image.h>
#include <SDL.h>
#include <iostream>

void ImageGenerator::generateImage(SDL_Renderer* renderer, int width, int height) {
    SDL_Surface* sceneRendering = SDL_CreateRGBSurface(0, width, height, 32, RED_MASK, GREEN_MASK, BLUE_MASK, ALPHA_MASK);
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, sceneRendering->pixels, sceneRendering->pitch);
    
    saveImage(sceneRendering);

    SDL_FreeSurface(sceneRendering);
}

const char* ImageGenerator::generateImageName()
{
    struct tm localTime;
    std::time_t now = std::time(nullptr);
    localtime_s(&localTime, &now);

    char timestamp[15];
    std::strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M%S", &localTime);
    
    std::string *imageName = new std::string("../resultados/");
    imageName->append(timestamp);
    imageName->append(".png");

    return imageName->c_str();
}

void ImageGenerator::saveImage(SDL_Surface* surface)
{
    const char* imageName = generateImageName();
    if (IMG_SavePNG(surface, imageName) != 0) {
        std::cout << IMG_GetError() << std::endl;
    }
    delete[] imageName;
}
