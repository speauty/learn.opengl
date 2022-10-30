#include "Texture.h"

Texture::Texture(unsigned int texIdx, const char* resoucePath)
	:m_TexId(texIdx) {

	stbi_set_flip_vertically_on_load(true);

	glGenTextures(1, &this->m_Id);
	glBindTexture(GL_TEXTURE_2D, this->m_Id);

	// �����Ʒ�ʽ GL_REPEAT-�ظ� GL_MIRRORED_REPEAT-�����ظ� GL_CLAMP_TO_EDGE-���� GL_CLAMP_TO_BORDER-�����ʾΪ��Ե��ɫ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// float borderColor[] = {1.0f, 1.0f, 0.0f, 1.0f};
	// glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// �������
	// GL_NEAREST-�ڽ����� Ĭ��ֵ OpenGL��ѡ�����ĵ���ӽ�����������Ǹ�����
	// GL_LINEAR-���Թ��� �����������긽������������, �����һ����ֵ, ���Ƴ���Щ��������֮�����ɫ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // ��С
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // �Ŵ�
	// �༶��Զ����(��Ҫ����С����ʹ��, �Ŵ�ʹ�ñ���GL_INVALID_ENUM): ��۲��ߵľ��볬��һ������ֵ��OpenGL��ʹ�ò�ͬ�Ķ༶��Զ���������ʺ�����ľ�����Ǹ�
	// GL_NEAREST_MIPMAP_NEAREST ʹ�����ڽ��Ķ༶��Զ������ƥ�����ش�С, ��ʹ���ڽ���ֵ�����������
	// GL_LINEAR_MIPMAP_NEAREST ʹ�����ڽ��Ķ༶��Զ������, ��ʹ�����Բ�ֵ���в���
	// GL_NEAREST_MIPMAP_LINEAR ��������ƥ�����ش�С�Ķ༶��Զ����֮��������Բ�ֵ, ʹ���ڽ���ֵ���в���
	// GL_LINEAR_MIPMAP_LINEAR �������ڽ��Ķ༶��Զ����֮��ʹ�����Բ�ֵ, ��ʹ�����Բ�ֵ���в���
	// ������ͼ
	int width, height, nrChannels; // ��� �߶� ��ɫͨ������
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(resoucePath, &width, &height, &nrChannels, 0);
	if (!data) {
		std::cout << "ERROR::TEXTURE::MSG(��������ʧ��, ����-" << this->getTexId() << ")" << std::endl;
	}

	unsigned int colorMode = GL_RGB;
	if (nrChannels == 4) {
		colorMode = GL_RGBA;
	}
	// ������ͼ��Դ
	// (ָ������Ŀ��, ָ���༶��Զ������(0-��������), �������ʽ, ��������Ŀ��, �߶�, Դͼ��ʽ, Դͼ��������, Դͼ����)
	glTexImage2D(GL_TEXTURE_2D, 0, colorMode, width, height, 0, colorMode, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	// �ͷ�ͼ���ڴ�
	if (data) stbi_image_free(data);

	std::cout << "������, Id: " << this->m_Id << " ����: " << this->m_TexId << std::endl;
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
