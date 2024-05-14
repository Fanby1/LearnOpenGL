#include "CObject.h"

void CObject::__rotation(std::shared_ptr<CShader> vShader)
{
	auto Current = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> Elapsed = Current - m_Start;  // Calculate elapsed time
	double Angle = M_PI * Elapsed.count();
	Eigen::Vector3f Axis(0, 0, 1);

	Eigen::AngleAxisf RotationVector(Angle, Axis);
	Eigen::Matrix3f RotationMatrix = RotationVector.toRotationMatrix();
	Eigen::DiagonalMatrix<float, 3> ScaleMatrix({ 0.7,0.5,1 });

	Eigen::Matrix4f Mat = Eigen::Matrix4f::Identity();
	Mat.block<3, 3>(0, 0) = RotationMatrix * ScaleMatrix;
	Mat(0, 3) = 0.5 * cos(Angle);
	Mat(1, 3) = 0.5 * sin(Angle);
	vShader->m_Transform = Mat;
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

void CObject::render()
{
	for (auto& It : m_VAOs) {
		__rotation(It.second);
		It.second->use();
		It.first->bind();
		glDrawElements(GL_TRIANGLES, It.first->getEBO()->getSize(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}


