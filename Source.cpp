#include <GLFW/glf3.h>
#include "Callbacks.h"

int main(int argc, char** argv) {
	glfwSetErrorCallback(glfwErrorCallback);
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	GLFWwindow* window = glfwCtrateWindow(800, 600, argv[0], nullptr, nullptr);
	glfwMakeConetxtCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSetWindowCloserCallback(window, glfw_window_close_callback);
	while (!glfwWindowShouldClose(window)) {
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwTerminate();
	return 0;

}