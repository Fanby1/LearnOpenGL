#version 450 core

in vec2 TexCoords;
in vec3 ambient;
in vec3 diffuse;
in vec3 specular;  // 从顶点着色器接收到的颜色
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
}; 
uniform Material material;

void main() {
	vec3 ambient = ambient *  texture(material.diffuse, TexCoords).rgb;

	vec3 diffuse = diffuse * texture(material.diffuse, TexCoords).rgb;  
    
	vec3 specular = specular * texture(material.specular, TexCoords).rgb;  
    
	vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
