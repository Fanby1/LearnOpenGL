#include "CStuff.h"

CStuff::CStuff(const std::string& vPath) : CRenderableObject(vPath)
{
}

void CStuff::renderV(std::shared_ptr<CCamera> vCamera, std::shared_ptr<CPointLight> vLight, std::shared_ptr<CDirectionalLight> vDirectionalLight)
{
	if (__isFunctionSet()) {
		auto Current = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> Elapsed = Current - m_Start;
		m_UpdateMoveFunction(Elapsed, *this);
	}
	Eigen::Vector3f LightPosition = { 0,0,0 };
	if (vLight) {
		Eigen::Vector3f LightPosition = vLight->getPosition();
	}
	for (auto& It : m_VAOToShadersMap) {
		__transform(It.second.m_ForwardShader);
		It.second.m_ForwardShader->use();
		if (vCamera) {
			vCamera->updateShaderUniforms(It.second.m_ForwardShader);
			It.second.m_ForwardShader->setVec3("viewPos", vCamera->getPosition());
		}
		if (vLight) {
			It.second.m_ForwardShader->setVec3("lightPos", LightPosition);
			It.second.m_ForwardShader->setVec3("lightColor", { 1,1,1 });
		}
		if (vDirectionalLight) {
			vDirectionalLight->updateShaderUniforms(It.second.m_ForwardShader);
		}
		
		It.first->bind();
		if (It.first->getEBO() != nullptr) {
			glDrawElements(GL_TRIANGLES, It.first->getEBO()->getSize(), GL_UNSIGNED_INT, 0);
		}
		else {
			auto VBOs = It.first->getVBOs();
			auto VBO = VBOs.begin();
			auto size = VBO->get()->getSize();
			glDrawArrays(GL_TRIANGLES, 0, VBO->get()->getSize());
		}
		glBindVertexArray(0);
	}
}
