#pragma once
#include <vector>

#include "ECS/Components.h"

class Entity
{
public:
	Entity();
	~Entity();

private:
	uint32_t m_EntityID;
};