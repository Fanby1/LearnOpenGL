#include "CCamera.h"

void CCamera::__computeProjection()
{
    // Convert FOV from degrees to radians
    float Theta = m_FeildOfView * (M_PI / 180.0f);
    float Range = m_NearPlane - m_FarPlane;
    float Invtan = 1.0f / tan(Theta / 2.0f);

    m_Projection(0, 0) = Invtan / m_AspectRatio;
    m_Projection(1, 1) = Invtan;
    m_Projection(2, 2) = (m_NearPlane + m_FarPlane) / Range;
    m_Projection(3, 2) = -1;
    m_Projection(2, 3) = 2 * m_NearPlane * m_FarPlane / Range;
    m_ComputeProjectionFlag = false;
}

void CCamera::__computeView()
{
    Eigen::Vector3f Z = (m_Position - m_CameraTarget).normalized();  // The "forward" vector.
    Eigen::Vector3f X = m_UpVector.cross(Z).normalized();  // The "right" vector.
    Eigen::Vector3f Y = Z.cross(X);  // The "up" vector.

    m_View = Eigen::Matrix4f::Identity();
    
    // Set the right, up and forward vectors
    m_View.block<3, 1>(0, 0) = X;
    m_View.block<3, 1>(0, 1) = Y;
    m_View.block<3, 1>(0, 2) = Z;

    // Set the translation part of the matrix
    m_View(0, 3) = -X.dot(m_Position);
    m_View(1, 3) = -Y.dot(m_Position);
    m_View(2, 3) = -Z.dot(m_Position);

    m_ComputeViewFlag = false;
}

void CCamera::setFeildOfView(float vFeildOfView)
{
    m_FeildOfView = vFeildOfView;
    m_ComputeProjectionFlag = true;
}

void CCamera::setAspectRatio(float vAspectRatio)
{
    m_AspectRatio = vAspectRatio;
    m_ComputeProjectionFlag = true;
}

void CCamera::setNearPlane(float vNearPlane)
{
    m_NearPlane = vNearPlane;
    m_ComputeProjectionFlag = true;
}

void CCamera::setFarPlane(float vFarPlane)
{
    m_FarPlane = vFarPlane;
    m_ComputeProjectionFlag = true;
}

void CCamera::rotateUpVector(float vRadian)
{
    Eigen::AngleAxisf RotationMatrix(vRadian, (m_CameraTarget - m_Position).normalized());
    m_UpVector = RotationMatrix * Eigen::Vector3f(0,1,0);
    m_ComputeViewFlag = true;
}

void CCamera::moveCameraTarget(const Eigen::Vector3f& vTargetDistance)
{
    m_CameraTarget += vTargetDistance;
    m_ComputeViewFlag = true;
}

void CCamera::setCameraTarget(const Eigen::Vector3f& vTargetVertix)
{
    m_CameraTarget = vTargetVertix;
    m_ComputeViewFlag = true;
}

void CCamera::moveCameraPosition(const Eigen::Vector3f& vPositionDistance)
{
    m_Position = vPositionDistance;
    m_ComputeViewFlag = true;
}

void CCamera::setCameraPosition(const Eigen::Vector3f& vTargetPostion)
{
    m_Position = vTargetPostion;
    m_ComputeViewFlag = true;
}

void CCamera::updateShaderUniforms(const std::shared_ptr<CShader>& vShader)
{
    if (__isFunctionSet()) {
        auto Current = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> Elapsed = Current - m_Start;
		m_UpdateMoveFunction(Elapsed, *this);
    }

    vShader->setProjection(getProjectionMatrix());
    vShader->setView(getViewMatrix());
    vShader->setVec3("viewPos", m_Position);
}

Eigen::Matrix4f CCamera::getProjectionMatrix()
{
    if (m_ComputeProjectionFlag) {
        __computeProjection();
    }
    return m_Projection;
}

Eigen::Matrix4f CCamera::getViewMatrix()
{
    if (m_ComputeViewFlag) {
        __computeView();
    }

    return m_View;
}
