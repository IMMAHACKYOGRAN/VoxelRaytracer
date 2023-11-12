#version 450

layout(location = 0) out vec4 colour;

in vec4 v_ScreenPosition;

uniform vec3 u_CameraPosition;
uniform mat4 u_InverseProjection;
uniform mat4 u_InverseView;

struct Sphere
{
	float Radius;
	vec3 Position;
};

struct Ray
{
	vec3 Origin;	
	vec3 Dir;
};

bool IntersectSphere(Ray ray, Sphere sphere)
{
	vec3 origin = ray.Origin - sphere.Position;

	float a = dot(ray.Dir, ray.Dir);
	float b = 2.0f * dot(origin, ray.Dir);
	float c = dot(origin, origin) - sphere.Radius * sphere.Radius;

	float discriminant = b * b - 4.0f * a * c;
	if (discriminant < 0.0f)
		return false;

	float closestT = (-b - sqrt(discriminant)) / (2.0f * a);
	return true;
}

void main()
{
	Ray ray;
	
	ray.Origin = u_CameraPosition;

	vec4 target = u_InverseProjection * vec4(v_ScreenPosition.xy, 1.0f, 1.0f);
	ray.Dir = vec4(u_InverseView * vec4(normalize(target.xyz / target.w), 0)).xyz; 

	Sphere sphere;
	sphere.Radius = 1.0f;
	sphere.Position = vec3(0.0f, 0.0f, 3.0f);

	vec3 CurrentColour = ray.Dir;

	if(IntersectSphere(ray, sphere))
	{
		CurrentColour = vec3(1.0f, 1.0f, 1.0f);
	}
	
	colour = vec4(CurrentColour, 1.0f);
}