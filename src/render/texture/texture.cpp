#include "Texture.h"

Texture::Texture(unsigned int texIdx, const char* resoucePath)
	:m_TexId(texIdx) {

	stbi_set_flip_vertically_on_load(true);

	glGenTextures(1, &this->m_Id);
	glBindTexture(GL_TEXTURE_2D, this->m_Id);

	// 纹理环绕方式 GL_REPEAT-重复 GL_MIRRORED_REPEAT-镜像重复 GL_CLAMP_TO_EDGE-拉伸 GL_CLAMP_TO_BORDER-溢出显示为边缘颜色
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// float borderColor[] = {1.0f, 1.0f, 0.0f, 1.0f};
	// glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// 纹理过滤
	// GL_NEAREST-邻近过滤 默认值 OpenGL会选择中心点最接近纹理坐标的那个像素
	// GL_LINEAR-线性过滤 基于纹理坐标附近的纹理像素, 计算出一个插值, 近似出这些纹理像素之间的颜色
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 缩小
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 放大
	// 多级渐远纹理(主要在缩小过滤使用, 放大使用报错GL_INVALID_ENUM): 距观察者的距离超过一定的阈值，OpenGL会使用不同的多级渐远纹理，即最适合物体的距离的那个
	// GL_NEAREST_MIPMAP_NEAREST 使用最邻近的多级渐远纹理来匹配像素大小, 并使用邻近插值进行纹理采样
	// GL_LINEAR_MIPMAP_NEAREST 使用最邻近的多级渐远纹理级别, 并使用线性插值进行采样
	// GL_NEAREST_MIPMAP_LINEAR 在两个最匹配像素大小的多级渐远纹理之间进行线性插值, 使用邻近插值进行采样
	// GL_LINEAR_MIPMAP_LINEAR 在两个邻近的多级渐远纹理之间使用线性插值, 并使用线性插值进行采样
	// 加载贴图
	int width, height, nrChannels; // 宽度 高度 颜色通道数量
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(resoucePath, &width, &height, &nrChannels, 0);
	if (!data) {
		std::cout << "ERROR::TEXTURE::MSG(载入纹理失败, 索引-" << this->getTexId() << ")" << std::endl;
	}

	unsigned int colorMode = GL_RGB;
	if (nrChannels == 4) {
		colorMode = GL_RGBA;
	}
	// 载入贴图资源
	// (指定纹理目标, 指定多级渐远纹理级别(0-基本级别), 纹理储存格式, 最终纹理的宽度, 高度, 源图格式, 源图数据类型, 源图数据)
	glTexImage2D(GL_TEXTURE_2D, 0, colorMode, width, height, 0, colorMode, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	// 释放图像内存
	if (data) stbi_image_free(data);

	std::cout << "绑定纹理, Id: " << this->m_Id << " 索引: " << this->m_TexId << std::endl;
}

Texture::~Texture()
{
	this->unbind();
	glDeleteTextures(1, &m_Id);
}

void Texture::bind() const
{
	glActiveTexture(GL_TEXTURE0 + this->m_TexId);
	glBindTexture(GL_TEXTURE_2D, this->m_Id);
}

void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
