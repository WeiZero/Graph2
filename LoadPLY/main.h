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

#define player_Width 40.0
#define player_Height 40.0
#define player_Speed 5.0

#define block_Width 30.0
#define block_Height 40.0
#define block_Speed 10.0

GLuint playerArray;
GLuint blockArray;
GLuint BG;
GLuint vao, vbo;
GLuint vaoQuad, vboQuad;
// 人物操控
mat4 Model;
GLuint _Model;
float move_x = 0.0;
bool moveRight = true;
bool moveNow = false;
int state = 0;

// 視角設定
mat4 Projection;
mat4 View;
vec3 CameraPos;
GLuint _Camera, _Proj, _View;

// 紀錄資訊
Point player[4], player_UV[4];

// 圖片設定
GLuint playerArrayTex;
int SpriteIndex = 0;
GLuint blockTex[6];
GLuint BGTex;


float quad[] = {
	//position		//UV
	-1, -1,		0, 0,
	 1, -1,		1, 0,
	 1,  1,		1, 1,

	-1, -1,		0, 0,
	 1,  1,		1, 1,
	-1,  1,		0, 1
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
		pos[0] = Point(x - block_Width, 300 - block_Height);
		pos[1] = Point(x, 300 - block_Height);
		pos[2] = Point(x, 300);
		pos[3] = Point(x - block_Width, 300);
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