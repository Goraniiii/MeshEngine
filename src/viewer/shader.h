#ifndef SHADER_H
#define SHADER_H

#include "../common.h"

#include <glad/glad.h>

#include <Eigen/Dense>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    uint32_t ID;

    Shader(const char* vertexPath, const char* fragmentPath);

    void use();

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, const Eigen::Matrix4f& mat) const;
    void setMat3(const std::string& name, const Eigen::Matrix3f& mat) const;
    void setVec3(const std::string& name, const Eigen::Vector3f& vec) const;

private:
    void checkCompileErrors(unsigned int shader, std::string type);
};

#endif