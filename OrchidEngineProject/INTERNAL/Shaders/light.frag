#version 330 core

out vec4 fragColor;

uniform vec3 ownLightColor;

void main(){
	fragColor = vec4(ownLightColor.x, ownLightColor.y, ownLightColor.z, 1.0);
}