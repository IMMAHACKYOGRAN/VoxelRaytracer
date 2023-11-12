#pragma once
#include <vector>
#include <glm.hpp>

#include "Object.h"

class Scene
{
public:
	Scene();
	~Scene();

 

private:
	std::vector<Object> Objects;
};