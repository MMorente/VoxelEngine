#version 450 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexTex;
layout (location = 3) in mat4 model;
layout (location = 7) in mat3 modelInvTrans;

uniform mat4 projection;
uniform mat4 view;
uniform mat3 viewInvTrans;

out vec3 Normal;
out vec4 Position;
out vec2 TexCoord;

void main(){
	Normal = normalize(viewInvTrans * modelInvTrans * vertexNormal);
	Position = view * model * vec4(vertexPosition, 1.0);
	gl_Position = projection * Position;
	TexCoord = vertexTex;
}