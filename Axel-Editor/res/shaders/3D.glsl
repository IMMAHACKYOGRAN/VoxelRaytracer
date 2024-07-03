#type vertex
#version 450

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 v_WorldPos;
out vec3 v_Normal;

void main()
{
	v_WorldPos = vec3(u_Model * vec4(a_Position, 1.0));
	v_Normal = a_Normal;

	gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
}

/* --------------------------------------------------------------- */

#type fragment
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
#define PI = 3.14159265359;
#define EPSILON = 0.000001;

/* --------------------------------------------------------------- */

float GGXNormalDistribution(vec3 n, vec3 h, vec3 rough)
{
	float a4 = rough * rough * rough * rough;
	float denominator = dot(n, h) * dot(n, h) * (a4 - 1.0) + 1.0;

	return (a4 / max(PI * denominator * denominator, EPSILON));
}

float GeomSmith(vec3 n, vec3 v, float rough)
{
	float K = (rough + 1.0) * (rough + 1.0) / 8.0;
	float denominator = max(dot(n, v), EPSILON) * (1.0 - K) + K;
	
	return max(dot(n, v), EPSILON) / denominator;
}

float FresnelSchlick(vec3 v, vec3 h, float F0)
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
	float attenuation = 1.0 / (dist * dist); // Inverse square law
	vec3 resultantLight = u_LightColor * attenuation;

	float D = GGXNormalDistribution(Normal, Half, u_Roughness);
	float G = GeomSmith(Normal, View, u_Roughness) * GeomSmith(Normal, Light, u_Roughness);
	vec3 F = FresnelSchlick(View, Half, F0);

	vec3 numerator = D * G * F;
	float denominator = 4.0 * max(dot(Normal, Light), EPSILON) * max(dot(Normal, View), EPSILON);
	vec3 specular = numerator / denominator;

	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - metallic;

	vec3 outgoingLight = (kD * u_Albedo / PI + specular) * resultantLight * max(dot(Normal, Light), 0.0);
	outgoingLight = outgoingLight / (outgoingLight + 1.0);
	outgoingLight = pow(outgoingLight, vec3(1.0 / 2.2));

	return outgoingLight;
}

void main()
{
	o_FinalColour = vec3(CalculatePBR(), 1);
}