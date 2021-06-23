#include "Scene.h"

#include <iostream>
#include <algorithm>   

#include <SDL.h>
#include <pugixml.hpp>
#include <glm/glm.hpp>


#include "Ray.h"
#include "SceneObject.h"
#include "Sphere.h"
#include "Cilinder.h"
#include "Plane.h"

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
	Sphere* testSphere = new Sphere(.5f, glm::vec3(1.0, 1.0, 1.f));
	testSphere->diffuse = Color();
	testSphere->diffuse.rgb = glm::vec3(1, 0, 0);
	testSphere->ambient = Color();
	testSphere->ambient.rgb = glm::vec3(1, 0, 0);
	testSphere->specular = Color();
	testSphere->specular.rgb = glm::vec3(1, 0, 0);
	testSphere->shininess = .3;
	testSphere->alpha = 1;
	testSphere->reflectionCoefficient = 0.f;
	testSphere->transmissionCoefficient = 0;
	objects.push_back(testSphere);

	/*
	Sphere* testSphere2 = new Sphere(.5f, glm::vec3(-1.0, 1.f, 1.f));
	testSphere2->diffuse = Color();
	testSphere2->diffuse.rgb = glm::vec3(1, 1, 1);
	testSphere2->ambient = Color();
	testSphere2->ambient.rgb = glm::vec3(1, 1, 1);
	testSphere2->specular = Color();
	testSphere2->specular.rgb = glm::vec3(1, 1, 1);
	testSphere2->shininess = .0f;
	testSphere2->alpha = .4f;
	testSphere2->reflectionCoefficient = 0.0f;
	testSphere2->transmissionCoefficient = 1.0;

	Cilinder* testCilinder = new Cilinder(.5, .5, glm::vec3(0.0, 1.0, 2.1));
	testCilinder->diffuse = Color();
	testCilinder->diffuse.rgb = glm::vec3(1, 0, 0);
	testCilinder->ambient = Color();
	testCilinder->ambient.rgb = glm::vec3(1, 0, 0);
	testCilinder->specular = Color();
	testCilinder->specular.rgb = glm::vec3(1, 0, 0);
	testCilinder->shininess = .3;
	testCilinder->alpha = 1.0;
	testCilinder->reflectionCoefficient = 1.0f;
	testCilinder->transmissionCoefficient = 0;
	objects.push_back(testCilinder);

	*/

	Color white;
	white.rgb = glm::dvec3(1, 1, 1);
	ambientLight = new Light(white, .3f);
	
	//obtener luz de xml
	PositionLight* l1 = new PositionLight(glm::vec3(2, -1, -1), white, 1);
	lights.push_back(l1);
	
	//obtener bk del xml
	backgroudColor.rgb = glm::vec3(0.8,0.8, 1);
	backgroudColor.reflection = 0;
	backgroudColor.transmission = 0;

	//TODO: obtener camara de xml
	camera = new Camera(glm::dvec3(0, 0, -10), glm::dvec3(0, 2, 0), glm::vec3(0, 1, 0), 90, 1.0, 2.0);
	//camera = new Camera(glm::vec3(0, 0, 0) , 1.f, 2.f);

	maxDepth = 10;
}

