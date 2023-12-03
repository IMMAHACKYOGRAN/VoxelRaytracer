#type vertex
#version 450

layout(location = 0) in vec4 a_Position;

out vec4 v_Colour;

void main()
{
	gl_Position = a_Position;
	v_Colour = vec4(a_Position.xy * 0.5f + 0.5f, 0.0f, 1.0f);
}

#type fragment
#version 450

layout(location = 0) out vec4 colour;

in vec4 v_Colour;

void main()
{
	colour = v_Colour;
}