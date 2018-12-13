#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <vector>
using namespace std;

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "LoadShaders.h"
#include "stbiloader.h"
#include "point.h"

#define PLAYER_WIDTH 40.0
#define PLAYER_HEIGHT 40.0
#define Player_Move_Speed 20
#define Player_Speed 5.0
#define Player_JumpHeight 20

#define player_Width 40.0
#define player_Height 40.0
#define player_Move_Speed 20
#define player_Speed 5.0

GLuint _ArrayProgram;

GLuint vao, vbo;

// 人物操控
mat4 Model;
GLuint _Model;
float move_x = 0.0, jump_y = 0.0;
bool is_ground = 0;
int is_Move = 0;
bool moveRight = true;
bool moveNow = false;
int tempstate;
int state = 0;

// 視角設定
mat4 Projection;
mat4 View;
vec3 CameraPos;
GLuint _Comera, _Proj, _View;

// 紀錄資訊
Point player[4], player_UV[4];

// 圖片設定
GLuint playerArrayTex;
int SpriteIndex = 0;