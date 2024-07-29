#include "CPointLight.h"

CPointLight::CPointLight(const std::string& vPath) : CRenderableObject(vPath)
{
	
}

void CPointLight::updateShaderUniforms(std::shared_ptr<CShader> vShader)
{
	vShader->setVec3("lightPos", m_Position);
	vShader->setVec3("lightColor", { 1,1,1 });
}

void CPointLight::renderV(std::shared_ptr<CCamera> vCamera, std::shared_ptr<CPointLight> vLight, std::shared_ptr<CDirectionalLight> vDirectionalLight)
{
	if (__isFunctionSet()) {
		auto Current = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> Elapsed = Current - m_Start;
		m_UpdateMoveFunction(Elapsed, *this);
	}
	for (auto& It : m_VAOToShadersMap) 
	{
		__transform(It.second.m_ForwardShader);
		It.second.m_ForwardShader->use();
		It.first->bind();
		if (vCamera) {
			vCamera->updateShaderUniforms(It.second.m_ForwardShader);
		}
		if (It.first->getEBO() != nullptr) 
		{
			glDrawElements(GL_TRIANGLES, It.first->getEBO()->getSize(), GL_UNSIGNED_INT, 0);
		}
		else 
		{
			auto VBOs = It.first->getVBOs();
			auto VBO = VBOs.begin();
			auto size = VBO->get()->getSize();
			glDrawArrays(GL_TRIANGLES, 0, VBO->get()->getSize());
		}
		glBindVertexArray(0);
	}
}
