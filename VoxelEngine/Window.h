#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec4.hpp>
#include <iostream>

struct WinSize {
	int width;
	int height;
};

class Window {
public:
	Window() {}

	Window(const char* name, int w, int h) {
		glfwInit();
		aspct = (float)w / h;
		window = glfwCreateWindow(w, h, name, nullptr, nullptr);
		if (!window) {
			std::cerr << "Failed to create GLFW window\n";
			exit(1);
		}
		glfwMakeContextCurrent(window);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cerr << "Failed to initialize GLAD\n";
			exit(1);
		}

		glViewport(0, 0, w, h);
		glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, int w, int h) { glViewport(0, 0, w, h); aspct = (float)w / h; dirty = true; });
	}

	void setColor(const glm::vec4& color) {
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void clearBuffers() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	bool isOpened() {
		return !glfwWindowShouldClose(window);
	}

	void swapBuffers() {
		glfwSwapBuffers(window);
	}

	void poll() {
		glfwPollEvents();
	}

	void terminate() {
		glfwTerminate();
	}

	float aspect() {
		return aspct;
	}

	WinSize getSize() {
		glfwGetFramebufferSize(window, &winSize.width, &winSize.height);
		return winSize;
	}

	GLFWwindow* getParentWindow() {
		return window;
	}

	bool isDirty() {
		return dirty;
	}

	void setDirty(bool state) {
		dirty = state;
	}

private:
	WinSize winSize{};
	GLFWwindow* window;
	static float aspct;
	static bool dirty;

	void initGL() {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	}

};

inline float Window::aspct = -1.f;
inline bool Window::dirty = true;