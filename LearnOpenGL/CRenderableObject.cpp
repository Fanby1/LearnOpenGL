#include "CRenderableObject.h"
#include <iostream>
#include <fstream>
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

CRenderableObject::CRenderableObject(const std::string& vPath, std::shared_ptr<CShader> vShader)
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
	addVAO(VAO, vShader);
}

void CRenderableObject::addVAO(std::shared_ptr<CVertexArrayObject> vVAO, std::shared_ptr<CShader> vShader)
{
	m_VAOs[vVAO] = vShader;
}

void CRenderableObject::deleteVAO(std::shared_ptr<CVertexArrayObject> vVAO)
{
	m_VAOs.erase(vVAO);
}

void CRenderableObject::clearVAO()
{
	m_VAOs.clear();
}

void CRenderableObject::setVAO(std::map<std::shared_ptr<CVertexArrayObject>, std::shared_ptr<CShader>>&& vVAOs)
{
	m_VAOs = vVAOs;
}

std::map<std::shared_ptr<CVertexArrayObject>, std::shared_ptr<CShader>>::iterator CRenderableObject::begin()
{
	return m_VAOs.begin();
}

std::map<std::shared_ptr<CVertexArrayObject>, std::shared_ptr<CShader>>::iterator CRenderableObject::end()
{
	return m_VAOs.end();
}

