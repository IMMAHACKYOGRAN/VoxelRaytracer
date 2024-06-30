#version 450

layout(location = 0) in vec3 a_Position;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out mat4 v_ProjectionMatrix;
out mat4 v_ViewMatrix;
out vec3 v_ScreenPosition;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
	v_ProjectionMatrix = u_Projection;
	v_ViewMatrix = u_View;
	v_ScreenPosition = a_Position;
}