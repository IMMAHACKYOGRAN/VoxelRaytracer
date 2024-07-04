#version 450

layout(location = 0) out vec4 o_FinalColour;

in vec3 v_WorldPos;
in vec3 v_Normal;

uniform vec3 u_CameraPos;

// PBR Material Inputs
uniform vec3 u_Albedo;
uniform float u_Metallic;
uniform float u_Roughness;

// Lighting
uniform vec3 u_LightPosition;
uniform vec3 u_LightColor;
uniform float u_LightIntensity;

// Constants
const float PI = 3.14159265359;
const float EPSILON = 0.000001;

/* --------------------------------------------------------------- */

float GGXNormalDistribution(vec3 n, vec3 h, float rough)
{
	float a4 = rough * rough * rough * rough;
	float denominator = dot(n, h) * dot(n, h) * (a4 - 1.0) + 1.0;

	float d = a4 / max((PI * denominator * denominator), EPSILON);
	return d;
}

float GeomSmith(vec3 n, vec3 v, float rough)
{
	float K = (rough + 1.0) * (rough + 1.0) / 8.0;
	float denominator = max(dot(n, v), EPSILON) * (1.0 - K) + K;

	float g = max(dot(n, v), EPSILON) / denominator;
	
	return g;
}

vec3 FresnelSchlick(vec3 v, vec3 h, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(max(1.0 - clamp(dot(h, v), 0.0, 1.0), 0.0), 5.0);
}

vec3 CalculatePBR()
{
	vec3 Normal = normalize(v_Normal);
	vec3 View = normalize(u_CameraPos - v_WorldPos);
	
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, u_Albedo, u_Metallic);

	vec3 Light = normalize(u_LightPosition - v_WorldPos);
	vec3 Half = normalize(View + Light);

	float dist = length(u_LightPosition - v_WorldPos);
	float attenuation = u_LightIntensity / (dist * dist); // Inverse square law
	vec3 resultantLight = u_LightColor * attenuation;

	float D = GGXNormalDistribution(Normal, Half, u_Roughness);
	float G = GeomSmith(Normal, View, u_Roughness) * GeomSmith(Normal, Light, u_Roughness);
	vec3 F = FresnelSchlick(View, Half, F0);

	vec3 numerator = D * G * F;
	float denominator = 4.0 * max(dot(Normal, Light), EPSILON) * max(dot(Normal, View), EPSILON);
	vec3 specular = numerator / denominator;

	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - u_Metallic;

	vec3 ambient = vec3(0.03) * u_Albedo;

	vec3 outgoingLight = (kD * u_Albedo / PI + specular) * resultantLight * max(dot(Normal, Light), EPSILON) + ambient;
	outgoingLight = outgoingLight / (outgoingLight + 1.0);
	outgoingLight = pow(outgoingLight, vec3(1.0 / 2.2));

	return outgoingLight;
}

void main()
{
	o_FinalColour = vec4(CalculatePBR(), 1);
}