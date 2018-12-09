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


#define PLAYER_WIDTH 40.0
#define PLAYER_HEIGHT 40.0
#define Player_Move_Speed 20
#define Player_Speed 5.0
#define Player_JumpHeight 20



float currentTime, spritePlayTime, lastTime;
int spriteIndex = 0;