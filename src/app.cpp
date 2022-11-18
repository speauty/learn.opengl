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

const std::string PATH_SHADER = ".\\assets\\shader\\";
const std::string PATH_TEXTURE = ".\\assets\\texture\\";

const unsigned int WIN_WIDTH = 800;
const unsigned int WIN_HEIGHT = 600;

// ���λ��
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// ʱ��
float deltaTime = 0.0f;	// ��ǰ֡����һ֡��ʱ���
float lastFrame = 0.0f; // ��һ֡��ʱ��

int main() {

	glfwInit(); // ��ʼ��glfw

	// ���������ɲο�: http://www.glfw.org/docs/latest/window.html#window_hints
	// ���opengl�汾Ϊ3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // ���openglΪ����ģʽ
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); // �������ڴ�С

	GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "O-Cat", NULL, NULL);
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
	// OpenGL�洢�������������Ϣ��һ��Z����(Z-buffer)�У�Ҳ����Ϊ��Ȼ���(Depth Buffer)
	// ��Ȳ���: ��Ƭ����Ҫ���������ɫʱ��OpenGL�Ὣ�������ֵ��z������бȽ�
	//			�����ǰ��Ƭ��������Ƭ��֮�������ᱻ���������򽫻Ḳ��
	// ������Ȳ���()
	glEnable(GL_DEPTH_TEST);

	std::cout << "״̬: ��ǰ����GL����: " << (unsigned char*)glGetString(GL_VENDOR) << std::endl;   // ����
	std::cout << "״̬: ��ǰ����GL��Ⱦ: " << (unsigned char*)glGetString(GL_RENDERER) << std::endl; // ��Ⱦ��
	std::cout << "״̬: ��ǰ����GL�汾: " << (unsigned char*)glGetString(GL_VERSION) << std::endl;  // �汾

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "״̬: ��ǰ����֧�������������: " << nrAttributes << std::endl;
	std::cout << std::endl;

	// ע���ӿڻص�, ע��: ��Ҫ��ϱ��GLFW_RESIZABLEΪGL_TRUE, ���ڴ�С��ֹ����ʱ, ����ص��ƺ�Ҳûɶ��
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallbackFn);

	// ����(λ������ ��ɫ ��������)
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = { // �����������
	  glm::vec3(0.0f,  0.0f,  0.0f),
	  glm::vec3(2.0f,  5.0f, -15.0f),
	  glm::vec3(-1.5f, -2.2f, -2.5f),
	  glm::vec3(-3.8f, -2.0f, -12.3f),
	  glm::vec3(2.4f, -0.4f, -3.5f),
	  glm::vec3(-1.7f,  3.0f, -7.5f),
	  glm::vec3(1.3f, -2.0f, -2.5f),
	  glm::vec3(1.5f,  2.0f, -2.5f),
	  glm::vec3(1.5f,  0.2f, -1.5f),
	  glm::vec3(-1.3f,  1.0f, -1.5f)
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
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); // ��Ԫ�ػ������
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// ���ö�������ָ��-����-λ��0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// ���ö�������ָ��-��ɫ-λ��1
	/*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);*/
	// ���ö�������ָ��-����-λ��2
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	Shader testShader;
	// @todo ����������޸�
	testShader.loadFromFile((PATH_SHADER + "vs.vertex.hlsl").c_str(), (PATH_SHADER + "fs.fragment.hlsl").c_str());

	Texture texture1(0, (PATH_TEXTURE + "cat.jpg").c_str());

	glm::mat4 projection = glm::mat4(1.0f); // ͶӰ����
	projection = glm::perspective(glm::radians(50.0f), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.5f, 100.0f);

	while (!glfwWindowShouldClose(window)) {

		// ����֡ʱ���
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// ע�ᰴ���ص�
		glfwSetKeyCallback(window, keyCallbackFn);

		glClearColor(0.2f, 0.7f, 0.1f, 0.0f); // ���������Ļ������ʹ�õ���ɫ
		// �����Ļ������, ����һ������ָ��Ҫ��յĻ�����, ��ѡֵ:
		// GL_COLOR_BUFFER_BIT - ��ɫ������(������ɫ��������RGBA��ɫ����)
		// GL_DEPTH_BUFFER_BIT - ��Ȼ�����(����ÿ�����ص����ֵ)
		// GL_STENCIL_BUFFER_BIT - ģ�建����
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		texture1.bind();

		testShader.exec();

		testShader.uniformSetInt("texture1", texture1.getTexId());

		// lookAt �����λ��(position)��Ŀ��λ��(target)������ռ��е�������(up)
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp); // �۲����

		testShader.uniformSetMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(view));
		testShader.uniformSetMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(vao);
		for (unsigned int i = 0; i < 10; i++) {
			// ����ÿ�����������������
			// ģ�;���(λ��, ����, ��ת) ���ֲ��ռ�䵽����ռ�
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;

			if (angle == 0) { angle = 10.0f; }
			
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			testShader.uniformSetMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


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

	// ������ƶ���������(WSAD)
	float cameraSpeed = static_cast<float>(10 * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cameraPos += cameraSpeed * cameraFront; // �������Զƽ����
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cameraPos -= cameraSpeed * cameraFront; // ��������ƽ����
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed; // ���������
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed; // ���������
}

void framebufferSizeCallbackFn(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
