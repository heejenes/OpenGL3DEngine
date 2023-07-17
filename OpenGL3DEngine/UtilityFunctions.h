#pragma once

#ifndef UTILITYFUNCTIONS_H
#define UTILITYFUNCTIONS_H

#include <iostream>
#include <string>
#include <fstream>

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

// Between -1.0 to 1.0
float GetRand() {
	float b = (((float)(rand() % 10000)) / 10000.f - 0.5f) * 2.f;
	return b;
}

glm::vec3 CrossProduct(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
	glm::vec3 aa = a - c;
	glm::vec3 bb = b - c;
	return glm::cross(aa, bb);
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
#endif