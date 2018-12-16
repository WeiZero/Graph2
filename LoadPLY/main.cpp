#include "main.h"
static unsigned int seed = 0x13371337;
static inline float random_float()
{
	float res;
	unsigned int tmp;

	seed *= 16807;

	tmp = seed ^ (seed >> 4) ^ (seed << 15);

	*((unsigned int *)&res) = (tmp >> 9) | 0x3F800000;

	return (res - 1.0f);
}
void playerInit() {
	player[0] = Point(-300, -210);
	player[1] = Point(-300 + player_Width, -210);

	player[2] = Point(-300 + player_Width, -210 + player_Height);
	player[3] = Point(-300, -210 + player_Height);

	playerArrayTex = stbiloader::GenArray_tex("Texture/1x.png", 1, 7);
	float tri_pos[] = {
		//position					//UV
		player[0].x, player[0].y,	0, 0,
		player[1].x, player[1].y,	1, 0,
		player[2].x, player[2].y,	1, 1,

		player[0].x, player[0].y,	0, 0,
		player[2].x, player[2].y,	1, 1,
		player[3].x, player[3].y,	0, 1,
	};
	glUseProgram(playerArray);
	_Proj = glGetUniformLocation(playerArray, "proj");
	_View = glGetUniformLocation(playerArray, "view");
	glUniformMatrix4fv(_Proj, 1, GL_FALSE, &Projection[0][0]);
	glUniformMatrix4fv(_View, 1, GL_FALSE, &View[0][0]);


	glBindTexture(GL_TEXTURE_2D_ARRAY, playerArrayTex);
	glUniform1i(glGetUniformLocation(playerArray, "Array_tex"), 0);
	glUniform1i(glGetUniformLocation(playerArray, "dir"), moveRight);
	glGenVertexArrays(1, &vaoPlayer);
	glGenBuffers(1, &vboPlayer);
	glBindVertexArray(vaoPlayer);
	glBindBuffer(GL_ARRAY_BUFFER, vboPlayer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tri_pos), &tri_pos, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FLOAT, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}
void TexInit() {
	BGTex = stbiloader::Gentexture("Texture/bg.png");
	BGTex2 = stbiloader::Gentexture("Texture/brick.png");
	ParticleTex = stbiloader::Gentexture("Texture/effect.png");
	blockTex[0] = stbiloader::Gentexture("Texture/1.png");
	blockTex[1] = stbiloader::Gentexture("Texture/2.png");
	blockTex[2] = stbiloader::Gentexture("Texture/3.png");
	blockTex[3] = stbiloader::Gentexture("Texture/4.png");
	blockTex[4] = stbiloader::Gentexture("Texture/5.png");
	blockTex[5] = stbiloader::Gentexture("Texture/6.png");
}

void BgInit() {
	
	glUseProgram(BG);
	_Proj = glGetUniformLocation(BG, "proj");
	_View = glGetUniformLocation(BG, "view");

	glBindTexture(GL_TEXTURE_2D, BGTex);
	glUniformMatrix4fv(_Proj, 1, GL_FALSE, &Projection[0][0]);
	glUniformMatrix4fv(_View, 1, GL_FALSE, &View[0][0]);

	glGenVertexArrays(1, &vaoQuad);
	glGenBuffers(1, &vboQuad);
	glBindVertexArray(vaoQuad);
	glBindBuffer(GL_ARRAY_BUFFER, vboQuad);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), &quad, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FLOAT, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	
	glGenVertexArrays(1, &vaoQuad2);
	glGenBuffers(1, &vboQuad2);
	glBindVertexArray(vaoQuad2);
	glBindBuffer(GL_ARRAY_BUFFER, vboQuad2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad2), &quad2, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FLOAT, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void blockInit() {

	float tri_pos[] = { 0 };
	glUseProgram(blockArray);

	glGenVertexArrays(1, &vaoBlock);
	glGenBuffers(1, &vboBlock);
	glBindVertexArray(vaoBlock);
	glBindBuffer(GL_ARRAY_BUFFER, vboBlock);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tri_pos), &tri_pos, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FLOAT, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void ParticleInit() {

	glUseProgram(particle);
	_Proj = glGetUniformLocation(particle, "proj");
	_View = glGetUniformLocation(particle, "view");

	glUniformMatrix4fv(_Proj, 1, GL_FALSE, &Projection[0][0]);
	glUniformMatrix4fv(_View, 1, GL_FALSE, &View[0][0]);

	glGenVertexArrays(1, &vaoParticle);
	glGenBuffers(1, &vboParticle);
	glBindVertexArray(vaoParticle);
	glBindBuffer(GL_ARRAY_BUFFER, vboParticle);
	glBufferData(GL_ARRAY_BUFFER, NUM_STARS * sizeof(star_t), NULL, GL_STATIC_DRAW);
	star_t * star = (star_t *)glMapBufferRange(GL_ARRAY_BUFFER, 0, NUM_STARS * sizeof(star_t), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	

	for (int i = 0; i < NUM_STARS; i++)
	{
		star[i].position[0] = (random_float() * 2.0f - 1.0f) * 300.0f;
		star[i].position[1] = (random_float() * 2.0f - 1.0f) * 300.0f;
		star[i].position[2] = random_float();
		star[i].color[0] = 0.8f + random_float() * 0.2f;
		star[i].color[1] = 0.8f + random_float() * 0.2f;
		star[i].color[2] = 0.8f + random_float() * 0.2f;
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(star_t), NULL);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(star_t), (void *)sizeof(vec3));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

}

