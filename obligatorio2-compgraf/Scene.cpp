#include "Scene.h"

#include <iostream>
#include <algorithm>   
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

#include <SDL.h>
#include <glm/glm.hpp>


#include "Ray.h"
#include "SceneObject.h"
#include "Sphere.h"
#include "Cilinder.h"
#include "Plane.h"
#include "Box.h"

Scene::Scene()
{
}

void Scene::loadSceneFromFile()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("../escena.xml");

	std::vector<double> cameraPosition = getXmlVector(doc.child("scene").child("camera").attribute("position"));
	std::vector<double> cameraLookAt = getXmlVector(doc.child("scene").child("camera").attribute("lookAt"));
	std::vector<double> vup = getXmlVector(doc.child("scene").child("camera").attribute("vup"));

	camera = new Camera(
		glm::dvec3(
			cameraPosition[0],
			cameraPosition[1],
			cameraPosition[2]
		),
		glm::dvec3(
			cameraLookAt[0],
			cameraLookAt[1],
			cameraLookAt[2]
		),
		glm::dvec3(
			vup[0],
			vup[1],
			vup[2]
		),
		doc.child("scene").child("camera").attribute("fov").as_double()
	);

	auto xmlObjects = doc.child("scene").child("objects").children();
	for (auto obj : xmlObjects) {
		SceneObject *object;

		std::string type = obj.attribute("type").as_string();
		std::vector<double> positionArray = getXmlVector(obj.attribute("position"));
		glm::dvec3 position = glm::dvec3(
			positionArray[0],
			positionArray[1],
			positionArray[2]
		);
		if (type == "sphere") {
			object = new Sphere(
				obj.attribute("radius").as_double(),
				position
			);
		}
		else if (type == "cilinder") {
			object = new Cilinder(
				obj.attribute("radius").as_double(), 
				obj.attribute("height").as_double(), 
				glm::vec3(position)
			);

		}
		else if (type == "plane") {
			std::vector<double> normal = getXmlVector(obj.attribute("normal"));
			std::vector<double> up = getXmlVector(obj.attribute("up"));
			
			object = new Plane(
				glm::dvec3(
					normal[0],
					normal[1],
					normal[2]
				),
				position,
				glm::dvec3(
					up[0],
					up[1],
					up[2]
				),
				obj.attribute("length").as_double(),
				obj.attribute("height").as_double()
			);
		}
		else if (type == "box") {
			std::vector<double> up = getXmlVector(obj.attribute("up"));
			std::vector<double> front = getXmlVector(obj.attribute("front"));

			object = new Box(
				position,
				glm::dvec3(
					up[0],
					up[1],
					up[2]
				),
				glm::dvec3(
					front[0],
					front[1],
					front[2]
				),
				obj.attribute("height").as_double(),
				obj.attribute("length").as_double(),
				obj.attribute("width").as_double()
			);

			std::vector<double> ambient = getXmlVector(obj.attribute("ambient"));
			std::vector<double> diffuse = getXmlVector(obj.attribute("diffuse"));
			std::vector<double> specular = getXmlVector(obj.attribute("specular"));

			dynamic_cast<Box*>(object)->setData(
				glm::vec3(ambient[0], ambient[1], ambient[2]),
				glm::vec3(diffuse[0], diffuse[1], diffuse[2]),
				glm::vec3(specular[0], specular[1], specular[2]),
				obj.attribute("shininess").as_double(),
				obj.attribute("alpha").as_double(),
				obj.attribute("reflectionCoefficient").as_double(),
				obj.attribute("transmissionCoefficient").as_double()
			);
		}
		else {
			object = new Sphere(
				0.5,
				glm::dvec3(0,0,0)
			);
		}

		if (type != "box") {
			std::vector<double> ambient = getXmlVector(obj.attribute("ambient"));
			object->ambient.rgb = glm::dvec3(
				ambient[0],
				ambient[1],
				ambient[2]
			);

			std::vector<double> diffuse = getXmlVector(obj.attribute("diffuse"));
			object->diffuse.rgb = glm::dvec3(
				diffuse[0],
				diffuse[1],
				diffuse[2]
			);

			std::vector<double> specular = getXmlVector(obj.attribute("specular"));
			object->specular.rgb = glm::dvec3(
				specular[0],
				specular[1],
				specular[2]
			);

			object->shininess = obj.attribute("shininess").as_double();
			object->alpha = obj.attribute("alpha").as_double();
			object->reflectionCoefficient = obj.attribute("reflectionCoefficient").as_double();
			object->transmissionCoefficient = obj.attribute("transmissionCoefficient").as_double();

		}

		objects.push_back(object);
	}
	

	Color white;
	white.rgb = glm::dvec3(1, 1, 1);

	auto xmlLights = doc.child("scene").child("lights").children();
	for (auto l : xmlLights) {
		std::string type = l.attribute("type").as_string();
		if (type == "ambient") {
			ambientLight = new Light(white, l.attribute("intensity").as_double());
		}
		else {
			std::vector<double> position = getXmlVector(l.attribute("position"));
			PositionLight* light = new PositionLight(
				glm::dvec3(
					position[0],
					position[1],
					position[2]
				),
				white,
				l.attribute("intensity").as_double()
			);
			lights.push_back(light);
		}
	}
	
	std::vector<double> bkColor = getXmlVector(doc.child("scene").child("background").attribute("color"));
	backgroudColor.rgb = glm::vec3(
		bkColor[0],
		bkColor[1],
		bkColor[2]
	);
	backgroudColor.reflection = 0;
	backgroudColor.transmission = 0;
	
	maxDepth = doc.child("scene").child("raytrace").attribute("depth").as_int();
}

