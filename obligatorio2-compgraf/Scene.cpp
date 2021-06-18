#include "Scene.h"

#include <iostream>
#include <algorithm>   

#include <SDL.h>
#include <pugixml.hpp>
#include <glm/glm.hpp>

#include "Ray.h"
#include "SceneObject.h"
#include "Sphere.h"

Scene::Scene()
{
}

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

	Sphere* testSphere = new Sphere(.5f, glm::vec3(0, 0, 1));
	testSphere->ambient = Color();
	testSphere->ambient.rgb.r = 255;
	testSphere->ambient.rgb.g = 255;
	testSphere->ambient.rgb.b = 0;
	testSphere->ambient.reflection = 0;
	testSphere->ambient.transmission = 0;

	testSphere->specular = Color();
	testSphere->specular.rgb.r = 255;
	testSphere->specular.rgb.g = 255;
	testSphere->specular.rgb.b = 0;
	testSphere->specular.reflection = 0;
	testSphere->specular.transmission = 0;

	testSphere->diffuse = Color();
	testSphere->diffuse.rgb.r = 255;
	testSphere->diffuse.rgb.g = 255;
	testSphere->diffuse.rgb.b = 0;
	testSphere->diffuse.reflection = 0;
	testSphere->diffuse.transmission = 0;

	testSphere->color = Color();
	testSphere->color.rgb = glm::vec3(255, 0, 255);

	objects.push_back(testSphere);

	Color white;
	white.rgb = glm::vec3(255, 255, 255);
	ambientLight = new Light(white, 0.3f);
	
	//obtener luz de xml
	PositionLight* l = new PositionLight(glm::vec3(100,100,100), white, 1);
	lights.push_back(l);

	//obtener bk del xml
	backgroudColor.rgb = glm::vec3(255, 255, 0);
	backgroudColor.reflection = 0;
	backgroudColor.transmission = 0;

	//TODO: obtener camara de xml
	camera = new Camera(glm::vec3(0, 0, 0) , 1.f, 1.f);

	maxDepth = 2;
}

void Scene::render(SDL_Renderer* renderer) {
	for (int y = 0; y < SCREEN_HEIGHT; ++y) {
		for (int x = 0; x < SCREEN_WIDTH; ++x) {
			double u = (double)x / SCREEN_WIDTH;
			double v = (double)y / SCREEN_HEIGHT;
			Ray ray(
				camera->getPosition(),
				camera->getDirectionToViewport(u, v)
			);

			Color pixel(rayTrace(ray, 1));

			SDL_SetRenderDrawColor(renderer, pixel.rgb[0], pixel.rgb[1], pixel.rgb[2], SDL_ALPHA_OPAQUE);
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
			intersections.push_back(hit);
		}
	}
	CollisionPoint* collisionPoint = getClosestObject(ray, intersections);
	if (collisionPoint != nullptr) {
//		return shadow(collisionPoint, ray, depth);
		return collisionPoint->object->color;
	}
	else {
		return backgroudColor;
	}
}