void MirrorInit() {

	glGenFramebuffers(1, &mframebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mframebuffer);
	// create a color attachment texture

	glGenTextures(1, &mtextureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, mtextureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mtextureColorbuffer, 0);
	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	glGenRenderbuffers(1, &mrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, mrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mrenderbuffer);
	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
							 // positions   // texCoords
		-1,  -0.65,		0, 0.13,
		 1,  -0.65,		1, 0.13,
		 1,  -1,		1, 0.3,

		-1,	 -0.65,		0, 0.13,
		 1,  -1,		1, 0.3,
		-1,  -1,		0, 0.3
	};
	glUseProgram(Mirror);
	// screen quad VAO
	glGenVertexArrays(1, &mquadVAO);
	glGenBuffers(1, &mquadVBO);
	glBindVertexArray(mquadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mquadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	// Get the uniform variables location. You've probably already done that before...
}

void FBOInit() {

	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// create a color attachment texture

	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	glGenRenderbuffers(1, &renderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);
	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
							 // positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};
	glUseProgram(FBO);
	// screen quad VAO
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	// Get the uniform variables location. You've probably already done that before...
}

void init() {
	srand(time(NULL));
	glClearColor(0.0, 0.0, 0.0, 1);//black screen
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	CameraPos = vec3(0.0f, 0.0f, 10.0f);
	Projection = ortho(-300.0f, 300.0f, -300.0f, 300.0f, 0.1f, 100.0f);
	View = lookAt(CameraPos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

	ShaderInfo playerArrayShader[] = {
		{ GL_VERTEX_SHADER, "Shader/playerArray.vs" },//vertex shader
		{ GL_FRAGMENT_SHADER, "Shader/playerArray.fs" },//fragment shader
		{ GL_NONE, NULL } };
	ShaderInfo blockArrayShader[] = {
		{ GL_VERTEX_SHADER, "Shader/block.vs" },//vertex shader
		{ GL_FRAGMENT_SHADER, "Shader/block.fs" },//fragment shader
		{ GL_NONE, NULL } };
	ShaderInfo backgroundShader[] = {
		{ GL_VERTEX_SHADER, "Shader/background.vs" },//vertex shader
		{ GL_FRAGMENT_SHADER, "Shader/background.fs" },//fragment shader
		{ GL_NONE, NULL } };
	ShaderInfo particleShader[] = {
		{ GL_VERTEX_SHADER, "Shader/particle.vs" },//vertex shader
		{ GL_FRAGMENT_SHADER, "Shader/particle.fs" },//fragment shader
		{ GL_NONE, NULL } };
	ShaderInfo MirrorShader[] = {
		{ GL_VERTEX_SHADER, "Shader/FBO.vs" },//vertex shader
		{ GL_FRAGMENT_SHADER, "Shader/FBO.fs" },//fragment shader
		{ GL_NONE, NULL } };
	ShaderInfo FBOShader[] = {
		{ GL_VERTEX_SHADER, "Shader/FBO.vs" },//vertex shader
		{ GL_FRAGMENT_SHADER, "Shader/GrayScale.fs" },//fragment shader
		{ GL_NONE, NULL } };

	playerArray = LoadShaders(playerArrayShader);
	blockArray = LoadShaders(blockArrayShader);
	BG = LoadShaders(backgroundShader);
	particle = LoadShaders(particleShader);
	Mirror = LoadShaders(MirrorShader);
	FBO = LoadShaders(FBOShader);

	TexInit();	
	playerInit();
	blockInit();
	BgInit();
	ParticleInit();
	MirrorInit();
	FBOInit();
}

#define DOR(angle) (angle*3.1415/180);
mat4 translate(float x, float y, float z) {
	vec4 t = vec4(x, y, z, 1);//w = 1 ,則x,y,z=0時也能translate
	vec4 c1 = vec4(1, 0, 0, 0);
	vec4 c2 = vec4(0, 1, 0, 0);
	vec4 c3 = vec4(0, 0, 1, 0);
	mat4 M = mat4(c1, c2, c3, t);
	return M;
}
mat4 scale(float x, float y, float z) {
	vec4 c1 = vec4(x, 0, 0, 0);
	vec4 c2 = vec4(0, y, 0, 0);
	vec4 c3 = vec4(0, 0, z, 0);
	vec4 c4 = vec4(0, 0, 0, 1);
	mat4 M = mat4(c1, c2, c3, c4);
	return M;
}
mat4 rotate(float angle, float x, float y, float z) {
	float r = DOR(angle);
	mat4 M = mat4(1);
	vec4 c1 = vec4(cos(r) + (1 - cos(r))*x*x, (1 - cos(r))*y*x + sin(r)*z, (1 - cos(r))*z*x - sin(r)*y, 0);
	vec4 c2 = vec4((1 - cos(r))*y*x - sin(r)*z, cos(r) + (1 - cos(r))*y*y, (1 - cos(r))*z*y + sin(r)*x, 0);
	vec4 c3 = vec4((1 - cos(r))*z*x + sin(r)*y, (1 - cos(r))*z*y - sin(r)*x, cos(r) + (1 - cos(r))*z*z, 0);
	vec4 c4 = vec4(0, 0, 0, 1);
	M = mat4(c1, c2, c3, c4);
	return M;
}
void updateModels() {
	Model = glm::mat4(1.0f);
	Model = translate(move_x, 0, 0.0);
}

void DrawPlayer()
{
		glUseProgram(playerArray);
		
		_View = glGetUniformLocation(playerArray, "view");
		_Model = glGetUniformLocation(playerArray, "model");

		glUniformMatrix4fv(_View, 1, GL_FALSE, &View[0][0]);
		glUniformMatrix4fv(_Model, 1, GL_FALSE, &Model[0][0]);

		if (state == 0)
			glUniform1i(glGetUniformLocation(playerArray, "SpriteIndex"), 0);
		else if (state == 1)
			glUniform1i(glGetUniformLocation(playerArray, "SpriteIndex"), SpriteIndex);

		float tri_pos[] = {
			//position					//UV
			player[0].x, player[0].y,	0, 0,
			player[1].x, player[1].y,	1, 0,
			player[2].x, player[2].y,	1, 1,

			player[0].x, player[0].y,	0, 0,
			player[2].x, player[2].y,	1, 1,
			player[3].x, player[3].y,	0, 1,
		};

		glBindVertexArray(vaoPlayer);
		glUniform1i(glGetUniformLocation(playerArray, "dir"), moveRight);
		glDrawArrays(GL_TRIANGLES, 0, 3 * 2);
	
}

void Drawblock() {
	for (int i = 0; i < blockList.size(); i++) {
			//cout << blockList.size() << endl;
			glUseProgram(blockArray);
			_Proj = glGetUniformLocation(blockArray, "proj");
			_View = glGetUniformLocation(blockArray, "view");
			glUniformMatrix4fv(_Proj, 1, GL_FALSE, &Projection[0][0]);
			glUniformMatrix4fv(_View, 1, GL_FALSE, &View[0][0]);

			float tri_pos[] = {
				//position					//UV
				blockList[i].pos[0].x, blockList[i].pos[0].y,  0, 0,
				blockList[i].pos[1].x, blockList[i].pos[1].y,  1, 0,
				blockList[i].pos[2].x, blockList[i].pos[2].y,  1, 1,

				blockList[i].pos[0].x, blockList[i].pos[0].y,  0, 0,
				blockList[i].pos[2].x, blockList[i].pos[2].y,  1, 1,
				blockList[i].pos[3].x, blockList[i].pos[3].y,  0, 1,
			};

			glBindVertexArray(vaoBlock);
			glBindBuffer(GL_ARRAY_BUFFER, vboBlock);
			glBufferData(GL_ARRAY_BUFFER, sizeof(tri_pos), &tri_pos, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0 * sizeof(float)));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FLOAT, 4 * sizeof(float), (void*)(2 * sizeof(float)));

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, blockTex[blockList[i].index]);
			glUniform1i(glGetUniformLocation(blockTex[blockList[i].index], "Tex"), 0);
			glBindVertexArray(vaoBlock);
			glDrawArrays(GL_TRIANGLES, 0, 3 * 2);
	}
}

