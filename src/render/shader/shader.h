#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
private:
	unsigned int m_RenderId;

public:
	Shader();
	~Shader();
	

	unsigned int loadFromFile(const char* vertexPath, const char* fragmentPath) const;
	void exec() const;

	void uniformSetBool(const std::string &name, bool val) const;
	void uniformSetInt(const std::string &name, int val) const;
	void uniformSetFloat(const std::string &name, float val) const;
	void uniformSet4Float(const std::string &name, float val1, float val2, float val3, float val4) const;
	void uniformSetMatrix4fv(const std::string &name, int cntMatrix, bool isTranspose, const float* val) const;

	unsigned int getId() const { return this->m_RenderId; }

private:
	unsigned int checkerCompileError(unsigned int shader, std::string type) const;
};

