#pragma once
#include <Axel.h>
using namespace Axel;

class test : public ScriptableEntity
{
	TransformComponent transform;

	virtual void Start() override
	{
		//transform = GetComponent<TransformComponent>();
	}

	virtual void Update(float dt) override
	{
		//transform.Rotation.z += 1.0f;
	}
};