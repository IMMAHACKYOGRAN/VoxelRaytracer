#version 450
// JANKIEST CODE IN THE WORLD
// PROBABLY DOESNT EVEN USE THE RIGHT ALGORITHYM
// IDFK WHAT TO DO

layout(location = 0) out vec4 colour;

in vec3 v_ScreenPosition;

uniform vec3 u_CamPos;
uniform ivec3 u_ObjectSize;
uniform mat4 u_InverseProjection;
uniform mat4 u_InverseView;

layout(std430, binding = 1) readonly buffer VoxelBuffer
{
	//uint Pallete[256];
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
	uint HitIndex;
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

		uint index = uint((hit.HitLocation.y * u_ObjectSize.y + hit.HitLocation.x) + hit.HitLocation.z * u_ObjectSize.x * u_ObjectSize.y);
		uint currentVoxelInt = Voxels[index / 4];
		hit.HitIndex = (currentVoxelInt >> ((4 - index % 4)*8));

		if (hit.HitIndex != 0)
		{
			hit.DidHit = true;
			return hit;
		}
	}

	return hit;
}

// vec3 DDATraverse(Ray ray)
// {
//   vec3 mapPos = ray.Pos;
//   vec3 deltaDist = abs(1.0 / ray.Dir);
//   vec3 rayStep = sign(ray.Dir);
//   vec3 sideDist = (rayStep * (mapPos - ray.Pos) + (rayStep * 0.5) + 0.5) * deltaDist;

//   for (int i = 0; i < MAX_RAY_STEPS; i++) {
//     if (sideDist.x < sideDist.y) {
//       if (sideDist.x < sideDist.z) {
//         sideDist.x += deltaDist.x;
//         mapPos.x += rayStep.x;
//       }
//       else {
//         sideDist.z += deltaDist.z;
//         mapPos.z += rayStep.z;
//       }
//     }
//     else {
//       if (sideDist.y < sideDist.z) {
//         sideDist.y += deltaDist.y;
//         mapPos.y += rayStep.y;
//       }
//       else {
//         sideDist.z += deltaDist.z;
//         mapPos.z += rayStep.z;
//       }
// 	  }

//     uint volumeIdx = uint((int(mapPos.y) * u_ObjectSize.y + int(mapPos.x)) + int(mapPos.z) * u_ObjectSize.x * u_ObjectSize.y); 
//     uint compressedValue = VoxelVolume[volumeIdx / 4];
//     uint palleteIdx = (compressedValue>>((4-volumeIdx%4)*8))&256; // litteral voodoo

//     //uint finalcolour = Pallete[palleteIdx / 4];
//     if (palleteIdx != 0) {
//       return vec3(1, 0, 0);
//       //return vec3((finalcolour>>24)/256, ((finalcolour>>16)&256)/256, ((finalcolour>>8)&256)/256); // More voodoo
//     }
//     return mapPos;
//   }

//   return vec3(0.0, 1.0, 0.0);
// }

vec3 CalculateRayDir()
{
	vec4 target = u_InverseProjection * vec4(v_ScreenPosition.xy, 1.0, 1.0);
	return vec4(u_InverseView * vec4(normalize(target.xyz / target.w), 0)).xyz; 
}

void main()
{
	// (gl_FragCoord.x / width, gl_FragCoord.y / height) Relative to the screen.
	// (v_ScreenPosition) Relative to the object
	
	Ray ray;
	// Ray origin should be the 3d coordinate of the current fragment
	ray.Origin = (v_ScreenPosition + 0.5) * 10;
	ray.Dir = normalize(v_ScreenPosition - ray.Origin);
	
	Hit hit = Traverse(ray);
	if(hit.DidHit)
	{
		colour = vec4(hit.HitLocation, 1);
	}
	else
	{
		discard;
	}
}
