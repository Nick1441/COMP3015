#version 460

in vec3 LightDir;
in vec2 TexCoord;
in vec3 ViewDir;
in vec3 Position;
in vec3 Normal;

layout (location = 0) out vec4 FragColor;
layout (binding = 0) uniform sampler2D ColorTex;
layout (binding = 1) uniform sampler2D NormalMapTex;
layout (binding = 3) uniform sampler2D ColorTex2;
layout (binding = 4) uniform sampler2D ColorTex3;

uniform int Pass;
uniform struct LightInfo 
{
  vec4 Position;
  vec3 La;
  vec3 L;
} Light;

uniform struct MaterialInfo 
{
  vec3 Ks;  //Specular Reflectivity
  vec3 Ka;
  float Shininess;   //Specular Shininess Factor
  vec3 Kd;
} Material;

uniform struct FogInfo
{
	float MaxDist;
	float MinDist;
	vec3 Color;
}Fog;

const int levels = 4;
const float scaleFactor = 1.0 / levels;

vec3 PhongModel(vec3 LightDir, vec3 normal, int i)
{
	vec3 texColor = vec3(0.0f);
	if (i == 1)
	{
		texColor = texture(ColorTex, TexCoord).xyz;
	}
	if (i == 2)
	{
		texColor = texture(ColorTex2, TexCoord).xyz;
	}
	if (i == 3)
	{
		texColor = texture(ColorTex3, TexCoord).xyz;
	}
	

	//Ambient
	vec3 ambient = Light.La * texColor;

	//Diffuse
	float sDotN = max( dot(LightDir, normal), 0.0 );
	vec3 diffuse = Material.Kd * texColor * sDotN;
	
	//Specular
	vec3 specular = vec3(0.0);

	if (sDotN > 0.0)
	{
		vec3 h = normalize(LightDir + ViewDir);
		specular = Material.Ks * pow(max(dot(normal, h), 0.0), Material.Shininess);
	}

	return ambient + Light.L * (diffuse + specular);
}

//Normal BlinnPhong Model
vec3 PhongModelNormal(vec3 position, vec3 normal)
{
	//Ambient
	vec3 ambient = Material.Ka * Light.La;

	//Diffuse
	vec3 s = normalize(vec3(Light.Position.xyz - position));
	float sDotN = max( dot(s, normal), 0.0 );
	vec3 diffuse = Light.L * Material.Kd * sDotN;
	
	//Specular
	vec3 specular = vec3(0.0);

	if (sDotN > 0.0)
	{
		vec3 v = normalize(-Position.xyz);
		vec3 h = normalize(v + s);
		specular = Material.Ks * pow(max(dot(h, normal), 0.0), Material.Shininess);
	}

	return ambient + Light.L * (diffuse + specular);
}

void main()
{
	vec3 norm = texture(NormalMapTex, TexCoord).xyz;
	norm.xy = 2.0 * norm.xy - 1.0;

	if (Pass == 0)
	{
		FragColor = vec4(PhongModel(LightDir, normalize(norm), 1), 1);
	}
	if (Pass == 1)
	{
		FragColor = vec4(PhongModelNormal(Position, normalize(Normal)), 1);
	}
	if (Pass == 2)
	{
		FragColor = vec4(PhongModel(LightDir, normalize(norm), 2), 1);
	}
	if (Pass == 3)
	{
		FragColor = vec4(PhongModel(LightDir, normalize(norm), 3), 1);
	}
	

}
