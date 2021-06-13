#include "Scene.h"

#include <iostream>

#include <SDL.h>
#include <pugixml.hpp>

#include "Ray.h"
#include "SceneObject.h"
#include "Sphere.h"

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

	Sphere* testSphere = new Sphere(500, glm::vec3(0, 0, -100));
	testSphere->color = Color();
	testSphere->color.red = 255;
	testSphere->color.blue = 255;
	testSphere->color.green = 0;
	testSphere->color.reflection = 0;
	testSphere->color.transmission = 0;
	objects.push_back(testSphere);

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
				glm::vec3(x, y, 0),
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
		CollisionPoint* hit = obj->intersects(ray);
		if (hit != nullptr) {
			//std::cout << "hit" << std::endl;
			intersections.push_back(hit);
		}
	}
	CollisionPoint* collisionPoint = getClosestObject(ray, intersections);
	if (collisionPoint != nullptr) {
		return shadow(collisionPoint, ray, depth);
	}
	else {
		return backgroudColor;
	}
}

Color Scene::shadow(CollisionPoint* hit, const Ray& ray, int depth)
{
	/*std::cout << "object color" << std::endl;
	std::cout << "red " << hit->object->color.red << std::endl;
	std::cout << "green " << hit->object->color.green << std::endl;
	std::cout << "blue " << hit->object->color.blue << std::endl << std::endl;*/

	Color color = getAmbientColor(*hit->object);

	float specularCoefficient;
	float transmissionCoefficient;

	for (PositionLight *light : lights) {
		glm::vec3 rayToLigth = hit->position - light->getPosition();

		if (glm::dot(hit->normal, rayToLigth) > 0) {
			/*
			* TODO:
			Calcular cuánta luz es bloqueada por sup. opacas y transp., y
			usarlo para escalar los términos difusos y especulares antes de
			añadirlos a color;
			*/
		}
	}

	if (depth < maxDepth) {
		if (hit->object->getReflectionCoefficient() > 0) {
			Ray reflectiveRay = getReflectiveRay(ray, hit->object->getReflectionCoefficient());
			Color reflectiveColor = rayTrace(reflectiveRay, depth + 1);
		}
		
		if (hit->object->getTransmissionCoefficient() > 0) {
			//TODO
		
		}
	}

	return hit->object->color;
}


CollisionPoint* Scene::getClosestObject(const Ray& ray, std::vector<CollisionPoint*> collisions)
{
	CollisionPoint* curClosest = nullptr;
	for (int i = 0; i < collisions.size(); i++)
	{
		if (curClosest == nullptr || collisions[i]->position.z < curClosest->position.z)
		{
			curClosest = collisions[i];
		}
	}
	return curClosest;
}

Color Scene::getAmbientColor(const SceneObject& object)
{
	return Color();
}

Ray Scene::getReflectiveRay(const Ray& ray, float reflectionCoefficient)
{
	return Ray();
}

Ray Scene::getTransmissionRay(const Ray& ray, float transmissionCoefficient)
{
	return Ray();
}
