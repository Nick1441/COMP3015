#version 460

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;								//Coords of Texture

layout (location = 0) out vec4 FragColor;
layout (binding = 0) uniform sampler2D CarImage;  //Texture 1
layout (binding = 2) uniform sampler2D AlphaTex;

uniform float Alpha_Threshold;

uniform int Pass_2;

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
  vec3 Kd;
  float Shininess;   //Specular Shininess Factor
} Material;

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
//ALPHA ONE
vec3 PhongModel(vec3 position, vec3 normal)
{
	//Texture Setting
	vec3 texColor = texture(CarImage, TexCoord).rbg;

	//Ambient
	vec3 ambient = Light.La * texColor;

	//Diffuse
	vec3 s = normalize(vec3(Light.Position.xyz - position));
	float sDotN = max( dot(s, normal), 0.0 );
	vec3 diffuse = Light.L * texColor * sDotN;
	
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
	vec4 alphaMap = texture(AlphaTex, TexCoord).rgba;
	float Threshold = normalize(Alpha_Threshold);

	if (Pass_2 == 0)
	{
		FragColor = vec4(PhongModelNormal(Position, normalize(Normal)), 1);
	}
	if (Pass_2 == 1)
	{
		if (alphaMap.a < (0.15))
		{
			discard;
		}
		else 
		{
			if (gl_FrontFacing)
			{
				FragColor = vec4(PhongModel(Position, normalize(Normal)), 1);
			}
			else 
			{
				FragColor = vec4(PhongModel(Position, normalize(-Normal)), 1);
			}
		}
	}

    
}
