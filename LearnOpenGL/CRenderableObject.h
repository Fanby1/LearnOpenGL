#pragma once
#include "CCamera.h"
#include "CObject.h"
#include "CDirectionalLight.h"
#include "def.h"

class CPointLight;
struct SShaderMessage;
class GLTRAINVER3_API CRenderableObject : public CObject
{
public:
	CRenderableObject() = default;

	CRenderableObject(const std::string& vPath);
	void addVAO(std::shared_ptr<CVertexArrayObject> vVAO);
	void deleteVAO(std::shared_ptr<CVertexArrayObject> vVAO);
	void clearVAO();
	void setVAOForwardShader(std::shared_ptr<CVertexArrayObject> vVAO, std::shared_ptr<CShader> vShader);
	void setVAOGeometryShader(std::shared_ptr<CVertexArrayObject> vVAO, std::shared_ptr<CShader> vShader);
	void setVAOLightingShader(std::shared_ptr<CVertexArrayObject> vVAO, std::shared_ptr<CShader> vShader);
	std::vector<std::shared_ptr<CVertexArrayObject>> getVAOs();
	std::map<std::shared_ptr<CVertexArrayObject>, SShaderMessage>::iterator begin();
	std::map<std::shared_ptr<CVertexArrayObject>, SShaderMessage>::iterator end();
	virtual void renderV(std::shared_ptr<CCamera> vCamera, std::shared_ptr<CPointLight> vLight, std::shared_ptr<CDirectionalLight> vDirectionalLight) = 0;

protected:
	std::map<std::shared_ptr<CVertexArrayObject>, SShaderMessage> m_VAOToShadersMap;
	std::vector<unsigned int> __createOffset(unsigned int vType);
	std::vector<float> __readFloatArrayFromFile(std::ifstream& vFile);
};

struct SShaderMessage {
	std::shared_ptr<CShader> m_ForwardShader = nullptr, m_GeometryShader = nullptr, m_LightingShader = nullptr;
};
