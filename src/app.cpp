#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

	unsigned char* glVersion;
	glVersion = (unsigned char*)glGetString(GL_VERSION);
	// ״̬: ��ǰ����GL 3.3.0 NVIDIA 512.72
	std::cout << "״̬: ��ǰ����GL " << glVersion << std::endl;

	// ע�ᰴ���ص�
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mode)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) // ע��ESC����Ϊ�رմ���
			glfwSetWindowShouldClose(window, GL_TRUE);
	});

	// ע���ӿڻص�, ע��: ��Ҫ��ϱ��GLFW_RESIZABLEΪGL_TRUE, ���ڴ�С��ֹ����ʱ, ����ص��ƺ�Ҳûɶ��
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	});

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // ���������Ļ������ʹ�õ���ɫ
		// �����Ļ������, ����һ������ָ��Ҫ��յĻ�����, ��ѡֵ:
		// GL_COLOR_BUFFER_BIT - ��ɫ������(������ɫ��������RGBA��ɫ����)
		// GL_DEPTH_BUFFER_BIT - ��Ȼ�����(����ÿ�����ص����ֵ)
		// GL_STENCIL_BUFFER_BIT - ģ�建����
		glClear(GL_COLOR_BUFFER_BIT);

		// ������ɫ������(������GLFW����ÿһ��������ɫֵ�Ĵ󻺳�), ������һ�����б���������, ���ҽ�����Ϊ�����ʾ����Ļ��
		// ˫����
		// ����: ��=>�� ��=>�� �����ػ���
		// ǰ���屣�����������ͼ��, ֱ������Ļ����ʾ
		// ������Ⱦָ���ں󻺳����, ��������Ⱦָ����ɺ�, ֱ�ӽ���ǰ����ͺ󻺳�
		glfwSwapBuffers(window);
		glfwPollEvents(); // ��ѯ�¼�
	}

	glfwTerminate();
	return 0;
}