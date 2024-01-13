#version 460 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D ourTexture;

struct PostProcess{
	float brightness;
	float contrast;
	float saturation;
	float gamma;
	vec3 grayscale_norm;
};
uniform PostProcess post_process;

vec4 func_brightness(vec4 col){
	col.x += post_process.brightness;
	col.y += post_process.brightness;
	col.z += post_process.brightness;
	col.w += post_process.brightness;
	return col;
}

vec4 func_contrast(vec4 col){
	col = (post_process.contrast,post_process.contrast,post_process.contrast,post_process.contrast) * ( col - (0.5,0.5,0.5,0.5) ) + (0.5,0.5,0.5,0.5);
	return col;
}

vec4 desaturation(vec4 col){
	float a = col.x * post_process.grayscale_norm.x;
	float b = col.y * post_process.grayscale_norm.y;
	float c = col.z * post_process.grayscale_norm.z;
	float d = a + b + c;
	col = vec4(d, d, d, col.w);
	return col;
}

vec4 func_saturation(vec4 col){
	vec4 grayscale = desaturation(col);
	float a = mix(grayscale.x, col.x, post_process.saturation);
	float b = mix(grayscale.y, col.y, post_process.saturation);
	float c = mix(grayscale.z, col.z, post_process.saturation);
	col = vec4(a, b, c, col.w);
	return col;
}

vec4 func_gamma(vec4 col){
	col.x = pow(col.x, post_process.gamma);
	col.y = pow(col.y, post_process.gamma);
	col.z = pow(col.z, post_process.gamma);
	col.w = pow(col.w, post_process.gamma);
	return col;
}

vec4 func_post_processing(vec4 col){
	col = func_brightness(col);
	col = func_contrast(col);
	col = func_saturation(col);
	col = func_gamma(col);
	return col;
}

void main(){
	vec4 TempObjectColor = (texture(ourTexture, TexCoord));
	vec3 ObjectColor = vec3(TempObjectColor.x, TempObjectColor.y, TempObjectColor.z);

	FragColor = func_post_processing(vec4(ObjectColor, 1.0));
}