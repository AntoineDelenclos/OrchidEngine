#version 460 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D ourTexture;


void main(){
	vec4 TempObjectColor = (texture(ourTexture, TexCoord));
	vec3 ObjectColor = vec3(TempObjectColor.x, TempObjectColor.y, 0.1);

	FragColor = vec4(1.0f) - vec4(ObjectColor, 1.0);
	//FragColor = func_post_processing(vec4(ObjectColor, 1.0));
}