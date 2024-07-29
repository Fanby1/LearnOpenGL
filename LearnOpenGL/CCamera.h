#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <Eigen/Dense>
#include "CObject.h"
#include "def.h"

class GLTRAINVER3_API CCamera : public CObject
{
private:
    float m_FeildOfView = 90.0f;  // Field of View, in degrees
    float m_AspectRatio = 16.0f / 9.0f;  // Aspect Ratio. Depends on the size of your window
    float m_NearPlane = 0.1f;  // Near clipping plane. Keep as big as possible, or you'll get precision issues.
    float m_FarPlane = 100.0f;  // Far clipping plane. Keep as little as possible.
    bool m_ComputeProjectionFlag = true;
    bool m_ComputeViewFlag = true;

    Eigen::Vector3f m_Position{ 0, 0, 5 };
    Eigen::Vector3f m_CameraTarget{ 0, 0, 0 };
    Eigen::Vector3f m_UpVector{ 0, 1, 0 };

    Eigen::Matrix4f m_Projection = Eigen::Matrix4f::Zero();
    Eigen::Matrix4f m_View = Eigen::Matrix4f::Identity();

    void __computeProjection();
    void __computeView();
public:
    CCamera() = default;
    CCamera(float vFeildOfView, float vAspectRatio, float vNearPlane, float vFarPlane)
        :m_FeildOfView(vFeildOfView), m_AspectRatio(vAspectRatio), m_NearPlane(vNearPlane), m_FarPlane(vFarPlane) {
    }
    void setFeildOfView(float vFeildOfView);
    void setAspectRatio(float vAspectRatio);
    void setNearPlane(float vNearPlane);
    void setFarPlane(float vFarPlane);
    void rotateUpVector(float vRadian);
    void moveCameraTarget(const Eigen::Vector3f& vTargetDistance);
    void setCameraTarget(const Eigen::Vector3f& vTargetVertix);
    void moveCameraPosition(const Eigen::Vector3f& vPositionDistance);
    void setCameraPosition(const Eigen::Vector3f& vTargetPostion);
    void updateShaderUniforms(const std::shared_ptr<CShader>& vShader);
    Eigen::Matrix4f getProjectionMatrix();
    Eigen::Matrix4f getViewMatrix();
};

