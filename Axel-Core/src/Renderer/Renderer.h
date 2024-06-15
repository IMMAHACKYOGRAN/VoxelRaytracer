#pragma once

#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include "EditorCamera.h"
#include "ECS/Components.h"

namespace Axel
{
	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const EditorCamera& camera);
		static void EndScene();

		static void Clear(const glm::vec4& colour);

		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		static void DrawCube(TransformComponent transform);

		struct Stats
		{
			uint32_t SceneCount = 0;
			uint32_t DrawCalls  = 0;
			uint32_t TriCount  = 0;

			uint32_t GetTotalVertexCount() const { return TriCount * 3; }
		};

		static Stats GetStats();
		static void ClearStats();

	private:
		static void DrawIndexed();

	};
}