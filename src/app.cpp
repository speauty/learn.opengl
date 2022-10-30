#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

	unsigned char* glVersion;
	glVersion = (unsigned char*)glGetString(GL_VERSION);
	// 状态: 当前载入GL 3.3.0 NVIDIA 512.72
	std::cout << "状态: 当前载入GL " << glVersion << std::endl;

	// 注册按键回调
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mode)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) // 注册ESC按键为关闭窗口
			glfwSetWindowShouldClose(window, GL_TRUE);
	});

	// 注册视口回调, 注意: 需要配合标记GLFW_RESIZABLE为GL_TRUE, 窗口大小禁止调整时, 这个回调似乎也没啥用
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	});

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 设置清除屏幕缓冲区使用的颜色
		// 清除屏幕缓冲区, 接收一个参数指定要清空的缓冲区, 可选值:
		// GL_COLOR_BUFFER_BIT - 颜色缓冲区(包含颜色索引或者RGBA颜色数据)
		// GL_DEPTH_BUFFER_BIT - 深度缓冲区(包含每个像素的深度值)
		// GL_STENCIL_BUFFER_BIT - 模板缓冲区
		glClear(GL_COLOR_BUFFER_BIT);

		// 交换颜色缓冲区(储存着GLFW窗口每一个像素颜色值的大缓冲), 它在这一迭代中被用来绘制, 并且将会作为输出显示在屏幕上
		// 双缓冲
		// 绘制: 左=>右 上=>下 逐像素绘制
		// 前缓冲保存最终输出的图像, 直接在屏幕上显示
		// 所有渲染指令在后缓冲绘制, 当所有渲染指令完成后, 直接交换前缓冲和后缓冲
		glfwSwapBuffers(window);
		glfwPollEvents(); // 轮询事件
	}

	glfwTerminate();
	return 0;
}