#version 450
// JANKIEST CODE IN THE WORLD
// PROBABLY DOESNT EVEN USE THE RIGHT ALGORITHYM
// IDFK WHAT TO DO

layout(location = 0) out vec4 colour;

in vec3 v_ObjectPosition;

uniform vec3 u_CamOrigin;
uniform ivec3 u_ObjectSize;
uniform mat4 u_InverseProjection;
uniform mat4 u_InverseView;
uniform vec2 u_Resolution;

// Works
layout(std430, binding = 1) readonly buffer VoxelBuffer
{
	uint Voxels[];
};

struct Ray
{
	vec3 Origin;
	vec3 Dir;
};

struct Hit
{
	bool DidHit;
	ivec3 HitLocation;
};

// Good Luck
Hit Traverse(Ray ray)
{
	Hit hit;
	hit.HitLocation = ivec3(floor(ray.Origin));
	hit.DidHit = false;

	vec3 InverseRayDir = 1.0 / ray.Dir;
	vec3 RayUnitStepSize = abs(InverseRayDir);
	ivec3 Stp = ivec3(sign(ray.Dir)); //stp is used because step is a glsl function
	vec3 RayLength = abs(hit.HitLocation + max(Stp, vec3(0.0)) - ray.Origin) * RayUnitStepSize;

	float MaxDistance = 10.0;
	float TotalDistance = 0.0;
	while (!hit.DidHit && TotalDistance < MaxDistance)
	{
		if (RayLength.x < RayLength.y)
		{
			if (RayLength.x < RayLength.z)
			{
				hit.HitLocation.x += Stp.x;
				TotalDistance = RayLength.x;
				RayLength.x += RayUnitStepSize.x;
			}
			else
			{
				hit.HitLocation.z += Stp.z;
				TotalDistance = RayLength.z;
				RayLength.z += RayUnitStepSize.z;
			}
		}
		else
		{
			if (RayLength.y < RayLength.z)
			{
				hit.HitLocation.y += Stp.y;
				TotalDistance = RayLength.y;
				RayLength.y += RayUnitStepSize.y;
			}
			else
			{
				hit.HitLocation.z += Stp.z;
				TotalDistance = RayLength.z;
				RayLength.z += RayUnitStepSize.z;
			}
		}

		uint index = int((hit.HitLocation.y * u_ObjectSize.y + hit.HitLocation.x) + hit.HitLocation.z * u_ObjectSize.x * u_ObjectSize.y);
		uint currentVoxelInt = Voxels[index / 4];
		uint currentVoxel = (currentVoxelInt >> (4 - index % 4));

		if (currentVoxel != 0)
		{
			hit.DidHit = true;
			return hit;
		}
	}

	return hit;
}

vec3 CalculateRayDir()
{
	vec2 screenPos = vec2(gl_FragCoord.x / u_Resolution.x, gl_FragCoord.y / u_Resolution.y);
	vec4 target = u_InverseProjection * vec4(screenPos, 1.0, 1.0);
	return vec4(u_InverseView * vec4(normalize(target.xyz / target.w), 0)).xyz; 
}

void main()
{
	Ray ray;
	ray.Origin = (v_ObjectPosition + 0.5) * 10;
	ray.Dir = v_ObjectPosition - u_CamOrigin;
	
	Hit hit = Traverse(ray);
	if(hit.DidHit)
	{
		colour = vec4(0, 1, 0, 1);
	}
	else
	{
		//discard;
		colour = vec4(0, 0, 0, 1);
	}
}
