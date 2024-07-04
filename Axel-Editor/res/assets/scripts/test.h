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
		auto& t = GetComponent<TransformComponent>().Translation;

		if (Input::IsKeyDown(KeyCode::W))
			t.z += dt;
		if (Input::IsKeyDown(KeyCode::S))
			t.z -= dt;
		if (Input::IsKeyDown(KeyCode::D))
			t.x += dt;
		if (Input::IsKeyDown(KeyCode::A))
			t.x -= dt;
		if (Input::IsKeyDown(KeyCode::Q))
			t.y += dt;
		if (Input::IsKeyDown(KeyCode::E))
			t.y -= dt;
	}
};