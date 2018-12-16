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
#include <time.h>

#define player_Width 40.0
#define player_Height 40.0
#define player_Speed 5.0

#define block_Width 60.0
#define block_Height 60.0
#define block_Speed 10.0

#define NUM_STARS 500

// shader program
GLuint playerArray;
GLuint blockArray;
GLuint BG;
GLuint particle;
GLuint FBO;
GLuint Mirror;

// Tex Bind
GLuint vaoPlayer, vboPlayer;
GLuint vaoBlock, vboBlock;
GLuint vaoQuad, vboQuad;
GLuint vaoQuad2, vboQuad2;
GLuint vaoParticle, vboParticle;

// 人物操控
mat4 Model;
GLuint _Model;
float move_x =  0.0;
int moveRight =  1 ;
bool moveNow =  false ;
int state =  0;

// 視角設定
mat4 Projection;
mat4 View;
vec3 CameraPos;
GLuint _Camera, _Proj, _View;

// 紀錄資訊
Point player[4];

// 圖片設定
GLuint playerArrayTex;
int SpriteIndex =  0 ;
GLuint BGTex, BGTex2;
GLuint blockTex[6];
GLuint ParticleTex;
GLuint quadVAO, quadVBO;
GLuint framebuffer, textureColorbuffer, renderbuffer;
GLuint mquadVAO, mquadVBO;
GLuint mframebuffer, mtextureColorbuffer, mrenderbuffer;

struct star_t
{
	glm::vec3     position;
	glm::vec3     color;
};

float quad[] = {
	//position		//UV
	-1, -0.7,		0, 0,
	 1, -0.7,		1, 0,
	 1,  1.3,		1, 1,

	-1, -0.7,		0, 0,
	 1,   1.3,		1, 1,
	-1,   1.3,		0, 1
};
float quad2[] = {
	//position		//UV
	-1, -0.7,		0, 0,
	1, -0.7,		1, 0,
	1,  -0.5,		1, 1,

	-1, -0.7,		0, 0,
	1,   -0.5,		1, 1,
	-1,   -0.5,		0, 1
};

class Block {
public:
	int index;	// 採用何種方塊顏色
	Point pos[4];
	Block() {
		index = 0;
		pos[0] = Point(0 - block_Width, 0 - block_Height);
		pos[1] = Point(0 , 0 - block_Height);
		pos[2] = Point(0, 0);
		pos[3] = Point(0 - block_Width, 0);
	}
	Block(int in, int x) {
		index = in;
		pos[0] = Point(x , 300 - block_Height / 2);
		pos[1] = Point(x + block_Width, 300 - block_Height / 2);
		pos[2] = Point(x + block_Width, 300 + block_Height / 2);
		pos[3] = Point(x, 300 + block_Height / 2);
	}
	int downY(int y) {
		pos[0].y -= y;
		pos[1].y -= y;
		pos[2].y -= y;
		pos[3].y -= y;
		return pos[0].y;
	}
};

int blockNum = 0;
vector<Block> blockList;

enum GameState { PLAY, DEAD };
GameState gameState = PLAY;

int execAddGenBlockCount = 0;//增加產生方塊頻率計數器
int genBlock = 0;//產生方塊計數器
int genBlockCount = 20;//產生方塊頻率

int execAddSpeed = 0;//增加掉落速度計數器
float dropSpeed = 2.0f;//掉落速度

int clearBlock = 0;//清除方塊計數器

float accelerateCoeff = 1;