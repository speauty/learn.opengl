#version 330 core
layout(location = 0) in vec3 aPos; // λ�ñ���������λ��ֵΪ0
layout(location = 1) in vec3 aColor; // ��ɫ����������λ��ֵΪ1

out vec3 vertexColor;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    vertexColor = aColor;
}