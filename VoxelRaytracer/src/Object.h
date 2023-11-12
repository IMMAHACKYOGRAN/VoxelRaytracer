#pragma once
#include "Renderer/VertexArray.h"

class Object
{
public:
	Object();
	~Object();

private:
	glm::vec3 m_Position = glm::vec3(0.0f);
	glm::mat4 m_ModelMat = glm::mat4(1.0f);

};