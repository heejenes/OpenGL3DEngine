#pragma once
#include <cstdio>
#include "UtilityFunctions.h"

static void glfwErrorCallback(int error, const char* description) {
	writeLog(description);
	throw("glfw error");
}

static void glfwWindowCloseCallback(GLFWwindow* window) {
	writeLog("window closed\n");
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}