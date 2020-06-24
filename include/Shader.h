#pragma once
#ifndef SHADER_H
#define SHADER_H
#define _CRT_SECURE_NO_WARNINGS
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define MAX_SHADER_SZ 100000

class Shader
{
public:
	GLuint Program;

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);

	void Use();

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec2(const std::string& name, const glm::vec2 &value) const;
	void setVec2(const std::string& name, float x, float y) const;
	void setVec3(const std::string& name, const glm::vec3 &value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
	void setVec4(const std::string& name, const glm::vec4 &value) const;
	void setVec4(const std::string& name, float x,float y, float z, float w) const;
	void setMat2(const std::string& name, const glm::mat2 &mat) const;
	void setMat3(const std::string& name, const glm::mat3& mat) const;
	void setMat4(const std::string& name, const glm::mat4& mat) const;
};


#endif