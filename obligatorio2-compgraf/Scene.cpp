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

	Sphere* testSphere = new Sphere(.49f, glm::vec3(0.0, 1.0, 2.1f));
	testSphere->diffuse = Color();
	testSphere->diffuse.rgb = glm::vec3(1, 0, 0);
	testSphere->ambient = Color();
	testSphere->ambient.rgb = glm::vec3(1, 0, 0);
	testSphere->specular = Color();
	testSphere->specular.rgb = glm::vec3(1, 0, 0);
	testSphere->shininess = .0f;
	testSphere->alpha = 1.f;
	testSphere->reflectionCoefficient = 0.0f;
	testSphere->transmissionCoefficient = 0;
	objects.push_back(testSphere);

	Sphere* testSphere2 = new Sphere(.5f, glm::vec3(0.0, 0.75f, 1.f));
	testSphere2->diffuse = Color();
	testSphere2->diffuse.rgb = glm::vec3(1, 1, 1);
	testSphere2->ambient = Color();
	testSphere2->ambient.rgb = glm::vec3(1, 1, 1);
	testSphere2->specular = Color();
	testSphere2->specular.rgb = glm::vec3(1, 1, 1);
	testSphere2->shininess = .0f;
	testSphere2->alpha = 0.4f;
	testSphere2->reflectionCoefficient = 0.0f;
	testSphere2->transmissionCoefficient = 1.0f;

	objects.push_back(testSphere2);

	Sphere* world = new Sphere(105.f, glm::vec3(0.0, 110.0, 2.f));
	world->diffuse = Color();
	world->diffuse.rgb = glm::vec3(0, 1, 0);
	world->ambient = Color();
	world->ambient.rgb = glm::vec3(0, 1, 0);
	world->specular = Color();
	world->specular.rgb = glm::vec3(1, 0, 0);
	world->shininess = 0.f;
	world->alpha = 1.f;
	world->reflectionCoefficient = 0.f;
	world->transmissionCoefficient = 0;

	objects.push_back(world);

	Color white;
	white.rgb = glm::vec3(1, 1, 1);
	ambientLight = new Light(white, 0.1f);
	
	//obtener luz de xml
	PositionLight* l = new PositionLight(glm::vec3(0, -3, 0), white, 1);
	lights.push_back(l);

	//obtener bk del xml
	backgroudColor.rgb = glm::vec3(0, 0, 0);
	backgroudColor.reflection = 0;
	backgroudColor.transmission = 0;

	//TODO: obtener camara de xml
	camera = new Camera(glm::vec3(0, 0, 0) , 1.f, 2.f);

	maxDepth = 5;
}

