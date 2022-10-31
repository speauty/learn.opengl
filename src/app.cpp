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

	glfwInit(); // 初始化glfw

	// 相关配置项可参考: http://www.glfw.org/docs/latest/window.html#window_hints
	// 标记opengl版本为3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 标记opengl为核心模式
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); // 调整窗口大小

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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

	unsigned char* glVersion;
	glVersion = (unsigned char*)glGetString(GL_VERSION);
	// 状态: 当前载入GL 3.3.0 NVIDIA 512.72
	std::cout << "状态: 当前载入GL " << glVersion << std::endl;
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "状态: 当前顶点支持属性最大数量: " << nrAttributes << std::endl;

	// 注册按键回调
	glfwSetKeyCallback(window, keyCallbackFn);
	// 注册视口回调, 注意: 需要配合标记GLFW_RESIZABLE为GL_TRUE, 窗口大小禁止调整时, 这个回调似乎也没啥用
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallbackFn);

	// 顶点(位置坐标 颜色 纹理坐标)
	float vertices[] = {
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 2.0f, 2.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 2.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,2.0f,
	};

	unsigned int indices[] = { // 索引
		0, 1, 3,
		1, 2, 3,
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
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); // 绑定元素缓冲对象
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 设置顶点属性指针-坐标-位置0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 设置顶点属性指针-颜色-位置1
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);
	// 设置顶点属性指针-纹理-位置2
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 6));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	Shader testShader;
	// @todo 这里这个待修复
	testShader.loadFromFile((PATH_SHADER + "vs.vertex.hlsl").c_str(), (PATH_SHADER + "fs.fragment.hlsl").c_str());

	Texture texture1(0, (PATH_TEXTURE + "container.jpg").c_str());
	Texture texture2(1, (PATH_TEXTURE + "awesomeface.png").c_str());
	
	testShader.exec();
	testShader.uniformSetInt("texture1", texture1.getTexId());
	testShader.uniformSetInt("texture2", texture2.getTexId());

	float step = 0.0;
	bool flag = true;
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 设置清除屏幕缓冲区使用的颜色
		// 清除屏幕缓冲区, 接收一个参数指定要清空的缓冲区, 可选值:
		// GL_COLOR_BUFFER_BIT - 颜色缓冲区(包含颜色索引或者RGBA颜色数据)
		// GL_DEPTH_BUFFER_BIT - 深度缓冲区(包含每个像素的深度值)
		// GL_STENCIL_BUFFER_BIT - 模板缓冲区
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
}

void framebufferSizeCallbackFn(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
