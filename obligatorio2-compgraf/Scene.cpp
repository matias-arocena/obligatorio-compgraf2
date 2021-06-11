#include "Scene.h"

#include <iostream>

#include <SDL.h>
#include <pugixml.hpp>

void Scene::loadSceneFromFile()
{
	pugi::xml_document doc;

	pugi::xml_parse_result result = doc.load_file("../escena.xml");

	std::cout << "Load result: " << result.description() << ", mesh name: " << doc.child("mesh").attribute("name").value() << std::endl;
}

void Scene::render(SDL_Renderer* renderer, int width, int height) {
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			SDL_SetRenderDrawColor(renderer, 255 * (x % 2) * (y % 2), 255 * (x % 2) * (y % 2), 255 * (x % 2) * (y % 2), SDL_ALPHA_OPAQUE);
			SDL_RenderDrawPoint(renderer, x, y);
		}
	}
}