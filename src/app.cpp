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

// 相机位置
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// 时间
float deltaTime = 0.0f;	// 当前帧和上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

int main() {

	glfwInit(); // 初始化glfw

	// 相关配置项可参考: http://www.glfw.org/docs/latest/window.html#window_hints
	// 标记opengl版本为3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 标记opengl为核心模式
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); // 调整窗口大小

	GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "O-Cat", NULL, NULL);
	if (NULL == window) {
		std::cout << "ERROR::INIT::MSG(创建窗口失败)" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	
	// glfwGetProcAddress 获取系统相关的OpenGL函数指针地址
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "ERROR::INIT::MSG(初始化GLAD失败)" << std::endl;
		glfwTerminate();
		return -1;
	}

	// 设置线框绘制模式
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// OpenGL存储它的所有深度信息于一个Z缓冲(Z-buffer)中，也被称为深度缓冲(Depth Buffer)
	// 深度测试: 当片段想要输出它的颜色时，OpenGL会将它的深度值和z缓冲进行比较
	//			如果当前的片段在其它片段之后，它将会被丢弃，否则将会覆盖
	// 启用深度测试()
	glEnable(GL_DEPTH_TEST);

	std::cout << "状态: 当前载入GL厂商: " << (unsigned char*)glGetString(GL_VENDOR) << std::endl;   // 厂商
	std::cout << "状态: 当前载入GL渲染: " << (unsigned char*)glGetString(GL_RENDERER) << std::endl; // 渲染器
	std::cout << "状态: 当前载入GL版本: " << (unsigned char*)glGetString(GL_VERSION) << std::endl;  // 版本

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "状态: 当前顶点支持属性最大数量: " << nrAttributes << std::endl;
	std::cout << std::endl;

	// 注册视口回调, 注意: 需要配合标记GLFW_RESIZABLE为GL_TRUE, 窗口大小禁止调整时, 这个回调似乎也没啥用
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallbackFn);

	// 顶点(位置坐标 颜色 纹理坐标)
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

	glm::vec3 cubePositions[] = { // 立方体的坐标
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

	unsigned int vbo, vao, ebo; // 顶点缓冲对象 顶点数组缓冲对象(GPU解释顶点数据) 元素缓冲对象
	glGenBuffers(1, &vbo); // 生成一个缓冲区
	glGenBuffers(1, &ebo);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao); // 绑定顶点数组对象

	glBindBuffer(GL_ARRAY_BUFFER, vbo); // 绑定到顶点数组缓冲区(允许同时绑定多个类型不同的缓冲)
	// 将顶点数据复制到已绑定好的缓冲区, 第四个参数为绘制模式, 主要有以下几个可选值:
	// 1. GL_STATIC_DRAW 数据不会或几乎不会改变 静态绘制
	// 2. GL_DYNAMIC_DRAW 数据会被改变很多 动态绘制
	// 3. GL_STREAM_DRAW 数据每次绘制时都会改变 流绘制
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 复制索引到元素缓冲区
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); // 绑定元素缓冲对象
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 设置顶点属性指针-坐标-位置0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 设置顶点属性指针-颜色-位置1
	/*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);*/
	// 设置顶点属性指针-纹理-位置2
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	Shader testShader;
	// @todo 这里这个待修复
	testShader.loadFromFile((PATH_SHADER + "vs.vertex.hlsl").c_str(), (PATH_SHADER + "fs.fragment.hlsl").c_str());

	Texture texture1(0, (PATH_TEXTURE + "cat.jpg").c_str());

	glm::mat4 projection = glm::mat4(1.0f); // 投影矩阵
	projection = glm::perspective(glm::radians(50.0f), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.5f, 100.0f);

	while (!glfwWindowShouldClose(window)) {

		// 计算帧时间差
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// 注册按键回调
		glfwSetKeyCallback(window, keyCallbackFn);

		glClearColor(0.2f, 0.7f, 0.1f, 0.0f); // 设置清除屏幕缓冲区使用的颜色
		// 清除屏幕缓冲区, 接收一个参数指定要清空的缓冲区, 可选值:
		// GL_COLOR_BUFFER_BIT - 颜色缓冲区(包含颜色索引或者RGBA颜色数据)
		// GL_DEPTH_BUFFER_BIT - 深度缓冲区(包含每个像素的深度值)
		// GL_STENCIL_BUFFER_BIT - 模板缓冲区
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		texture1.bind();

		testShader.exec();

		testShader.uniformSetInt("texture1", texture1.getTexId());

		// lookAt 摄像机位置(position)、目标位置(target)和世界空间中的上向量(up)
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp); // 观察矩阵

		testShader.uniformSetMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(view));
		testShader.uniformSetMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(vao);
		for (unsigned int i = 0; i < 10; i++) {
			// 计算每个立方体的世界坐标
			// 模型矩阵(位移, 缩放, 旋转) 将局部空间变到世界空间
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;

			if (angle == 0) { angle = 10.0f; }
			
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			testShader.uniformSetMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		// 交换颜色缓冲区(储存着GLFW窗口每一个像素颜色值的大缓冲), 它在这一迭代中被用来绘制, 并且将会作为输出显示在屏幕上
		// 双缓冲
		// 绘制: 左=>右 上=>下 逐像素绘制
		// 前缓冲保存最终输出的图像, 直接在屏幕上显示
		// 所有渲染指令在后缓冲绘制, 当所有渲染指令完成后, 直接交换前缓冲和后缓冲
		glfwSwapBuffers(window);
		glfwPollEvents(); // 轮询事件
	}

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);

	glfwTerminate();
	return 0;
}

void keyCallbackFn(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// 注册ESC按键为关闭窗口
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	// 摄像机移动按键控制(WSAD)
	float cameraSpeed = static_cast<float>(10 * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cameraPos += cameraSpeed * cameraFront; // 摄像机向远平面移
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cameraPos -= cameraSpeed * cameraFront; // 摄像机向近平面移
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed; // 摄像机右移
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed; // 摄像机左移
}

void framebufferSizeCallbackFn(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
