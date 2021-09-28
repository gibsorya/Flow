#pragma once

#include <GLFW/glfw3.h>
#include "flow/flow.hpp"

/*

GLFW Window and it's data

*/
typedef struct FlowWindow FlowWindow;
struct FlowWindow {

	[[nodiscard]] GLFWwindow* getWindow() const {
		return window;
	}
	GLFWwindow* getWindow() {
		return window;
	}

	void setWindow(GLFWwindow* window) {
		this->window = window;
	}

	GLFWwindow* createWindow() {
		return glfwCreateWindow(800, 600, "Flow Engine", nullptr, nullptr);
	}

	const u32 WIDTH = 800;
	const u32 HEIGHT = 600;

	GLFWwindow* window;
private:
	

};