#include "Shader.h"

Shader::Shader() {
	this->m_RenderId = glCreateProgram(); // 创建着色器程序
}

Shader::~Shader() {
	glDeleteProgram(this->m_RenderId);
}

unsigned int Shader::loadFromFile(const char* vertexPath, const char* fragmentPath) const {
	std::string vertexCode, fragmentCode;
	std::ifstream vShaderFile, fShaderFile;

	// 标记输入文件流可以抛出的异常 failbit-IO操作逻辑错误 badbit-IO操作读写错误
	vShaderFile.exceptions(std::ifstream::failbit|std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit|std::ifstream::badbit);

	try {
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		
		std::stringstream vShaderStream, fShaderStrem;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStrem << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStrem.str();
	} catch (std::ifstream::failure& e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
		return 1;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertex, fragment;

	vertex = glCreateShader(GL_VERTEX_SHADER); // 创建着色器(顶点着色器)
	glShaderSource(vertex, 1, &vShaderCode, NULL); // 载入源码
	glCompileShader(vertex); // 编译着色器
	this->checkerCompileError(vertex, "VERTEX");

	fragment = glCreateShader(GL_FRAGMENT_SHADER); // 创建着色器(片段着色器)
	glShaderSource(fragment, 1, &fShaderCode, NULL); // 载入源码
	glCompileShader(fragment); // 编译着色器
	if (this->checkerCompileError(fragment, "FRAGMENT") > 0) {
		return 1;
	}

	glAttachShader(this->m_RenderId, vertex); // 附加着色器
	glAttachShader(this->m_RenderId, fragment);
	glLinkProgram(this->m_RenderId); // 链接
	if (this->checkerCompileError(this->m_RenderId, "PROGRAM") > 0) {
		return 1;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return 0;
}

void Shader::exec() const {
	glUseProgram(this->m_RenderId);
}

void Shader::uniformSetBool(const std::string& name, bool val) const {
	glUniform1i(glGetUniformLocation(this->m_RenderId, name.c_str()), (int)val);
}

void Shader::uniformSetInt(const std::string& name, int val) const {
	glUniform1i(glGetUniformLocation(this->m_RenderId, name.c_str()), (int)val);
}

void Shader::uniformSetFloat(const std::string& name, float val) const {
	glUniform1f(glGetUniformLocation(this->m_RenderId, name.c_str()), val);
}

void Shader::uniformSet4Float(const std::string& name, float val1, float val2, float val3, float val4) const {
	glUniform4f(glGetUniformLocation(this->m_RenderId, name.c_str()), val1, val2, val3, val4);
}

void Shader::uniformSetMatrix4fv(const std::string& name, int cntMatrix, bool isTranspose, const float* val) const {
	glUniformMatrix4fv(glGetUniformLocation(this->m_RenderId, name.c_str()), cntMatrix, isTranspose, val);
}

unsigned int Shader::checkerCompileError(unsigned int shader, std::string type) const {
	int success;
	char info[1024];

	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, info);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << " reason: " << info << std::endl;
			return 1;
		}
	} else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, info);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << " reason: " << info << std::endl;
			return 1;
		}
	}
	return 0;
}
