#pragma once

#include <iostream>

#include <glad/glad.h>
#include "stb_image.h"

class Texture
{
private:
	unsigned int m_Id;
	unsigned int m_TexId;

public:
	Texture(unsigned int texIdx, const char* resoucePath);
	~Texture();

	unsigned int getId() const { return m_Id; }
	unsigned int getTexId() const { return m_TexId; }
	void bind() const;
	void unbind() const;
};

