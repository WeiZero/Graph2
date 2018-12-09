#include "vgl.h"
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <map>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <stdio.h>
using namespace std;

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
using namespace glm;

#include "LoadShaders.h"
#include "vmath.h"
#include "TextureApp.h"
#include "point.h"


#define player_WIDTH 40.0
#define player_HEIGHT 40.0
#define player_Move_Speed 20
#define player_Speed 5.0

float currentTime, spritePlayTime, lastTime;
int spriteIndex = 0;
GLuint program,programArray;
GLuint vao, vbo, Avao, Avbo;

// 人物操控
mat4 Model;
GLuint _Model;
float move_x = 0.0;
bool moveRight = true;
bool moveNow = false;
int state = 0;

// 圖片擺放
GLuint TexArray,Tex[7];
int texnum = 0;

// 視角設定
mat4 Projection;
mat4 View;
vec3 CameraPos;
GLuint _Camera, _Proj, _View;

// 紀錄資訊
Point player[4], player_UV[4];
vec4 playerPos[4];