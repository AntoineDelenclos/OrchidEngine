#version 330 core

in vec2 TexCoord;
in vec3 crntPos;

out vec4 fragColor;

uniform sampler2D ourTexture;
uniform vec3 ownLightColor;

void main(){
	vec4 TempColor = (texture(ourTexture, TexCoord));
	vec3 Color = vec3(TempColor.x, TempColor.y, TempColor.z);
	Color.x = Color.x * ownLightColor.x;
	Color.y = Color.y * ownLightColor.y;
	Color.z = Color.z * ownLightColor.z;

	fragColor = vec4(Color.x, Color.y, Color.z, 1.0);
}