void Scene::render(SDL_Renderer* renderer, SDL_Renderer* reflectionRenderer, SDL_Renderer* transimssionRenderer) {
	for (int y = 0; y < SCREEN_HEIGHT; ++y) {
		for (int x = 0; x < SCREEN_WIDTH; ++x) {

			Ray ray(
				camera->getPosition(),
				camera->getDirectionToViewport(x -1, y-1),
				VACUUM
			);

			Color pixel(rayTrace(ray, 1));

			SDL_SetRenderDrawColor(renderer, static_cast<Uint8>(255 * pixel.rgb.r), static_cast<Uint8>(255 * pixel.rgb.g), static_cast<Uint8>(255 * pixel.rgb.b), SDL_ALPHA_OPAQUE);
			SDL_RenderDrawPoint(renderer, x, y);

			SDL_SetRenderDrawColor(reflectionRenderer, static_cast<Uint8>(255 * pixel.reflection), static_cast<Uint8>(255 * pixel.reflection), static_cast<Uint8>(255 * pixel.reflection), SDL_ALPHA_OPAQUE);
			SDL_RenderDrawPoint(reflectionRenderer, x, y);
			
			SDL_SetRenderDrawColor(transimssionRenderer, static_cast<Uint8>(255 * pixel.transmission), static_cast<Uint8>(255 * pixel.transmission), static_cast<Uint8>(255 * pixel.transmission), SDL_ALPHA_OPAQUE);
			SDL_RenderDrawPoint(transimssionRenderer, x, y);
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
		return shadow(collisionPoint, ray, depth);
	}
	else {
		glm::dvec3 rgb = backgroudColor.rgb;
		Color color;
		color.rgb = rgb;
		return color;
	}
}

Color Scene::shadow(const CollisionPoint* hit, const Ray& ray, int depth)
{   
	Color diffuse;
	diffuse.rgb = glm::dvec3(0, 0, 0);
	Color specular;
	specular.rgb = glm::dvec3(0, 0, 0);
	float brightness = 0;
	for (PositionLight* light : lights) {
		Ray rayToLight(hit->position, light->getPosition() - hit->position, ray.getMedium());
		double modifiedIntensity = light->getIntensity();

		if (glm::dot(hit->normal, rayToLight.getDirection()) > 0) {
			bool in_shade = false;
			bool test = false;
			for (auto& obj : objects) 
			{
				CollisionPoint* otherHit = obj->intersects(rayToLight);
				if (otherHit != nullptr && otherHit->distanceFromOrigin < glm::length(light->getPosition() - otherHit->position)) 
				{
					if (otherHit->object != hit->object &&
						otherHit->object->alpha == 1.f)
					{
						in_shade = true;
						break;
					}
					else
					{
						modifiedIntensity *= 1 - otherHit->object->alpha;
						test = true;
					}
				}
			}

			if (!in_shade) {
				double angle = std::asin(glm::dot(hit->normal, rayToLight.getDirection()));
				if (angle > 0)
				{
					if (test)
						std::cout << "intensity: " << modifiedIntensity << std::endl;
					diffuse.rgb += angle * modifiedIntensity * hit->object->diffuse.rgb;
					/*if (hit->object->getReflectionCoefficient() == 0) brightness += angle * modifiedIntensity; //TODO: diffuse coef?*/

					glm::dvec3 reflected_light = glm::normalize(getReflectiveVector(rayToLight.getDirection(), hit->normal));
					specular.rgb +=  hit->object->getReflectionCoefficient() * 
									std::pow(std::max(0.0, glm::dot(reflected_light, (-hit->hitDir))), hit->object->getShininess()) *
									modifiedIntensity * 0.3 * 
									glm::dvec3(1,1,1);
				}
			}
		}
	}
	Color result;
	result.rgb = glm::dvec3(0, 0, 0);
	if (depth < maxDepth && hit->object->alpha < 1 && (hit->object->getReflectionCoefficient() > 0 || hit->object->getTransmissionCoefficient() > 0)) {
		double kr = fresnel(ray.getDirection(), hit->normal, ray.getMedium(), hit->object->getTransmissionCoefficient());

		Color reflectiveColor;
		reflectiveColor.rgb = glm::dvec3(0, 0, 0);
		if (hit->object->getReflectionCoefficient() > 0) {
			Ray reflectiveRay = getReflectiveRay(ray, *hit);
			reflectiveColor = rayTrace(reflectiveRay, depth + 1);
			reflectiveColor.reflection = hit->object->reflectionCoefficient;
		}

		Color transmissionColor;
		transmissionColor.rgb = glm::dvec3(0, 0, 0);
		if (hit->object->getTransmissionCoefficient() > 0) {
			if (kr < 1) {
				Ray refractionRay = getTransmissionRay(ray, *hit);
				transmissionColor = rayTrace(refractionRay, depth + 1);
				transmissionColor.transmission = hit->object->alpha;
			}
		}

		result.rgb = (reflectiveColor.rgb * kr + transmissionColor.rgb * (1 - kr)) * (1 - hit->object->alpha);
		result.transmission = transmissionColor.transmission;
		result.reflection = reflectiveColor.reflection;
	}

	result.rgb += (hit->object->ambient.rgb * ambientLight->getIntensity() + diffuse.rgb + specular.rgb) * hit->object->alpha;
	result.rgb.r = result.rgb.r * (result.rgb.r < 1.f) + (result.rgb.r >= 1);
	result.rgb.g = result.rgb.g * (result.rgb.g < 1.f) + (result.rgb.g >= 1);
	result.rgb.b = result.rgb.b * (result.rgb.b < 1.f) + (result.rgb.b >= 1);

	return result;
}

double Scene::fresnel(const glm::dvec3& direction, const glm::dvec3& normal, const double& inTransmissionCoefficient, const double& outTransmissionCoefficient)
{
	double cosi = glm::clamp(-1.0, 1.0, glm::dot(direction, normal));
	double etai = inTransmissionCoefficient;
	double etat = outTransmissionCoefficient;
	if (cosi > 0) { 
		std::swap(etai, etat); 
	}
	// Compute sini using Snell's law
	double sint = etai / etat * sqrt(std::max(0.0, 1 - cosi * cosi));
	// Total internal reflection
	if (sint >= 1) {
		return 1.0;
	}
	else {
		double cost = sqrt(std::max(0.0, 1 - sint * sint));
		cosi = abs(cosi);
		double Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
		double Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
		return (Rs * Rs + Rp * Rp) / 2;
	}
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
	return Ray(hit.position, hit.hitDir - 2 * glm::dot(hit.hitDir, hit.normal) * hit.normal, ray.getMedium());
}

glm::vec3 Scene::getReflectiveVector(const glm::vec3 vec, const glm::vec3 axis)
{
	return 2 * (glm::dot(vec, axis)) * axis - vec;
}

Ray Scene::getTransmissionRay(const Ray& ray, const CollisionPoint& hit)
{
	bool outside = glm::dot(hit.hitDir, hit.normal) < 0;
	glm::dvec3 bias = 0.01 * hit.normal;

	glm::dvec3 refractionRayOrig = outside ? hit.position - bias : hit.position + bias;

	double cosIncidenceNormal = glm::clamp(-1.0, 1.0, glm::dot(-hit.normal, glm::normalize(hit.hitDir)));
	double incidenceRefraction = ray.getMedium();
	double outRefraction = hit.object->getTransmissionCoefficient();
	
	glm::dvec3 normal = -hit.normal;
	if (cosIncidenceNormal < 0) { 
		cosIncidenceNormal = -cosIncidenceNormal; 
	}
	else {
		//std::swap(incidenceRefraction, outRefraction); 
		normal = -hit.normal; 
	}
	double eta = incidenceRefraction / outRefraction;
	double k = 1 - eta * eta * (1 - cosIncidenceNormal * cosIncidenceNormal);
	glm::dvec3 refractionDirection = k < 0 ? glm::dvec3(0, 0, 0) : eta * glm::normalize(hit.hitDir) + (eta * cosIncidenceNormal - sqrt(k)) * normal;

	return Ray(refractionRayOrig, refractionDirection, hit.object->getMedium());
}

bool distanceComparator(const CollisionPoint& a, const CollisionPoint& b) {
	return a.distanceFromOrigin < b.distanceFromOrigin;
}
