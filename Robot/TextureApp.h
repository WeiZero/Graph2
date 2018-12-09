#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <string.h>
#include <vector>

#include <opencv\cv.h>
#include <opencv\highgui.h>

namespace TextureApp{
	bool	LoadPngImage(char *name, int &outWidth, int &outHeight, bool &outHasAlpha, GLubyte **outData);
	GLuint	GenTexture(char* filepath);
	GLuint GenTextureArray(char* filepath, int rowCount, int colCount);
	void ScreenShot(std::string& file);
}

