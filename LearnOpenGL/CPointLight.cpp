#include "CPointLight.h"

CPointLight::CPointLight(const std::string& vPath)
{
	
}

void CPointLight::updateShaderUniforms(std::shared_ptr<CShader> vShader)
{
	if (__isFunctionSet()) {
		auto Current = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> Elapsed = Current - m_Start;
		m_UpdateMoveFunction(Elapsed, *this);
	}
	/*
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float constant;
	float linear;
	float quadratic;
	*/
	vShader->setVec3(m_NameInShader + ".position", m_Position);
	vShader->setVec3(m_NameInShader + ".ambient", {0.1f, 0.1f, 0.1f});
	vShader->setVec3(m_NameInShader + ".diffuse", m_Color);
	vShader->setVec3(m_NameInShader + ".specular", m_Color);

	vShader->setFloat(m_NameInShader + ".constant", 1.0f);
	vShader->setFloat(m_NameInShader + ".linear", 0.7f);
	vShader->setFloat(m_NameInShader + ".quadratic", 1.8f);
}