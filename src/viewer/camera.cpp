#include "camera.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846 // Define M_PI if not already defined
#endif

Eigen::Matrix4f Camera::getViewMatrix() const
{
    Eigen::Vector3f eye;

    eye.x() = distance * cos(pitch) * sin(yaw);
    eye.y() = distance * sin(pitch);
    eye.z() = distance * cos(pitch) * cos(yaw);

    Eigen::Vector3f center(0, 0, 0);
    Eigen::Vector3f up(0, 1, 0);

    Eigen::Vector3f f = (center - eye).normalized();
    Eigen::Vector3f s = f.cross(up).normalized();
    Eigen::Vector3f u = s.cross(f);

    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    view(0, 0) = s.x(); view(0, 1) = s.y(); view(0, 2) = s.z();
    view(1, 0) = u.x(); view(1, 1) = u.y(); view(1, 2) = u.z();
    view(2, 0) = -f.x(); view(2, 1) = -f.y(); view(2, 2) = -f.z();

    view(0, 3) = -s.dot(eye);
    view(1, 3) = -u.dot(eye);
    view(2, 3) = f.dot(eye);

    return view;
}

Eigen::Matrix4f Camera::getProjectionMatrix(float aspect) const
{
    float fov = 45.0f * M_PI / 180.0f;
    float near = 0.1f;
    float far = 100.0f;

    float f = 1.0f / tan(fov / 2);

    Eigen::Matrix4f proj = Eigen::Matrix4f::Zero();

    proj(0, 0) = f / aspect;
    proj(1, 1) = f;
    proj(2, 2) = (far + near) / (near - far);
    proj(2, 3) = (2 * far * near) / (near - far);
    proj(3, 2) = -1;

    return proj;
}