std::vector<double> Scene::getXmlVector(pugi::xml_attribute attr)
{
	double i;
	std::istringstream in(attr.as_string());
	std::vector<double> vec;
	while (in >> i) vec.push_back(i);
	return vec;
}

void Scene::render(SDL_Renderer* renderer, SDL_Renderer* reflectionRenderer, SDL_Renderer* transimssionRenderer, SDL_Renderer* ambientRenderer, SDL_Renderer* diffuseRenderer, SDL_Renderer* specularRenderer) {
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

			SDL_SetRenderDrawColor(ambientRenderer, static_cast<Uint8>(255 * pixel.ambient.r), static_cast<Uint8>(255 * pixel.ambient.g), static_cast<Uint8>(255 * pixel.ambient.b), SDL_ALPHA_OPAQUE);
			SDL_RenderDrawPoint(ambientRenderer, x, y);

			SDL_SetRenderDrawColor(diffuseRenderer, static_cast<Uint8>(255 * pixel.diffuse.r), static_cast<Uint8>(255 * pixel.diffuse.g), static_cast<Uint8>(255 * pixel.diffuse.b), SDL_ALPHA_OPAQUE);
			SDL_RenderDrawPoint(diffuseRenderer, x, y);

			SDL_SetRenderDrawColor(specularRenderer, static_cast<Uint8>(255 * pixel.specular.r), static_cast<Uint8>(255 * pixel.specular.g), static_cast<Uint8>(255 * pixel.specular.b), SDL_ALPHA_OPAQUE);
			SDL_RenderDrawPoint(specularRenderer, x, y);
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
		color.ambient = glm::vec3(0, 0, 0);
		color.diffuse = glm::vec3(0, 0, 0);
		color.specular = glm::vec3(0, 0, 0);
		color.transmission = 0;
		color.reflection = 0;
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
					//if (test)
						//std::cout << "intensity: " << modifiedIntensity << std::endl;
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

	result.ambient = hit->object->ambient.rgb;
	result.ambient.r = result.ambient.r * (result.ambient.r < 1.f) + (result.ambient.r >= 1);
	result.ambient.g = result.ambient.g * (result.ambient.g < 1.f) + (result.ambient.g >= 1);
	result.ambient.b = result.ambient.b * (result.ambient.b < 1.f) + (result.ambient.b >= 1);

	result.diffuse = diffuse.rgb;
	result.diffuse.r = result.diffuse.r * (result.diffuse.r < 1.f) + (result.diffuse.r >= 1);
	result.diffuse.g = result.diffuse.g * (result.diffuse.g < 1.f) + (result.diffuse.g >= 1);
	result.diffuse.b = result.diffuse.b * (result.diffuse.b < 1.f) + (result.diffuse.b >= 1);

	result.specular = specular.rgb;
	result.specular.r = result.specular.r * (result.specular.r < 1.f) + (result.specular.r >= 1);
	result.specular.g = result.specular.g * (result.specular.g < 1.f) + (result.specular.g >= 1);
	result.specular.b = result.specular.b * (result.specular.b < 1.f) + (result.specular.b >= 1);

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
