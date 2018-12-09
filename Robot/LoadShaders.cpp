#include "LoadShaders.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

GLuint LoadShaders(const char *vertex_file_path,const char *fragment_file_path){
	GLuint program;
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

	const char* vs_source;
	const char* fs_source;

	//讀取vertex shader code到string裡
	string VertexShaderCode;
	ifstream VertexShaderStream(vertex_file_path, ios::in);
	if(VertexShaderStream.is_open()){//檔案存在並成功開啟
		string Line = "";
		while(getline(VertexShaderStream,Line))
			VertexShaderCode += "\n" + Line;//行間要自己加上換行符號
		VertexShaderStream.close();
	}
	else{
		printf("Impossible to open %s. Are you in the right directory ?" 
			   "Don't forget to read the FAQ !\n", vertex_file_path);
		return 0;
	}

	// Read the Fragment Shader code from the file
	string FragmentShaderCode;
	ifstream FragmentShaderStream(fragment_file_path, ios::in);
	if(FragmentShaderStream.is_open()){
		string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}
	else{
		printf("Impossible to open %s. Are you in the right directory ?" 
			"Don't forget to read the FAQ !\n", fragment_file_path);
		return 0;
	}

	cout<<VertexShaderCode<<endl;
	cout<<FragmentShaderCode<<endl;
	
	//compile vertex shader
	vs_source = VertexShaderCode.c_str();
	glShaderSource(vs,1,&vs_source,NULL);
	glCompileShader(vs);
	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	fs_source = FragmentShaderCode.c_str();
	glShaderSource(fs, 1, &fs_source , NULL);
	glCompileShader(fs);

	GLint Result = GL_FALSE;
	int InfoLogLength;
#ifdef _DEBUG
	// Check Vertex Shader
	glGetShaderiv(vs, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		vector<char> VertexShaderErrorMessage(InfoLogLength+1);//須先include <vector>
		glGetShaderInfoLog(vs, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Check Fragment Shader
	glGetShaderiv(fs, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(fs, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}
#endif /* DEBUG */

	// Link the program
	printf("Linking program\n");
	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

#ifdef _DEBUG
	// Check the program
	glGetProgramiv(program, GL_LINK_STATUS, &Result);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(program, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}
#endif /* DEBUG */


	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}