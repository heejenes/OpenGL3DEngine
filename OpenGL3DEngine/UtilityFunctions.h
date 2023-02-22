#pragma once
#include <iostream>
#include <string>
#include <fstream>

void processInput(GLFWwindow* window) {
	// if user has pressed esc, close window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

static const char* extractVersion(const char* full) {
	std::string p1 = full;
	static std::string p2;
	p2.clear();
	p2 = p1.substr(
		p1.find_last_of("/\\") + 1,
		(p1.length() - p1.find_last_of("/\\")) - (p1.length() - (p1.find_last_of(".")) + 1)
	);
	return p2.c_str();
}

#ifdef _DEBUG
static void writeLog(const char* msg) {
	std::cout << msg << std::endl;
	std::ofstream logs;
	logs.open("LOGGER_OUTPUT.txt", std::ofstream::app | std::ofstream::out);
	logs << msg;
	logs.close();
}
#else
static void writeLog(const char* msg) {
}
#endif