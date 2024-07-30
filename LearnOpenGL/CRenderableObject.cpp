#include "CRenderableObject.h"
#include <iostream>
#include <fstream>
#include <ranges>
#include "HiveLogger.h"

std::vector<unsigned int> CRenderableObject::__createOffset(unsigned int vType)
{
	std::vector<unsigned int> Offset;
	if (vType & VERTEX_TYPE_VERTEX_BIT) 
	{
		Offset.push_back(3);
	}
	else 
	{
		Offset.push_back(0);
	}
	if (vType & VERTEX_TYPE_COLOR_BIT) 
	{
		Offset.push_back(3);
	}
	else 
	{
		Offset.push_back(0);
	}
	if (vType & VERTEX_TYPE_TEXTURE_BIT) 
	{
		Offset.push_back(2);
	}
	else 
	{
		Offset.push_back(0);
	}
	if (vType & VERTEX_TYPE_NORMAL_BIT) 
	{
		Offset.push_back(3);
	}
	else 
	{
		Offset.push_back(0);
	}
	return Offset;
}

std::vector<float> CRenderableObject::__readFloatArrayFromFile(std::ifstream& vFile) 
{
	std::vector<float> Result;
	std::string FileBuffer;

	while (std::getline(vFile, FileBuffer)) 
	{
		std::stringstream ss(FileBuffer);
		std::string ValueString;

		while (std::getline(ss, ValueString, ',')) 
		{
			try 
			{
				Result.push_back(std::stof(ValueString));
			}
			catch (const std::invalid_argument& e) 
			{
				HIVE_LOG_ERROR("Invalid number in file: {}", ValueString);
			}
			catch (const std::out_of_range& e) 
			{
				HIVE_LOG_ERROR("Number out of range in file: {}", ValueString);
			}
		}
	}
	vFile.close();
	return Result;
}

CRenderableObject::CRenderableObject(const std::string& vPath)
{
	std::ifstream File(vPath);
	if (!File.is_open()) 
	{
		std::cerr << "Failed to open file: " << vPath << std::endl;
	}
	std::string Mode;
	unsigned int Type = 0;
	if (std::getline(File, Mode)) 
	{
		std::size_t Position = Mode.find(":");
		if (Position != std::string::npos) 
		{
			std::string ValueString = Mode.substr(Position + 1);
			try 
			{
				Type = std::stoi(ValueString);
			}
			catch (const std::invalid_argument& e) 
			{
				HIVE_LOG_ERROR("Invalid number format: {}",e.what());
			}
			catch (const std::out_of_range& e) 
			{
				HIVE_LOG_ERROR("Number out of range: {}", e.what());
			}
		}
	}

	std::vector<float> Vertices = __readFloatArrayFromFile(File);
	std::vector<unsigned int> Offset = __createOffset(Type);

	auto VBO = std::make_shared<CVertexBufferObject>(Vertices.data(), sizeof(float) * Vertices.size(), Type, Offset);
	auto VAO = std::make_shared<CVertexArrayObject>();
	VAO->addVBO(VBO);
	addVAO(VAO);
}

void CRenderableObject::addVAO(std::shared_ptr<CVertexArrayObject> vVAO)
{
	m_VAOToShadersMap[vVAO] = SShaderMessage();
}

void CRenderableObject::deleteVAO(std::shared_ptr<CVertexArrayObject> vVAO)
{
	m_VAOToShadersMap.erase(vVAO);
}

void CRenderableObject::clearVAO()
{
	m_VAOToShadersMap.clear();
}

void CRenderableObject::setVAOForwardShader(std::shared_ptr<CVertexArrayObject> vVAO, std::shared_ptr<CShader> vShader)
{
	m_VAOToShadersMap[vVAO].m_ForwardShader = vShader;
}

void CRenderableObject::setVAOGeometryShader(std::shared_ptr<CVertexArrayObject> vVAO, std::shared_ptr<CShader> vShader)
{
	m_VAOToShadersMap[vVAO].m_GeometryShader = vShader;
}

void CRenderableObject::setVAOLightingShader(std::shared_ptr<CVertexArrayObject> vVAO, std::shared_ptr<CShader> vShader)
{
	m_VAOToShadersMap[vVAO].m_LightingShader = vShader;
}

const SShaderMessage& CRenderableObject::getVAOShaders(std::shared_ptr<CVertexArrayObject> vVAO)
{
	return m_VAOToShadersMap[vVAO];
}

std::vector<std::shared_ptr<CVertexArrayObject>> CRenderableObject::getVAOs()
{
	std::vector<std::shared_ptr<CVertexArrayObject>> Keys;

	for (const auto& Key : std::views::keys(m_VAOToShadersMap)) {
		Keys.push_back(Key);
	}
	return Keys;
}

std::map<std::shared_ptr<CVertexArrayObject>, SShaderMessage>::iterator CRenderableObject::begin()
{
	return m_VAOToShadersMap.begin();
}

std::map<std::shared_ptr<CVertexArrayObject>, SShaderMessage>::iterator CRenderableObject::end()
{
	return m_VAOToShadersMap.end();
}

