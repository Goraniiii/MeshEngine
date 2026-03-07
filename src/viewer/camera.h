#pragma once

#include <Eigen/Dense>

class Camera
{
public:

    float distance = 300.0f;
    float yaw = 0.0f;
    float pitch = 0.0f;

    Eigen::Matrix4f getViewMatrix() const;

    Eigen::Matrix4f getProjectionMatrix(float aspect) const;
};