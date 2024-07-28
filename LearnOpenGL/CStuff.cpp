#include "CStuff.h"

CStuff::CStuff(const std::string& vPath, std::shared_ptr<CShader> vShader) : CRenderableObject(vPath, vShader)
{
}

void CStuff::renderV(std::shared_ptr<CCamera> vCamera, std::shared_ptr<CPointLight> vLight, std::shared_ptr<CDirectionalLight> vDirectionalLight)
{
	if (__isFunctionSet()) 
	{
		auto Current = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> Elapsed = Current - m_Start;
		m_UpdateMoveFunction(Elapsed, *this);
	}
	Eigen::Vector3f LightPosition = { 0, 0, 0 };
	if (vLight) 
	{
		Eigen::Vector3f LightPosition = vLight->getPosition();
	}
	for (auto& It : m_VAOs) 
	{
		__transform(It.second);
		It.second->use();
		if (vCamera) 
		{
			vCamera->updateShaderUniforms(It.second);
		}
		It.second->setVec3("viewPos", vCamera->getPosition());
		if (vLight) 
		{
			It.second->setVec3("lightPos", LightPosition);
			It.second->setVec3("lightColor", { 1,1,1 });
		}
		if (vDirectionalLight) 
		{
			vDirectionalLight->updateShaderUniforms(It.second);
		}
		
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
