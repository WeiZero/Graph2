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

GLuint _ArrayProgram;

GLuint _RunArray, _IdleArray, _JumpTex;

float currentTime, spritePlayTime, lastTime;
int SpriteIndex = 0;

GLuint vao, vbo;

glm::mat4 Model;
GLuint _Model;
float move_x = 0.0, jump_y = 0.0;
bool is_ground = 0;
int is_Move = 0;
int tempstate;
int state = 0;

mat4 Projection;
mat4 View;
vec3 CameraPos;

GLuint _Comera, _Proj, _View;

Point Player[4], Playeruv[4];
glm::vec4 PlayerPos[4];

Point tempR[4], tempL[4];
Point nowground;