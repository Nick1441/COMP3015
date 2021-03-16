#version 460

in vec3 Position;
in vec3 Normal;

layout (location = 0) out vec4 FragColor;

uniform int Pass;

uniform struct LightInfo 
{
  vec4 Position;
  vec3 La;
  vec3 L;
} Light;

uniform struct MaterialInfo 
{
  vec3 Ka;  //Ambient Reflectivity
  vec3 Kd;  //Diffuse Reflectivity
  vec3 Ks;  //Specular Reflectivity
  float Shininess;   //Specular Shininess Factor
} Material;

uniform struct FogInfo
{
	float MaxDist;
	float MinDist;
	vec3 Color;
}Fog;

const int levels = 4;
const float scaleFactor = 1.0 / levels;

vec3 BlinnPhongModel(vec3 position, vec3 normal)
{
	//Ambient
	float ambientStrength = 0.2f;
	vec3 ambient = ambientStrength * Light.La;

	//Diffuse
	vec3 s = normalize(vec3(Light.Position.xyz - position));
	float sDotN = max( dot(s, normal), 0.0 );

	//Toon Shading
	vec3 diffuse = Material.Kd * floor(sDotN * levels) * scaleFactor;


	return ambient + Light.L * (diffuse);
}

void main()
{
	if (Pass == 0)
	{
		FragColor = vec4(BlinnPhongModel(Position, normalize(Normal)), 1);
	}
	if (Pass == 1)
	{
		float dist = abs(Position.z);

		float fogFactor = (Fog.MaxDist - dist)/(Fog.MaxDist - Fog.MinDist);

		fogFactor = clamp(fogFactor, 0.0, 1.0);

		vec3 shadeColour = BlinnPhongModel(Position, normalize(Normal));

		vec3 colour = mix(Fog.Color, shadeColour, fogFactor);

		FragColor = vec4(colour, 1.0);
	}
}
