#pragma once
#include "CCamera.h"
#include "CObject.h"
#include "CDirectionalLight.h"

class CPointLight;
class GLTRAINVER3_API CRenderableObject : public CObject
{
public:
	CRenderableObject() = default;
	CRenderableObject(const std::string& vPath, std::shared_ptr<CShader> vShader);
	void addVAO(std::shared_ptr<CVertexArrayObject> vVAO, std::shared_ptr<CShader> vShader);
	void deleteVAO(std::shared_ptr<CVertexArrayObject> vVAO);
	void clearVAO();
	void setVAO(std::map<std::shared_ptr<CVertexArrayObject>, std::shared_ptr<CShader>>&& vVAOs);
	std::map<std::shared_ptr<CVertexArrayObject>, std::shared_ptr<CShader>>::iterator begin();
	std::map<std::shared_ptr<CVertexArrayObject>, std::shared_ptr<CShader>>::iterator end();
	virtual void renderV(std::shared_ptr<CCamera> vCamera, std::shared_ptr<CPointLight> vLight, std::shared_ptr<CDirectionalLight> vDirectionalLight) = 0;

protected:
	std::map<std::shared_ptr<CVertexArrayObject>, std::shared_ptr<CShader>> m_VAOs;
	std::vector<unsigned int> __createOffset(unsigned int vType);
	std::vector<float> __readFloatArrayFromFile(std::ifstream& vFile);
};

