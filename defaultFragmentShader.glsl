#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightPos;
uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;

uniform vec3 materialAmbient;
uniform vec3 materialDiffuse;
uniform vec3 materialSpecular;
uniform float materialShininess;

uniform vec3 viewPos;

void main(){
	vec3 ambient = lightAmbient * materialAmbient;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = lightDiffuse * (materialDiffuse * diff);

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
	vec3 specular = lightSpecular * (materialSpecular * spec);

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}