#version 450 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

// out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
}; 

uniform Material material;

void main()
{    
    gPosition = FragPos;
    gNormal = normalize(Normal);
    gAlbedoSpec.rgb = vec3(1.0, 1.0, 1.0);
    gAlbedoSpec.a = 1.0;
	
	// FragColor = vec4(Normal, 1.0);
}
