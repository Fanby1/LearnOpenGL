#include "CPointLight.h"

CPointLight::CPointLight(const std::string& vPath, std::shared_ptr<CShader> vShader) : CRenderableObject(vPath, vShader)
{
	
}

void CPointLight::renderV(std::shared_ptr<CCamera> vCamera, std::shared_ptr<CPointLight> vLight, std::shared_ptr<CDirectionalLight> vDirectionalLight)
{
	if (__isFunctionSet()) {
		auto Current = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> Elapsed = Current - m_Start;
		m_UpdateMoveFunction(Elapsed, *this);
	}
	for (auto& It : m_VAOs) 
	{
		__transform(It.second);
		It.second->use();
		It.first->bind();
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
