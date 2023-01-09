#version 450 core

struct LightProperties{
	vec3 color;
	bool isEnabled;
	vec3 ambient;
	bool isLocal;
	vec3 position;
	bool isSpot;
	vec3 halfVector;
	float spotCosCutoff;
	vec3 coneDirection;
	float spotExponent;
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
};

const int MaxLights = 1;
layout (std140) uniform LightBlock{
	LightProperties Lights[MaxLights];
};

uniform mat4 view;

uniform float Shininess = 50.f;
uniform float StrengthDiff = 20.f;
uniform float StrengthSpec = 0.f;
uniform vec3 EyeDirection = vec3(0, 0, 1);

uniform sampler2D TextureSampler;

in vec3 Normal;
in vec4 Position;
in vec2 TexCoord;

out vec4 FragColor;

void main(){
	vec3 scatteredLight = vec3(0.0);
	vec3 reflectedLight = vec3(0.0);

	for (int light = 0; light < MaxLights; light++){
		if (!Lights[light].isEnabled){
			continue;
		}

		vec3 halfVector;
		vec3 lightDirection = vec3(view * vec4(Lights[light].position, 1.0));
		float attenuation = 1.0;

		if (Lights[light].isLocal){
			lightDirection = lightDirection - vec3(Position);
			float lightDistance = length(lightDirection);
			lightDirection = lightDirection/lightDistance;

			attenuation = 1.0 / 
				(Lights[light].constantAttenuation
				+ Lights[light].linearAttenuation * lightDistance
				+ Lights[light].quadraticAttenuation * lightDistance * lightDistance);

			if (Lights[light].isSpot){
				float spotCos = dot(lightDirection, -Lights[light].coneDirection);
				if (spotCos < Lights[light].spotCosCutoff){
					attenuation = 0.0;
				} 
				else{
					attenuation *= pow(spotCos, Lights[light].spotExponent);
				}
			}
			halfVector = normalize(lightDirection + EyeDirection);
		}
		else{
			halfVector = Lights[light].halfVector;
		}
		float diffuse = max(0.0, dot(Normal, lightDirection));
		float specular = max(0.0, dot(Normal, halfVector));
		
		if (diffuse == 0.0){
			specular = 0.0;
		}
		else{
			specular = pow(specular, Shininess) * StrengthSpec;
		}

		scatteredLight += Lights[light].ambient * attenuation + Lights[light].color * diffuse * attenuation * StrengthDiff;
		reflectedLight += Lights[light].color * specular * attenuation;
	}

	vec3 rgb = min(scatteredLight + reflectedLight, vec3(1.0));
	FragColor = texture(TextureSampler, TexCoord) * vec4(rgb, 1.0);
}