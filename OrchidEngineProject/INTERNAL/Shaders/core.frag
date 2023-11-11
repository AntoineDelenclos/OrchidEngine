#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 crntPos; //Fragment Position

out vec4 FragColor;

uniform sampler2D ourTexture;

uniform vec3 viewPos;

struct PostProcess{
	float brightness;
	float contrast;
	float saturation;
	float gamma;
	vec3 grayscale_norm;
};
uniform PostProcess post_process;

struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
	float transparency;
};
uniform Material material;

struct Light{
	vec3 color;
	vec3 position;
	vec3 ambientIntensity;
	vec3 diffuseStrength;
	vec3 specularStrength;
};
uniform Light light;


vec4 func_brightness(vec4 col){
	col.x += post_process.brightness;
	col.y += post_process.brightness;
	col.z += post_process.brightness;
	col.w += post_process.brightness;
	return col;
}

vec4 func_contrast(vec4 col){
	col = (post_process.contrast,post_process.contrast,post_process.contrast,post_process.contrast) * ( col - (0.5f,0.5f,0.5f,0.5f) ) + (0.5f,0.5f,0.5f,0.5f);
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

	vec3 ambientVec = light.ambientIntensity * material.ambient * light.color;

	vec3 normalVec = normalize(Normal);

	vec3 lightDirection = normalize(light.position - crntPos);

	float diffuse = max(dot(normalVec, lightDirection), 0.0);
	vec3 diffuseVec = light.diffuseStrength * (diffuse * material.diffuse) * light.color;

	vec3 viewDir = normalize(viewPos - crntPos);
	vec3 reflectDir = reflect(-lightDirection, normalVec);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 128);
	vec3 specularVec = light.specularStrength * (spec * material.specular) * light.color;

	vec3 result = (ambientVec + diffuseVec + specularVec) * ObjectColor;
	FragColor = vec4(result, material.transparency);

	FragColor = func_post_processing(FragColor);
}