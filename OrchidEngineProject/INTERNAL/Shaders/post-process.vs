#version 460 core

//La valeur de location correspond à l'id d'attribution pour chaque vertex dans glVertexAttribPointer

//layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

out vec2 TexCoord;

void main() {
	//gl_Position = vec4(position,1.0);
	TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
}