#pragma once
#include <vector>

#include "Components.h"

class Entity
{
public:
	Entity();
	~Entity();

private:
	uint32_t m_EntityID;
	std::vector<> m_Components;
};