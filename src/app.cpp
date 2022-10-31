#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <render/shader/shader.h>
#include <render/texture/texture.h>

void keyCallbackFn(GLFWwindow* window, int key, int scancode, int action, int mode);
void framebufferSizeCallbackFn(GLFWwindow* window, int width, int height);

const std::string PATH_SHADER = "E:\\MonkeyCode\\github.com\\speauty\\learn.opengl\\assets\\shader\\";
const std::string PATH_TEXTURE = "E:\\MonkeyCode\\github.com\\speauty\\learn.opengl\\assets\\texture\\";

int main() {

	glfwInit(); // ��ʼ��glfw

	// ���������ɲο�: http://www.glfw.org/docs/latest/window.html#window_hints
	// ���opengl�汾Ϊ3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // ���openglΪ����ģʽ
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); // �������ڴ�С

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (NULL == window) {
		std::cout << "ERROR::INIT::MSG(��������ʧ��)" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	
	// glfwGetProcAddress ��ȡϵͳ��ص�OpenGL����ָ���ַ
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "ERROR::INIT::MSG(��ʼ��GLADʧ��)" << std::endl;
		glfwTerminate();
		return -1;
	}

	// �����߿����ģʽ
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	unsigned char* glVersion;
	glVersion = (unsigned char*)glGetString(GL_VERSION);
	// ״̬: ��ǰ����GL 3.3.0 NVIDIA 512.72
	std::cout << "״̬: ��ǰ����GL " << glVersion << std::endl;
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "״̬: ��ǰ����֧�������������: " << nrAttributes << std::endl;

	// ע�ᰴ���ص�
	glfwSetKeyCallback(window, keyCallbackFn);
	// ע���ӿڻص�, ע��: ��Ҫ��ϱ��GLFW_RESIZABLEΪGL_TRUE, ���ڴ�С��ֹ����ʱ, ����ص��ƺ�Ҳûɶ��
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallbackFn);

	// ����(λ������ ��ɫ ��������)
	float vertices[] = {
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 2.0f, 2.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 2.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,2.0f,
	};

	unsigned int indices[] = { // ����
		0, 1, 3,
		1, 2, 3,
	};

	unsigned int vbo, vao, ebo; // ���㻺����� �������黺�����(GPU���Ͷ�������) Ԫ�ػ������
	glGenBuffers(1, &vbo); // ����һ��������
	glGenBuffers(1, &ebo);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao); // �󶨶����������

	glBindBuffer(GL_ARRAY_BUFFER, vbo); // �󶨵��������黺����(����ͬʱ�󶨶�����Ͳ�ͬ�Ļ���)
	// ���������ݸ��Ƶ��Ѱ󶨺õĻ�����, ���ĸ�����Ϊ����ģʽ, ��Ҫ�����¼�����ѡֵ:
	// 1. GL_STATIC_DRAW ���ݲ���򼸺�����ı� ��̬����
	// 2. GL_DYNAMIC_DRAW ���ݻᱻ�ı�ܶ� ��̬����
	// 3. GL_STREAM_DRAW ����ÿ�λ���ʱ����ı� ������
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// ����������Ԫ�ػ�����
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); // ��Ԫ�ػ������
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// ���ö�������ָ��-����-λ��0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// ���ö�������ָ��-��ɫ-λ��1
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);
	// ���ö�������ָ��-����-λ��2
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 6));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	Shader testShader;
	// @todo ����������޸�
	testShader.loadFromFile((PATH_SHADER + "vs.vertex.hlsl").c_str(), (PATH_SHADER + "fs.fragment.hlsl").c_str());

	Texture texture1(0, (PATH_TEXTURE + "container.jpg").c_str());
	Texture texture2(1, (PATH_TEXTURE + "awesomeface.png").c_str());
	
	testShader.exec();
	testShader.uniformSetInt("texture1", texture1.getTexId());
	testShader.uniformSetInt("texture2", texture2.getTexId());

	float step = 0.0;
	bool flag = true;
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // ���������Ļ������ʹ�õ���ɫ
		// �����Ļ������, ����һ������ָ��Ҫ��յĻ�����, ��ѡֵ:
		// GL_COLOR_BUFFER_BIT - ��ɫ������(������ɫ��������RGBA��ɫ����)
		// GL_DEPTH_BUFFER_BIT - ��Ȼ�����(����ÿ�����ص����ֵ)
		// GL_STENCIL_BUFFER_BIT - ģ�建����
		glClear(GL_COLOR_BUFFER_BIT);

		texture1.bind();
		texture2.bind();

		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.1f, 0.1f, 1.0f));
		trans = glm::scale(trans, glm::vec3(0.5, step, 0.5));
		testShader.uniformSetMatrix4fv("transform", 1, GL_FALSE, glm::value_ptr(trans));

		if (step >= 2) {
			step = 2.0;
			flag = false;
		}
		if (step <= 0) {
			step = 0.0;
			flag = true;
		}

		if (flag) {
			step += 0.01;
		} else {
			step -= 0.01;
		}

		testShader.exec();

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// ������ɫ������(������GLFW����ÿһ��������ɫֵ�Ĵ󻺳�), ������һ�����б���������, ���ҽ�����Ϊ�����ʾ����Ļ��
		// ˫����
		// ����: ��=>�� ��=>�� �����ػ���
		// ǰ���屣�����������ͼ��, ֱ������Ļ����ʾ
		// ������Ⱦָ���ں󻺳����, ��������Ⱦָ����ɺ�, ֱ�ӽ���ǰ����ͺ󻺳�
		glfwSwapBuffers(window);
		glfwPollEvents(); // ��ѯ�¼�
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);

	glfwTerminate();
	return 0;
}

void keyCallbackFn(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// ע��ESC����Ϊ�رմ���
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

void framebufferSizeCallbackFn(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
