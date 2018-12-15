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
#define player_Height 30.0
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

// Tex Bind
GLuint vaoPlayer, vboPlayer;
GLuint vaoBlock, vboBlock;
GLuint vaoQuad, vboQuad;
GLuint vaoParticle, vboParticle;

// �H���ޱ�
mat4 Model[2];
GLuint _Model;
float move_x[2] = { 0.0 };
int moveRight[2] = { 1 };
bool moveNow[2] = { false };
int state[2] = { 0 };

// �����]�w
mat4 Projection;
mat4 View[2];
vec3 CameraPos;
GLuint _Camera, _Proj, _View;

// ������T
Point player[4];

// �Ϥ��]�w
GLuint playerArrayTex;
int SpriteIndex[2] = { 0 };
GLuint BGTex;
GLuint blockTex[6];
GLuint ParticleTex;
GLuint framebuffer[2], textureColorbuffer[2];
GLuint quadVAO, quadVBO;
GLuint screenTexture1, screenTexture2;

struct star_t
{
	glm::vec3     position;
	glm::vec3     color;
};

float quad[] = {
	//position		//UV
	-1, -1,		0, 0,
	 1, -1,		1, 0,
	 1,  1.5,		1, 1,

	-1, -1,		0, 0,
	 1,   1.5,		1, 1,
	-1,   1.5,		0, 1
};

class Block {
public:
	int index;	// �ĥΦ�ؤ���C��
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
		pos[0] = Point(x - block_Width, 300 - block_Height / 2);
		pos[1] = Point(x, 300 - block_Height / 2);
		pos[2] = Point(x, 300 + block_Height / 2);
		pos[3] = Point(x - block_Width, 300 + block_Height / 2);
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