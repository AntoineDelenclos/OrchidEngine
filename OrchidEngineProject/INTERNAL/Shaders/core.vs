#version 460 core

//La valeur de location correspond à l'id d'attribution pour chaque vertex dans glVertexAttribPointer

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

out vec2 TexCoord;
out vec3 Normal;
out vec3 crntPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 movement;

void main() {
	gl_Position = projection * view * movement * model * vec4(position,1.0);
	TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
	Normal = normal;
	crntPos = vec3(model * vec4(position, 1.0)); //Permet d'avoir la position du Frag et donc relative aux coordonnées du monde
}