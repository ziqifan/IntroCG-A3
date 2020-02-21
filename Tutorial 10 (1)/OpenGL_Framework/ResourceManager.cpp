#include "ResourceManager.h"

std::vector<Transform*> ResourceManager::Transforms;

void ResourceManager::addEntity(Transform * entity)
{
	Transforms.push_back(entity);
}
