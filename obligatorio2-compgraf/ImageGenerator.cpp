#include "ImageGenerator.h"

#include <ctime>
#include <string>
#include <filesystem>

#include <SDL_image.h>
#include <SDL.h>
#include <iostream>

void ImageGenerator::generateImage(SDL_Surface* surface, std::string imageName, int width, int height) {
    
    std::string dirName = generateName();
    std::filesystem::create_directory(dirName);
    
    std::string name = dirName + "/" + imageName + ".png";

    saveImage(surface, name.c_str());

    SDL_FreeSurface(surface);
}

std::string ImageGenerator::generateName()
{
    struct tm localTime;
    std::time_t now = std::time(nullptr);
    localtime_s(&localTime, &now);

    char timestamp[15];
    std::strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M%S", &localTime);
    
    std::string imageName = std::string("../resultados/");
    imageName.append(timestamp);

    return imageName;
}

void ImageGenerator::saveImage(SDL_Surface* surface, const char* imageName)
{
    if (IMG_SavePNG(surface, imageName) != 0) {
        std::cout << IMG_GetError() << std::endl;
    }
}
