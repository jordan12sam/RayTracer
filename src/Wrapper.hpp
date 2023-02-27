#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cassert>

#define glWrap(x) x;

void glClearError();

bool glLogCall(const char* function, const char* file, int line);