void Scene::render(SDL_Renderer* renderer) {
	for (int y = 0; y < SCREEN_HEIGHT; ++y) {
		double v = (float)y / SCREEN_HEIGHT;
		for (int x = 0; x < SCREEN_WIDTH; ++x) {
			double u = (float)x / SCREEN_WIDTH;


			Ray ray(
				camera->getPosition(),
				camera->getDirectionToViewport(u, v)
			);

			Color pixel(rayTrace(ray, 1));

			// std::cout << "(" << x << "," << y << ") (" << u << "," << v << ") (" << pixel.rgb.r << "," << pixel.rgb.g << "," << pixel.rgb.b << ")" << std::endl;
			SDL_SetRenderDrawColor(renderer, 255 * pixel.rgb.r, 255 * pixel.rgb.g, 255 * pixel.rgb.b, SDL_ALPHA_OPAQUE);
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
		return shadow(collisionPoint, ray, depth);
	}
	else {

		glm::vec3 rgb = backgroudColor.rgb;
		Color color;
		color.rgb = rgb;
		return color;
	}
}

Color Scene::shadow(const CollisionPoint* hit, const Ray& ray, int depth)
{
	Color diffuse;
	diffuse.rgb = glm::vec3(0, 0, 0);
	Color specular;
	specular.rgb = glm::vec3(0, 0, 0);
	float brightness = 0;
	for (PositionLight* light : lights) {


		Ray rayToLight(hit->position, light->getPosition() - hit->position);
		float modifiedIntensity = light->getIntensity();

		if (glm::dot(hit->normal, rayToLight.getDirection()) > 0) {
			bool in_shade = false;
			bool test = false;
			for (auto& obj : objects) 
			{
				CollisionPoint* otherHit = obj->intersects(rayToLight);
				if (otherHit != nullptr && otherHit->distanceFromOrigin < glm::length(light->getPosition() - otherHit->position)) 
				{
					std::cout << "hit " << hit->position.x << " " << hit->position.y << " " << hit->position.z << std::endl;

					if (otherHit->object != hit->object &&
						otherHit->object->alpha == 1.f)
					{
						std::cout << "object b " << otherHit->object->diffuse.rgb.b << std::endl;
						in_shade = true;
						break;
					}
					else
					{
						std::cout << "modified intensity " << std::endl;
						modifiedIntensity *= 1 - otherHit->object->alpha;
						test = true;
					}
				}
			}

			if (!in_shade) {
				float angle = std::asin(glm::dot(hit->normal, rayToLight.getDirection()));
				if (angle > 0) 
				{
					if (test)
						std::cout << "intensity: " << modifiedIntensity << std::endl;
					diffuse.rgb += angle * modifiedIntensity * hit->object->diffuse.rgb;
					/*if (hit->object->getReflectionCoefficient() == 0) brightness += angle * modifiedIntensity; //TODO: diffuse coef?*/

					glm::vec3 reflected_light = glm::normalize(getReflectiveVector(rayToLight.getDirection(), hit->normal));
					specular.rgb +=  hit->object->getReflectionCoefficient() * 
									std::pow(std::max(0.f, glm::dot(reflected_light, (-hit->hitDir))), hit->object->getShininess()) *
									modifiedIntensity * 0.3f * 
									glm::vec3(1,1,1);
				}
			}
		}
	}
	Color result;
	result.rgb = glm::vec3(0, 0, 0);
	if (depth < maxDepth && hit->object->alpha < 1 && (hit->object->getReflectionCoefficient() > 0 || hit->object->getTransmissionCoefficient() > 0)) {
		float kr = 1;
		fresnel(ray.getDirection(), hit->normal, hit->object->getTransmissionCoefficient(), kr);

		Color reflectiveColor;
		if (hit->object->getReflectionCoefficient() > 0) {
			std::cout << "reflecting" << std::endl;
			Ray reflectiveRay = getReflectiveRay(ray, *hit);
			reflectiveColor = rayTrace(reflectiveRay, depth + 1);
		}

		Color transmissionColor;
		transmissionColor.rgb = glm::vec3(0, 0, 0);
		if (hit->object->getTransmissionCoefficient() > 0) {
			if (kr < 1) {
				//std::cout << "refracting" << std::endl;
				Ray refractionRay = getTransmissionRay(ray, *hit);
				transmissionColor = rayTrace(refractionRay, depth + 1);

			}
		}

		result.rgb = (reflectiveColor.rgb * kr + transmissionColor.rgb * (1 - kr)) * (1 - hit->object->alpha);
	}

	result.rgb += (hit->object->ambient.rgb * ambientLight->getIntensity() + diffuse.rgb + specular.rgb) * hit->object->alpha;
	result.rgb.r = result.rgb.r * (result.rgb.r < 1.f) + (result.rgb.r >= 1);
	result.rgb.g = result.rgb.g * (result.rgb.g < 1.f) + (result.rgb.g >= 1);
	result.rgb.b = result.rgb.b * (result.rgb.b < 1.f) + (result.rgb.b >= 1);

	return result;
	
		

}

void Scene::fresnel(const glm::vec3& I, const glm::vec3& N, const float& ior, float& kr)
{
	float cosi = glm::clamp(-1.f, 1.f, glm::dot(I, N));
	float etai = 1, etat = ior;
	if (cosi > 0) { std::swap(etai, etat); }
	// Compute sini using Snell's law
	float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
	// Total internal reflection
	if (sint >= 1) {
		kr = 1;
	}
	else {
		float cost = sqrtf(std::max(0.f, 1 - sint * sint));
		cosi = fabsf(cosi);
		float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
		float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
		kr = (Rs * Rs + Rp * Rp) / 2;
	}
	// As a consequence of the conservation of energy, transmittance is given by:
	// kt = 1 - kr;
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
	return Ray(hit.position, hit.hitDir - 2 * glm::dot(hit.hitDir, hit.normal) * hit.normal);
//	return Ray(glm::vec3(std::cos(hit.angle) / hit.normal.x, std::cos(hit.angle) / hit.normal.y, std::cos(hit.angle) / hit.normal.z) + hit.position, hit.position);
}

glm::vec3 Scene::getReflectiveVector(const glm::vec3 vec, const glm::vec3 axis)
{
	return 2 * (glm::dot(vec, axis)) * axis - vec;
}

Ray Scene::getTransmissionRay(const Ray& ray, const CollisionPoint& hit)
{
	float cosi = glm::clamp(-1.f, 1.f, glm::dot(hit.hitDir, hit.normal));
	float etai = 1, etat = hit.object->getTransmissionCoefficient();
	glm::vec3 n = hit.normal;
	if (cosi < 0) { cosi = -cosi; }
	else { std::swap(etai, etat); n = -hit.normal; }
	float eta = etai / etat;
	float k = 1 - eta * eta * (1 - cosi * cosi);
	glm::vec3 refractionDirection = k < 0 ? glm::vec3(0, 0, 0) : eta * hit.hitDir + (eta * cosi - sqrtf(k)) * n;

	bool outside = glm::dot(hit.hitDir, hit.normal) < 0;
	glm::vec3 bias = 0.1f * hit.normal;

	glm::vec3 refractionRayOrig = outside ? hit.position - bias : hit.position + bias;

	return Ray(refractionRayOrig, refractionDirection);
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
