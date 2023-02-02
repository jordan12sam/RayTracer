#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cassert>

#define glWrap(x) glClearError();\
    x;\
    assert(glLogCall(#x, __FILE__, __LINE__));

void glClearError();

bool glLogCall(const char* function, const char* file, int line);