void DrawBG() {

	glUseProgram(BG);

	glBindVertexArray(vaoQuad);
	glBindTexture(GL_TEXTURE_2D, BGTex);
	glDrawArrays(GL_TRIANGLES, 0, 3 * 2);
	glBindVertexArray(vaoQuad2);
	glBindTexture(GL_TEXTURE_2D, BGTex2);
	glDrawArrays(GL_TRIANGLES, 0, 3 * 2);

}


void DrawParticle() {

	glUseProgram(particle);
	float f_timer_cnt = glutGet(GLUT_ELAPSED_TIME);
	float currentTime = f_timer_cnt * 0.001f;

	currentTime *= 0.1f;
	currentTime -= floor(currentTime);

	glUniform1f(glGetUniformLocation(particle, "time"), currentTime);

	glEnable(GL_POINT_SPRITE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(vaoParticle);
	glBindTexture(GL_TEXTURE_2D, ParticleTex);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glDrawArrays(GL_POINTS, 0, NUM_STARS);
}

void DrawMirror() {

	glUseProgram(Mirror);
	glBindVertexArray(mquadVAO);
	glBindTexture(GL_TEXTURE_2D, mtextureColorbuffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void DrawScreen() {
	glUseProgram(FBO);
	if (gameState == PLAY)
		glUniform1f(glGetUniformLocation(FBO, "dead"), 0);
	else
		glUniform1f(glGetUniformLocation(FBO, "dead"), 1);
	glBindVertexArray(quadVAO);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void display() {
	glBindFramebuffer(GL_FRAMEBUFFER, mframebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	updateModels();

	DrawPlayer();
	Drawblock();
	
	DrawBG();
	if(gameState==PLAY)
		DrawParticle();
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer); // back to default
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	DrawMirror();
	DrawBG();
	Drawblock();
	DrawPlayer();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	DrawScreen();
	
	if(gameState==PLAY)
		glFlush();//強制執行上次的OpenGL commands
	glutSwapBuffers();//調換前台和後台buffer ,當後臺buffer畫完和前台buffer交換使我們看見它
}


void PressKey(unsigned char key, int x, int y) {

	switch (key)
	{
	case 'd':
		if (gameState == PLAY){
			moveRight = 1;
			moveNow = true;
			state = 1;
		}
		break;
	case 'a':
		if (gameState == PLAY) {
			moveRight = 0;
			moveNow = true;
			state = 1;
		}
		break;
	case 's':
		if (gameState == PLAY) {
		moveNow = false;
		state = 0;
		}
		break;
	case 'r':
		if (gameState == DEAD)
		{
			gameState = PLAY;
			blockList.clear();
			move_x = 0;
			moveNow = false;
			state = 0;
			genBlockCount = 20;
			dropSpeed = 2.0f;
		}
		break;
	case 'f':
		if (gameState == DEAD)
		{
			gameState = PLAY;
			blockList.clear();
			move_x = 0;
			moveNow = false;
			state = 0;
		}
		break;
	case VK_SPACE:
		accelerateCoeff = 1.6;
		break;
	}
}
void KeyUp(unsigned char key, int x, int y) {
	switch (key)
	{
	case VK_SPACE:
		accelerateCoeff = 1;
		break;
	}
}
bool isCollide()
{
	int overlap = false;
	int number = -1;
	int bias = 5;
	float player_x = move_x - 300.0f;
	float player_y = player[0].y;
	for (int i = 0; i < blockList.size(); i++)
	{
		if (player_x + player_Width - bias > blockList[i].pos[0].x &&
			player_x + player_Width - bias< blockList[i].pos[0].x + block_Width &&
			player[0].y + player_Height > blockList[i].pos[0].y &&
			player[0].y + player_Height < blockList[i].pos[0].y + block_Height)
			overlap = 1;
		if (player_x + bias > blockList[i].pos[0].x &&
			player_x + bias < blockList[i].pos[0].x + block_Width &&
			player[0].y + player_Height > blockList[i].pos[0].y &&
			player[0].y + player_Height < blockList[i].pos[0].y + block_Height)
			overlap = 2;
		if (player_x + player_Width - bias> blockList[i].pos[0].x &&
			player_x + player_Width - bias< blockList[i].pos[0].x + block_Width &&
			player[0].y > blockList[i].pos[0].y &&
			player[0].y  < blockList[i].pos[0].y + block_Height)
			overlap = 3;
		if (player_x + bias > blockList[i].pos[0].x &&
			player_x + bias < blockList[i].pos[0].x + block_Width &&
			player[0].y  > blockList[i].pos[0].y &&
			player[0].y  < blockList[i].pos[0].y + block_Height)
			overlap = 4;
		if (overlap > 0)
		{
			/*cout << i << endl;
			cout << overlap << endl;
			cout << player_x <<" "<<player[0].y<<endl;
			cout << blockList[i].pos[0].x << " " << blockList[i].pos[0].y << endl;*/
			return true;
		}
	}
		return false;
}

void Timer(int x) {
	if (gameState == PLAY)
	{
		execAddGenBlockCount++;
		if (execAddGenBlockCount > 250)//5秒
		{
			if(genBlockCount > 1)
				genBlockCount--;
			execAddGenBlockCount = 0;
		}

		genBlock++;
		if (genBlock > genBlockCount) {
			int s = rand() % 10 * block_Width - 300;
			if (blockNum >= 6)
				blockNum = 0;
			Block block(blockNum, s);
			blockList.push_back(block);
			blockNum++;
			genBlock=0;
		}

		execAddSpeed++;
		if (execAddSpeed > 10)
		{
			dropSpeed += 0.02f;
			execAddSpeed = 0;
		}
		for (int i = 0; i < blockList.size(); i++)
			blockList[i].downY(dropSpeed);

		clearBlock++;
		if (clearBlock > 5)
		{
			for (int i = 0; i < blockList.size(); i++) {
				if (blockList[i].pos[0].y <= -210) {
					blockList.erase(blockList.begin() + i);
					i--;
				}
			}
			clearBlock = 0;
		}

		switch (state)
		{
		case 0:
			SpriteIndex = 0;
			break;
		case 1:
			if (moveRight == 1 && moveNow)
				move_x += ((move_x + player_Speed*accelerateCoeff)
					< 570 ? player_Speed*accelerateCoeff : 0);
			else if (moveNow)
				move_x -= ((move_x - player_Speed*accelerateCoeff)
					>= -5 ? player_Speed*accelerateCoeff : 0);
			SpriteIndex = (SpriteIndex + 1 >= 6) ? 1 : SpriteIndex + 1;
			break;
		}

		if (isCollide())
		{
			gameState = DEAD;
			cout << "You Died!" << endl;
			cout << "With " << 1000 / (20 * genBlockCount)
				 << " BlockCount per Second," << endl;
			cout << "And " << dropSpeed << " DropSpeed!" << endl;
			cout << "Press R to Restart or F to Continue." << endl << endl;
		}
	}
	glutTimerFunc(20, Timer,x);
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("Project2");

	glewExperimental = GL_TRUE; //置於glewInit()之前
	if (glewInit()) {
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;//c error
		exit(EXIT_FAILURE);
	}

	cout << "Press Space to Acclerate!" << endl << endl;
	glutDisplayFunc(display);
	glutKeyboardFunc(PressKey);
	glutKeyboardUpFunc(KeyUp);
	glutTimerFunc(30, Timer, 0);

	init();
	glutMainLoop();

	system("Pause");
	return 0;
}
