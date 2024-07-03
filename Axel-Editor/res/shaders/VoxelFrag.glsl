#version 450

layout(location = 0) out vec4 colour;

uniform vec2 u_ViewportSize;
uniform vec3 u_CameraPos;

in mat4 v_ProjectionMatrix;
in mat4 v_ViewMatrix;
in vec3 v_ScreenPosition;

bool InVoxel(ivec3 position) {
	vec3 adjustedposition = vec3(position) + vec3(0.5); // To ensure that ray is not on a boundry.
    float d = length(adjustedposition) - 5.0; // locus of |Z| = 12
	return d < 0.0;
}

vec3 Traverse(vec3 dir, vec3 org)
{
	ivec3 mapPos = ivec3(floor(org));
	vec3 deltaDist = abs(vec3(length(dir)) / dir);
	ivec3 rayStep = ivec3(sign(dir));
	vec3 sideDist = (sign(dir) * (vec3(mapPos) - org) + (sign(dir) * 0.5) + 0.5) * deltaDist;

	bvec3 mask;

	bool hit = false;

	for (int i = 0; i < 100; i++) {
		if (InVoxel(mapPos)) 
		{
			hit = true;
			break;
		}

		mask = lessThanEqual(sideDist.xyz, min(sideDist.yzx, sideDist.zxy));

		sideDist += vec3(mask) * deltaDist;
		mapPos += ivec3(vec3(mask)) * rayStep;
	}

	if (hit)
	{
		if (mask.x) {
			return vec3(0.5);
		}
		if (mask.y) {
			return vec3(1.0);
		}
		if (mask.z) {
			return vec3(0.75);
		}
	}
	return vec3(0);
}

vec2 CalculatePos(vec3 camPos, vec3 invRayDir, vec3 boxMin, vec3 boxMax)
{
	vec3 tMin = (boxMin - camPos) * invRayDir;
	vec3 tMax = (boxMax - camPos) * invRayDir;

	vec3 t1 = min(tMin, tMax);
	vec3 t2 = max(tMin, tMax);

	float tNear = max(max(t1.x, t1.y), t1.z);
	float tFar  = min(min(t2.x, t2.y), t2.z);

	return vec2(tNear, tFar);
}

void main()
{
	vec2 coord = vec2(gl_FragCoord.x / u_ViewportSize.x, gl_FragCoord.y / u_ViewportSize.y) * 2.0 - 1.0;
	vec4 target = v_ProjectionMatrix * vec4(coord.xy, 1, 1);
	vec4 direction = v_ViewMatrix * vec4(normalize(target.xyz), 0);

	vec3 rayPos = (v_ViewMatrix * vec4(0.0, 0.0, 0.0, 1.0)).xyz;

	vec2 intersections = CalculatePos(u_CameraPos, 1/direction.xyz, vec3(-0.5), vec3(0.5));

	if(intersections.x > intersections.y || intersections.y < 0) //make sky color if no intersection at all
	{
		colour = vec4(1, 0, 0 ,1);
	}
	else
	{
		colour = vec4(Traverse(direction.xyz, rayPos), 1);
	}

}