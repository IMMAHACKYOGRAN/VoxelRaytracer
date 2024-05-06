#version 450

const int MAX_RAY_STEPS = 100;

layout(location = 0) out vec4 colour;

in vec3 v_ScreenPosition;

uniform sampler3D VoxelVolume;

uniform vec3 u_CamPos;
uniform vec3 u_CamDir;
uniform ivec2 u_Resolution;

uniform vec3 u_ObjectSize;

struct Ray
{
  vec3 Pos;
  vec3 Dir;
};

/*
Calculates the position of the ray projected onto the 
face of bounding box of the voxel volume.
*/
vec3 CalculateRayPos()
{
  return vec3(0.0, 0.0, 0.0);
}

// Possibly incorrect function
vec3 CalculateRayDir()
{
  vec2 screenPos = (gl_FragCoord.xy/u_Resolution.xy);
  vec3 planeU = vec3(1.0, 0.0, 0.0);
  vec3 planeV = vec3(0.0, 1.0, 0.0) * (u_Resolution.x / u_Resolution.y);

  return normalize((u_CamDir + screenPos.x * planeU + screenPos.y * planeV));
}

vec3 Traverse(Ray ray)
{
  float stepDist = 0.02;
  vec3 rayStep = sign(ray.Dir);
  vec3 mapPos = vec3(0,0,0); // Bad
  for (int i = 0; i < MAX_RAY_STEPS; i++)
  {
    mapPos += rayStep * stepDist;
    if (texture(VoxelVolume, mapPos).r != 0)
      return vec3(1, 0, 0);
  }
  return vec3(texture(VoxelVolume, mapPos));
}

void main()
{
  Ray ray;
  ray.Pos = (v_ScreenPosition + 0.5) * 10;
  ray.Dir = CalculateRayDir();

  colour = vec4(Traverse(ray), 1.0); 
}