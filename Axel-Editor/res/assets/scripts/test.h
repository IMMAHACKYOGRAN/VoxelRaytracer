#pragma once
#include <Axel.h>
using namespace Axel;

class test : public ScriptableEntity
{
	virtual void Start() override
	{
	}

	virtual void Update(float dt) override
	{
		AX_INFO("script");
	}
};