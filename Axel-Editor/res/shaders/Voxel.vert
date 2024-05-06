#version 450

layout(location = 0) in vec3 a_Position;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 v_ObjectPosition;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
	v_ObjectPosition = a_Position;
}