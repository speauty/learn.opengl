#pragma once
#include <string>


class Shader
{
private:
	unsigned int m_Id;

public:
	Shader();
	~Shader();
	

	unsigned int loadFromFile(const char* vertexPath, const char* fragmentPath) const;
	void exec() const;

	void uniformSetBool(const std::string &name, bool val) const;
	void uniformSetInt(const std::string &name, int val) const;
	void uniformSetFloat(const std::string &name, float val) const;
	void uniformSet4Float(const std::string &name, float val1, float val2, float val3, float val4) const;

	unsigned int getId() const { return this->m_Id; }

private:
	unsigned int checkerCompileError(unsigned int shader, std::string type) const;
};

