#pragma once
#include "GameObject.h"

class ResourceManager
{
public:
	static void addEntity(Transform* entity);

	static std::vector<Transform*> Transforms;
};