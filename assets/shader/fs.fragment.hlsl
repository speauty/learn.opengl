#version 330 core
out vec4 FragColor;

in vec3 vertexColor; // �Ӷ�����ɫ�����������������������ͬ��������ͬ��

//uniform vec4 ourColor;

void main()
{
    FragColor = vec4(vertexColor, 0.5);
}