#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <render/shader/shader.h>

void keyCallbackFn(GLFWwindow* window, int key, int scancode, int action, int mode);
void framebufferSizeCallbackFn(GLFWwindow* window, int width, int height);

const std::string PATH_SHADER = "E:\\MonkeyCode\\github.com\\speauty\\learn.opengl\\assets\\shader\\";

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

	// ע�ᰴ���ص�
	glfwSetKeyCallback(window, keyCallbackFn);
	// ע���ӿڻص�, ע��: ��Ҫ��ϱ��GLFW_RESIZABLEΪGL_TRUE, ���ڴ�С��ֹ����ʱ, ����ص��ƺ�Ҳûɶ��
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallbackFn);

	// ����(λ������ ��ɫ ��������)
	float vertices[] = {
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // ����
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,// �Ҷ���
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,// �󶥵�
	};

	unsigned int indices[] = { // ����
		0, 1, 2, // ��һ��������
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// ���ö�������ָ��-��ɫ-λ��1
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	Shader testShader;
	// @todo ����������޸�
	testShader.loadFromFile((PATH_SHADER + "vs.vertex.hlsl").c_str(), (PATH_SHADER + "fs.fragment.hlsl").c_str());

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // ���������Ļ������ʹ�õ���ɫ
		// �����Ļ������, ����һ������ָ��Ҫ��յĻ�����, ��ѡֵ:
		// GL_COLOR_BUFFER_BIT - ��ɫ������(������ɫ��������RGBA��ɫ����)
		// GL_DEPTH_BUFFER_BIT - ��Ȼ�����(����ÿ�����ص����ֵ)
		// GL_STENCIL_BUFFER_BIT - ģ�建����
		glClear(GL_COLOR_BUFFER_BIT);

		testShader.exec();

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		glfwPollEvents(); // ��ѯ�¼�
		// ������ɫ������(������GLFW����ÿһ��������ɫֵ�Ĵ󻺳�), ������һ�����б���������, ���ҽ�����Ϊ�����ʾ����Ļ��
		// ˫����
		// ����: ��=>�� ��=>�� �����ػ���
		// ǰ���屣�����������ͼ��, ֱ������Ļ����ʾ
		// ������Ⱦָ���ں󻺳����, ��������Ⱦָ����ɺ�, ֱ�ӽ���ǰ����ͺ󻺳�
		glfwSwapBuffers(window);
	}

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