Color Scene::shadow(CollisionPoint* hit, const Ray& ray, int depth)
{

	Color ambient;
	//	ambient.rgb = hit->object->ambient.rgb + ambientLight->getColor().rgb * ambientLight->getIntensity();
	ambient.rgb = hit->object->ambient.rgb;
	Color diffuse;
	diffuse.rgb = hit->object->diffuse.rgb;
	Color specular;
	specular.rgb = hit->object->specular.rgb;
	Color transparency;
	transparency.rgb = glm::vec3(0, 0, 0);

	float ambientK = 0;
	float diffuseK = 0;
	float specularK = 0;

	double reflectionCoefficient = 1.0;
	double transmissionCoefficient = 1.0;
/*
	for (PositionLight *light : lights) {
		Ray rayToLight(light->getPosition(), hit->position);

		if (glm::dot(hit->normal, rayToLight.getDir()) > 0) {
			bool intersects = false;
			for (auto& obj : objects) {
				CollisionPoint* hit = obj->intersects(rayToLight);
				if (hit != nullptr && hit->distanceFromOrigin < glm::length(light->getPosition() - hit->position)) {
					intersects = true;
					break;
				}
			}					
			if (!intersects) {
				diffuseK += light->getIntensity() * std::max(0.f, glm::dot(hit->normal, -rayToLight.getDir()));

				//Ver en nuestro caso el valor de n
				int n = 10;
				glm::vec3 V = glm::normalize(camera->getPosition() -hit->position);
				glm::vec3 R = glm::normalize(2 * glm::dot(hit->normal, rayToLight.getDir()) * hit->normal - rayToLight.getDir());
				
				specularK += std::max(0.f, (float)pow(glm::dot(V, R), n));

			}

		}
	}
	if (depth < maxDepth) {
		if (hit->object->getReflectionCoefficient() > 0) {
			Ray reflectiveRay = getReflectiveRay(ray, *hit);
			Color reflectiveColor = rayTrace(reflectiveRay, depth + 1);
				
			specular.rgb += reflectiveColor.rgb * hit->object->getReflectionCoefficient();
		}
		
		if (hit->object->getTransmissionCoefficient() > 0) {
			//TODO: Ver si necesitamos considerar 2 objetos intersectandose 
			if (!isTotalInternalReflection(hit->angle, AIR, hit->object->getMedium())) {
				Ray transmissionRay = getTransmissionRay(ray, *hit);
				Color transmissionColor = rayTrace(transmissionRay, depth + 1);
				//TODO: Donde va la transparencia?
				transparency.rgb += transmissionColor.rgb * hit->object->getReflectionCoefficient();
			}
		}
	}

	float red = ambient.rgb[0] * ambientK / (ambientK + diffuseK + specularK) + diffuse.rgb[0] * diffuseK / (ambientK + diffuseK + specularK) + specular.rgb[0] * specularK / (ambientK + diffuseK + specularK);
	float green = ambient.rgb[1] * ambientK / (ambientK + diffuseK + specularK) + diffuse.rgb[1] * diffuseK / (ambientK + diffuseK + specularK) + specular.rgb[1] * specularK / (ambientK + diffuseK + specularK);
	float blue = ambient.rgb[2] * ambientK / (ambientK + diffuseK + specularK) + diffuse.rgb[2] * diffuseK / (ambientK + diffuseK + specularK) + specular.rgb[2] * specularK / (ambientK + diffuseK + specularK);
	*/


	Color result;
	//result.rgb = glm::vec3(red, green, blue);
	result.rgb = glm::vec3(0, 0, 0);
	result.reflection = hit->object->getReflectionCoefficient();
	result.transmission = hit->object->getTransmissionCoefficient();
	return result;

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

Ray Scene::getReflectiveRay(const Ray& ray, const CollisionPoint& hit)
{
	return Ray(glm::vec3(std::cos(hit.angle) / hit.normal.x, std::cos(hit.angle) / hit.normal.y, std::cos(hit.angle) / hit.normal.z) + hit.position, hit.position);
}

// TODO
Ray Scene::getTransmissionRay(const Ray& ray, const CollisionPoint& hit)
{
	float angle = std::asin((AIR * std::sin(hit.angle)) / hit.object->getMedium());
	return Ray(-glm::vec3(std::cos(angle) / hit.normal.x, std::cos(angle) / hit.normal.y, std::cos(angle) / hit.normal.z) + hit.position, hit.position);
}

bool Scene::isTotalInternalReflection(double incidenceAngle, double outMediumCoefficient, double inMediumCoefficient)
{
	if (outMediumCoefficient < inMediumCoefficient)
		return false;
	return incidenceAngle > std::asin(inMediumCoefficient/outMediumCoefficient);
}

bool distanceComparator(const CollisionPoint& a, const CollisionPoint& b) {
	return a.distanceFromOrigin < b.distanceFromOrigin;
}
