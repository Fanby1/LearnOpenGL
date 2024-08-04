#version 450 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gDepthTexture;

struct DirectionalLight {
    //vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct PointLight {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float constant;
	float linear;
	float quadratic;
};
struct Material {
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
}; 

uniform DirectionalLight light;
uniform PointLight pointLight;
uniform Material material;
uniform vec3 viewPos;

vec3 CalcDirectionalLighting(vec3 fragPos, vec3 normal, vec3 albedo, vec3 specular, vec3 viewDir)
{
	    // ambient
    vec3 ambient = light.ambient * albedo;
  	
    // diffuse 
    vec3 norm = normalize(normal);
    // vec3 lightDir = normalize(light.position - FragPos);
    vec3 lightDir = normalize(-light.direction);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * albedo;  
    
    // specular
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    specular = light.specular * spec * specular;  

    vec3 lighting = ambient + diffuse + specular;  

	return ambient + diffuse + specular;
}

vec3 CalcPointLighting(PointLight light, vec3 fragPos, vec3 normal, vec3 albedo, vec3 specularColor, vec3 viewDir)
{
	// ambient
	vec3 ambient = light.ambient * albedo;
	
	// diffuse 
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * albedo;  
	
	// specular
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * specularColor;  

	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));   
	attenuation = 1.0;
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	// ambient = vec3(0.0, 0.0, 0.0);
	// diffuse = vec3(0.0, 0.0, 0.0);
	// specular = vec3(0.0, 0.0, 0.0);

	vec3 lighting = ambient + diffuse + specular;  

	return lighting;
}

void main()
{
	float depthValue = texture(gDepthTexture, TexCoords).r;  
    gl_FragDepth = depthValue;
	// Retrieve data from G-buffer
    vec3 fragPos = texture(gPosition, TexCoords).rgb;
    vec3 normal = normalize(texture(gNormal, TexCoords).rgb);
    vec3 albedo = texture(gAlbedoSpec, TexCoords).rgb;
    vec3 specular = texture(gAlbedoSpec, TexCoords).aaa;
    vec3 wiewDir = normalize(viewPos - fragPos);

	// Lighting calculations
	vec3 lighting = CalcDirectionalLighting(fragPos, normal, albedo, specular, wiewDir);
	lighting += CalcPointLighting(pointLight, fragPos, normal, albedo, specular, wiewDir);

    FragColor = vec4(lighting, 1.0);
}
