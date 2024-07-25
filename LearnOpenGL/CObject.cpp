#include "CObject.h"

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

std::vector<unsigned int> CObject::__createOffset(unsigned int vType)
{
	std::vector<unsigned int> Offset;
	if(vType & VERTEX_TYPE_VERTEX_BIT) {
		Offset.push_back(3);
	}
	else {
		Offset.push_back(0);
	}
	if(vType & VERTEX_TYPE_COLOR_BIT) {
		Offset.push_back(3);
	}
	else {
		Offset.push_back(0);
	}
	if(vType & VERTEX_TYPE_TEXTURE_BIT) {
		Offset.push_back(2);
	}
	else {
		Offset.push_back(0);
	}
	if(vType & VERTEX_TYPE_NORMAL_BIT) {
		Offset.push_back(3);
	}
	else {
		Offset.push_back(0);
	}
	return Offset;
}

std::vector<float> CObject::__readFloatArrayFromFile(std::ifstream& vFile) {
	std::vector<float> Result;
	std::string Line;

	while (std::getline(vFile, Line)) {
		std::stringstream ss(Line);
		std::string Value;
		
		while (std::getline(ss, Value, ',')) {
			try {
				Result.push_back(std::stof(Value));
			}
			catch (const std::invalid_argument& e) {
				std::cerr << "Invalid number in file: " << Value << std::endl;
			}
			catch (const std::out_of_range& e) {
				std::cerr << "Number out of range in file: " << Value << std::endl;
			}
		}
	}

	vFile.close();
	return Result;
}

CObject::CObject(const std::string& vPath, std::shared_ptr<CShader> vShader)
{
	std::ifstream File(vPath);
	if (!File.is_open()) {
		std::cerr << "Failed to open file: " << vPath << std::endl;
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
				std::cerr << "Invalid number format: " << e.what() << std::endl;
			}
			catch (const std::out_of_range& e) {
				std::cerr << "Number out of range: " << e.what() << std::endl;
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

void CObject::render()
{
	for (auto& It : m_VAOs) {
		__rotation(It.second);
		It.second->use();
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


