#include "Scene.h"

#include <iostream>

#include <SDL.h>
#include <pugixml.hpp>

#include "Ray.h"
#include "SceneObject.h"

void Scene::loadSceneFromFile()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("../escena.xml");

	projectionCenter = glm::vec3(
		doc.child("projectionCenter").attribute("x").as_int(),
		doc.child("projectionCenter").attribute("y").as_int(),
		doc.child("projectionCenter").attribute("z").as_int()
	);

	std::cout << projectionCenter.x << "," << projectionCenter.y << "," << projectionCenter.z << std::endl;

	//obtener bk del xml
	backgroudColor.red = 0;
	backgroudColor.green = 0;
	backgroudColor.blue = 0;
	backgroudColor.reflection = 0;
	backgroudColor.transmission = 0;

}

void Scene::render(SDL_Renderer* renderer) {
	for (int y = 0; y < SCREEN_HEIGHT; ++y) {
		for (int x = 0; x < SCREEN_WIDTH; ++x) {
			Ray ray(
				glm::vec2(x, y),
				projectionCenter
			);

			Color pixel(rayTrace(ray, 1));

			SDL_SetRenderDrawColor(renderer, pixel.red, pixel.green, pixel.blue, SDL_ALPHA_OPAQUE);
			SDL_RenderDrawPoint(renderer, x, y);
		}
	}
}

Color Scene::rayTrace(const Ray& ray, int depth)
{
	std::vector<CollisionPoint*> intersections;
	for (auto &obj : objects) {
		CollisionPoint* hit = obj.intersects(ray);
		if (hit != nullptr) {
			intersections.push_back(hit);
		}
	}
	CollisionPoint* collisionPoint = getClosestObject(intersections);
	if (collisionPoint != nullptr) {
		return shadow(collisionPoint, ray, depth);
	}
	else {
		return backgroudColor;
	}
}

Color Scene::shadow(CollisionPoint* hit, const Ray& ray, int depth)
{
	return Color();
}


CollisionPoint* Scene::getClosestObject(std::vector<CollisionPoint*> collisions)
{
	return nullptr;
}
