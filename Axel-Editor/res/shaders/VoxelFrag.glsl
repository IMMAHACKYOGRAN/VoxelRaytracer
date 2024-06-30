#version 450

layout(location = 0) out vec4 colour;

uniform vec2 u_ViewportSize;

in mat4 v_ProjectionMatrix;
in mat4 v_ViewMatrix;
in vec3 v_ScreenPosition;

vec3 Traverse(vec3 dir, vec3 org)
{
	return vec3(0, 0, 0);
}

void main()
{
	vec2 coord = vec2(gl_FragCoord.x / u_ViewportSize.x, gl_FragCoord.y / u_ViewportSize.y) * vec2(2.0) - vec2(1.0);
	vec4 target = v_ProjectionMatrix * vec4(coord.xy, 1, 1);
	vec4 direction = v_ViewMatrix * vec4(normalize(target.xyz), 0);

	colour = vec4(direction.xyz, 1);
}