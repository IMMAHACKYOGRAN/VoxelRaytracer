#version 450

layout(location = 0) in vec4 a_Position;

out vec4 v_ScreenPosition;

void main()
{
	gl_Position = a_Position;
	v_ScreenPosition = a_Position;
}