#type vertex
#version 450

layout(location = 0) in vec3 a_Position;

out vec4 v_Colour;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0f);
	v_Colour = vec4(a_Position * 0.5 + 0.5, 1.0f);
}

#type fragment
#version 450

layout(location = 0) out vec4 colour;

in vec4 v_Colour;

void main()
{
	colour = v_Colour;
}