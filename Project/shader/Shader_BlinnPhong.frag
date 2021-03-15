#version 460

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

layout (location = 0) out vec4 FragColor;
layout (binding = 0) uniform sampler2D Tex1;

uniform struct LightInfo 
{
  vec4 Position;
  vec3 La;
  vec3 L;
} Lights;

uniform struct MaterialInfo 
{
  vec3 Ka;  //Ambient Reflectivity
  vec3 Kd;  //Diffuse Reflectivity
  vec3 Ks;  //Specular Reflectivity
  float Shininess;   //Specular Shininess Factor
} Material;

uniform struct SpotLightInfo 
{
  vec3 Position;
  vec3 La;
  vec3 L;
  vec3 Direction;
  float Exponent;
  float CutOff;
} Spot;

//Light One, Need to check which light ngl..
vec3 LightsModel(vec3 position, vec3 normal)
{
	//Ambient
	vec3 ambient = Material.Ka * Lights.La;

	//Diffuse
	vec3 s = normalize(vec3(Lights.Position.xyz - position));
	float sDotN = max( dot(s, normal), 0.0 );
	vec3 diffuse = Lights.L * Material.Kd * sDotN;
	
	//Specular
	vec3 specular = vec3(0.0);

	if (sDotN > 0.0)
	{
		vec3 v = normalize(-Position.xyz);
		vec3 h = normalize(v + s);
		specular = Material.Ks * pow(max(dot(h, normal), 0.0), Material.Shininess);
	}

	return ambient + Lights.L * (diffuse + specular);
}

//SpotLight!
vec3 BlinnPhongSpot(vec3 position, vec3 normal)
{
	//Ambient
	vec3 ambient = Material.Ka * Spot.La;

	//Diffuse
	vec3 s = normalize(vec3(Spot.Position.xyz - position));

	float cosAng = dot(-s, normalize(Spot.Direction));
	float angle = acos(cosAng);
	float spotScale = 0.0;

	vec3 diffuse = vec3(0.0f);
	vec3 specular = vec3(0.0f);

	if (angle < Spot.CutOff)
	{
		spotScale = pow(cosAng, Spot.Exponent);

		float sDotN = max( dot(s, normal), 0.0 );
		diffuse = Spot.L * Material.Kd * sDotN;
	
		//Specular
		specular = vec3(0.0);

		if (sDotN > 0.0)
		{
			vec3 v = normalize(-Position.xyz);
			vec3 h = normalize(v + s);
			specular = Material.Ks * pow(max(dot(h, normal), 0.0), Material.Shininess);
		}
	}

	return ambient + spotScale * Spot.L * (diffuse + specular);
}

vec3 TextureMeth(vec3 position, vec3 normal)
{
	//Texture Setting
	vec3 texColor = texture(Tex1, TexCoord).rgb; //Extract Colour for each frag.

	//Ambient
	vec3 ambient = Lights.La * texColor;

	//Diffuse
	vec3 s = normalize(vec3(Lights.Position.xyz - position));
	float sDotN = max( dot(s, normal), 0.0 );
	vec3 diffuse = Lights.L * texColor * sDotN;
	
	//Specular
	vec3 specular = vec3(0.0);

	if (sDotN > 0.0)
	{
		vec3 v = normalize(-Position.xyz);
		vec3 h = normalize(v + s);
		specular = Material.Ks * pow(max(dot(h, normal), 0.0), Material.Shininess);
	}

	return ambient + Lights.L * (diffuse + specular);
}

void main()
{
	vec4 FinalResultLights = vec4(LightsModel(Position, normalize(Normal)), 1);
	vec4 FinalResultSpot = vec4(BlinnPhongSpot(Position, normalize(Normal)), 1);
	vec4 TexMeth = vec4(TextureMeth(Position, normalize(Normal)), 1);
    FragColor = FinalResultLights + FinalResultSpot + TexMeth;
	//FragColor = FinalResultLights + FinalResultSpot;
	//FragColor = TexMeth;
}
