#include "Scene.h"

#include <iostream>

#include <pugixml.hpp>

void Scene::loadSceneFromFile()
{
	pugi::xml_document doc;

	pugi::xml_parse_result result = doc.load_file("../escena.xml");

	std::cout << "Load result: " << result.description() << ", mesh name: " << doc.child("mesh").attribute("name").value() << std::endl;
}
