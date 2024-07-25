#pragma once
#include "CCamera.h"
#include "CObject.h"

class CLight;
class CRenderableObject :
    public CObject
{
protected:
	std::map<std::shared_ptr<CVertexArrayObject>, std::shared_ptr<CShader>> m_VAOs;
	std::vector<unsigned int> __createOffset(unsigned int vType);
	std::vector<float> __readFloatArrayFromFile(std::ifstream& vFile);
public:
	CRenderableObject() = default;
	CRenderableObject(const std::string& vPath, std::shared_ptr<CShader> vShader);
	void addVAO(std::shared_ptr<CVertexArrayObject> vVAO, std::shared_ptr<CShader> vShader);
	void deleteVAO(std::shared_ptr<CVertexArrayObject> vVAO);
	void clearVAO();
	void setVAO(std::map<std::shared_ptr<CVertexArrayObject>, std::shared_ptr<CShader>>&& vVAOs);
	std::map<std::shared_ptr<CVertexArrayObject>, std::shared_ptr<CShader>>::iterator begin();
	std::map<std::shared_ptr<CVertexArrayObject>, std::shared_ptr<CShader>>::iterator end();
	virtual void renderV(std::shared_ptr<CCamera> vCamera, std::shared_ptr<CLight> vLight) = 0;
};

