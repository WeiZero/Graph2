#include "stbiloader.h"
GLuint texnum = 0;
GLuint stbiloader::Gentexture(char *filename) 
{
	char* format = filename;
	unsigned int textureID = 0;
	while (*format != 0) format++;
	while (format != filename && *format != '.') format--;

	// load and create a texture 
	// -------------------------
	unsigned int texture1;
	// texture 1
	// ---------
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
											// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (strcmp(format, ".png") == 0)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	return texture1;
}

GLuint stbiloader::GenArray_tex(char const *filename,int rowCount,int colCount)
{
	unsigned int textureID;
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
											// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char *Data = stbi_load(filename, &width, &height, &nrChannels, 0);

	int subWidth = width / colCount;
	int subHeight = height / rowCount;

	float spriteCount = rowCount * colCount;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D_ARRAY, textureID);
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA32F, subWidth, subHeight, spriteCount);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glPixelStorei(GL_UNPACK_ROW_LENGTH, width);

	if (Data)
	{
		int layer = 0;
		for (int row = rowCount - 1; row >= 0; --row)
		{
			for (int col = 0; col < colCount; ++col)
			{
				GLubyte *data = Data + (row * width * subHeight + col * subWidth) * 4;
				glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, layer, subWidth, subHeight, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
				++layer;
				if (layer >= spriteCount)
				{
					break;
				}
			}
		}
		//glBindTexture(GL_TEXTURE_2D_ARRAY, textureID);
		//texnum++;	
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
		return -1;
	}
	stbi_image_free(Data);
	return textureID;
}