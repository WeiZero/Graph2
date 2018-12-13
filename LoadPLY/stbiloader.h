#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <iostream>
using namespace std;

#include "stb_image.h"

namespace stbiloader {
	GLuint Gentexture(char *filename);
	GLuint GenArray_tex(char const *filename, int rowCount, int colCount);
}