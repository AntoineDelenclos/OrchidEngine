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
	sampler2D diffuseTexture;
	sampler2D specularTexture;
	float shininess;
	float transparency;
};
uniform Material material;

struct DirectionalLight{
	bool on;
	vec3 color;
	vec3 direction;
	vec3 ambientIntensity;
	vec3 diffuseStrength;
	vec3 specularStrength;
};
#define MAX_DIRECTIONAL_LIGHTS 32
uniform int number_dir_light;
uniform DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];

struct PointLight{
	bool on; //Si l'entité est active ou non
	vec3 color;
	vec3 position;
	float constant;
	float linear;
	float quadratic;
	vec3 ambientIntensity;
	vec3 diffuseStrength;
	vec3 specularStrength;
};
#define MAX_POINT_LIGHTS 32
uniform int number_point_light;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

struct SpotLight{
	bool on;
	vec3 color;
	vec3 position;
	vec3 direction;
	float innerCutOff;
	float outerCutOff;
	vec3 ambientIntensity;
	vec3 diffuseStrength;
	vec3 specularStrength;
};
#define MAX_SPOT_LIGHTS 32
uniform int number_spot_light;
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

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

vec3 directional_light_calcul(DirectionalLight light, vec3 normal, vec3 view){
	if (light.on == false){
		return vec3(0.0,0.0,0.0);
	}

	vec3 lightDir = normalize(-light.direction);

	vec3 ambientVec = light.ambientIntensity * material.ambient * light.color;

	float diffuse = max(dot(normal, lightDir), 0.0);
	vec3 diffuseVec = light.diffuseStrength * diffuse * vec3(texture(material.diffuseTexture,TexCoord)) * light.color;

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(view, reflectDir), 0.0), material.shininess * 128);
	vec3 specularVec = light.specularStrength * spec * vec3(texture(material.specularTexture, TexCoord)) * light.color;

	return (ambientVec + diffuseVec + specularVec);
}

vec3 point_light_calcul(PointLight light, vec3 normal, vec3 view){
	if (light.on == false){
		return vec3(0.0,0.0,0.0);
	}
	
	vec3 lightDir = normalize(light.position - crntPos);

	vec3 ambientVec = light.ambientIntensity * material.ambient * light.color;

	float diffuse = max(dot(normal, lightDir), 0.0);
	vec3 diffuseVec = light.diffuseStrength * diffuse * vec3(texture(material.diffuseTexture,TexCoord)) * light.color;

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(view, reflectDir), 0.0), material.shininess * 128);
	vec3 specularVec = light.specularStrength * spec * vec3(texture(material.specularTexture, TexCoord)) * light.color;

	float distance = length(light.position - crntPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance*distance));

	ambientVec += attenuation;
	diffuseVec += attenuation;
	specularVec += attenuation;

	return (ambientVec + diffuseVec + specularVec);
}

vec3 spot_light_calcul(SpotLight light, vec3 normal, vec3 view){
	if (light.on == false){
		return vec3(0.0,0.0,0.0);
	}

	vec3 lightDir = normalize(light.position - crntPos);

	vec3 ambientVec = light.ambientIntensity * material.ambient * light.color;

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.innerCutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff)/epsilon, 0.0, 1.0);
	
	float diffuse = max(dot(normal, lightDir), 0.0);
	vec3 diffuseVec = light.diffuseStrength * diffuse * vec3(texture(material.diffuseTexture,TexCoord)) * light.color;

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(view, reflectDir), 0.0), material.shininess * 128);
	vec3 specularVec = light.specularStrength * spec * vec3(texture(material.specularTexture, TexCoord)) * light.color;

	diffuseVec *= intensity;
	specularVec *= intensity;

	return (ambientVec + diffuseVec + specularVec);
}

vec3 directionalLightsGlobalCalcul(vec3 normal, vec3 view){
	vec3 dirResult;
	for (int i = 0; i < number_dir_light; i++){
		dirResult += directional_light_calcul(directionalLights[i], normal, view);
	}
	return dirResult;
}

vec3 pointLightsGlobalCalcul(vec3 normal, vec3 view){
	vec3 pointResult;
	for (int i = 0; i < number_point_light; i++){
		pointResult += point_light_calcul(pointLights[i], normal, view);
	}
	return pointResult;
}

vec3 spotLightsGlobalCalcul(vec3 normal, vec3 view){
	vec3 spotResult;
	for (int i = 0; i < number_spot_light; i++){
		spotResult += spot_light_calcul(spotLights[i], normal, view);
	}
	return spotResult;
}

void main(){
	vec4 TempObjectColor = (texture(ourTexture, TexCoord));
	vec3 ObjectColor = vec3(TempObjectColor.x, TempObjectColor.y, TempObjectColor.z);
	
	vec3 normalVec = normalize(Normal);
	vec3 viewDir = normalize(viewPos - crntPos);
	vec3 result;

	result += directionalLightsGlobalCalcul(normalVec, viewDir);
	result += pointLightsGlobalCalcul(normalVec, viewDir);
	result += spotLightsGlobalCalcul(normalVec, viewDir);
	
	ObjectColor *= result;
	FragColor = vec4(ObjectColor, material.transparency);
	FragColor = func_post_processing(FragColor);
}