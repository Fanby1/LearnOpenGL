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

void CObject::setUpdateMoveFunction(std::function<void(std::chrono::duration<double>, CObject&)> vFunction)
{
	m_UpdateMoveFunction = vFunction;
}

void CObject::move(Eigen::Vector3f vDisplacement)
{
	m_Position += vDisplacement;
}

void CObject::setNameInShader(const std::string& vName)
{
	m_NameInShader = vName;
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


