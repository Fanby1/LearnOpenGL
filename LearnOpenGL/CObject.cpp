#include "CObject.h"
#include <string>
#include <fstream>
#include <iostream>
#include "HiveLogger.h"


void CObject::__transform(std::shared_ptr<CShader> vShader)
{
	Eigen::Matrix4f Mat = Eigen::Matrix4f::Identity();
	Mat.block<3, 3>(0, 0) *= m_Scale;
	Mat.block<3, 1>(0, 3) = m_Position;
	vShader->setModel(Mat);
}

void CObject::__rotation(std::shared_ptr<CShader> vShader)
{
	auto Current = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> Elapsed = Current - m_Start;  // Calculate elapsed time
	double Angle = M_PI * Elapsed.count() * 0.1;
	Eigen::Vector3f Axis(0, 1, 0);

	Eigen::AngleAxisf RotationVector(Angle, Axis);
	Eigen::Matrix3f RotationMatrix = RotationVector.toRotationMatrix();
	Eigen::DiagonalMatrix<float, 3> ScaleMatrix({ 0.7,0.5,1 });

	Eigen::Matrix4f Mat = Eigen::Matrix4f::Identity();
	Mat.block<3, 3>(0, 0) = RotationMatrix * ScaleMatrix;
	Mat(0, 3) = 0.5 * cos(-Angle);
	Mat(1, 3) = 0.5 * sin(-Angle);
	vShader->setModel(Mat);
}

	while (std::getline(vFile, Line)) {
		std::stringstream ss(Line);
		std::string Value;
		
		while (std::getline(ss, Value, ',')) {
			try {
				Result.push_back(std::stof(Value));
			}
			catch (const std::invalid_argument& e) {
				HIVE_LOG_ERROR("Invalid number in file: {}", Value);
			}
			catch (const std::out_of_range& e) {
				HIVE_LOG_ERROR("Number out of range in file: {}", Value);
			}
		}
	}


CObject::CObject(const std::string& vPath, std::shared_ptr<CShader> vShader)
{
	std::ifstream File(vPath);
	if (!File.is_open()) {
		HIVE_LOG_ERROR("Failed to open file: {}", vPath);
	}
	std::string Mode;
	unsigned int Type = 0;
	if (std::getline(File, Mode)) {
		std::size_t Position = Mode.find(":");
		if (Position != std::string::npos) {
			std::string NumberStr = Mode.substr(Position + 1);
			try {
				Type = std::stoi(NumberStr);
			}
			catch (const std::invalid_argument& e) {
				HIVE_LOG_ERROR("Invalid number format: {}", e.what());
			}
			catch (const std::out_of_range& e) {
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

void CObject::addVAO(std::shared_ptr<CVertexArrayObject> vVAO, std::shared_ptr<CShader> vShader)
{
	m_VAOs[vVAO] = vShader;
}

void CObject::deleteVAO(std::shared_ptr<CVertexArrayObject> vVAO)
{
	m_VAOs.erase(vVAO);
}

void CObject::clearVAO()
{
	m_VAOs.clear();
}

void CObject::setVAO(std::map<std::shared_ptr<CVertexArrayObject>, std::shared_ptr<CShader>>&& vVAOs)
{
	m_VAOs = vVAOs;
}

std::map<std::shared_ptr<CVertexArrayObject>, std::shared_ptr<CShader>>::iterator CObject::begin()
{
	return m_VAOs.begin();
}

std::map<std::shared_ptr<CVertexArrayObject>, std::shared_ptr<CShader>>::iterator CObject::end()
{
	return m_VAOs.end();
}

void CObject::setUpdateMoveFunction(std::function<void(std::chrono::duration<double>, CObject&)> vFunction)
{
	m_UpdateMoveFunction = vFunction;
}

void CObject::move(Eigen::Vector3f vDisplacement)
{
	m_Position += vDisplacement;
}

void CObject::setPosition(Eigen::Vector3f vNewPosition)
{
	m_Position = vNewPosition;
}

void CObject::scale(float vScale)
{
	m_Scale *= vScale;
}

void CObject::setScale(float vScale)
{
	m_Scale = vScale;
}

Eigen::Vector3f CObject::getPosition()
{
	return m_Position;
}


