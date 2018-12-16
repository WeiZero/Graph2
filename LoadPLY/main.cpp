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
	blockTex[0] = stbiloader::Gentexture("Texture/01.png");
	blockTex[1] = stbiloader::Gentexture("Texture/02.png");
	blockTex[2] = stbiloader::Gentexture("Texture/03.png");
	blockTex[3] = stbiloader::Gentexture("Texture/04.png");
	blockTex[4] = stbiloader::Gentexture("Texture/05.png");
	blockTex[5] = stbiloader::Gentexture("Texture/06.png");
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

void ScreenInit() {

	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
							 // positions   // texCoords
		-1,  -1,		0, 0,
		 1,  -1,		1, 0,
		 1,  -0.7,		1, 1,

		-1,	 -1,		0, 0,
		 1,  -0.7,		1, 1,
		-1,  -0.7,		0, 1
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
	screenTexture1 = glGetUniformLocation(FBO, "screenTexture1");
	// Then bind the uniform samplers to texture units:
	
	glUniform1i(screenTexture1, 0);
}

void init() {
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
	ShaderInfo FBOShader[] = {
		{ GL_VERTEX_SHADER, "Shader/FBO.vs" },//vertex shader
		{ GL_FRAGMENT_SHADER, "Shader/FBO.fs" },//fragment shader
		{ GL_NONE, NULL } };
	playerArray = LoadShaders(playerArrayShader);
	blockArray = LoadShaders(blockArrayShader);
	BG = LoadShaders(backgroundShader);
	particle = LoadShaders(particleShader);
	FBO = LoadShaders(FBOShader);

	TexInit();	
	playerInit();
	blockInit();
	BgInit();
	ParticleInit();
	ScreenInit();
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
	glBindVertexArray(vaoQuad2);
	glBindTexture(GL_TEXTURE_2D, BGTex2);
	glDrawArrays(GL_TRIANGLES, 0, 3 * 2);
	glBindVertexArray(vaoQuad);
	glBindTexture(GL_TEXTURE_2D, BGTex);
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

void DrawScreen() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glUseProgram(FBO);
	glBindVertexArray(quadVAO);
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer[0]);	// use the color attachment texture as the texture of the quad plane
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer[1]);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	updateModels();

	DrawPlayer();
	Drawblock();
	
	DrawBG();
	DrawParticle();
	DrawScreen();

	glFlush();//強制執行上次的OpenGL commands
	glutSwapBuffers();//調換前台和後台buffer ,當後臺buffer畫完和前台buffer交換使我們看見它
}


void PressKey(unsigned char key, int x, int y) {

	switch (key)
	{
	case 'd':
		moveRight = 1;
		moveNow = true;
		state = 1;
		break;
	case 'a':
		moveRight = 0;
		moveNow = true;
		state = 1;
		break;
	case 's':
		moveNow = false;
		state = 0;
		break;
	}
}

int Xpos = 0;
void Timer(int x) {

	if (x > 5) {
		int s = (Xpos + 1) * block_Width - 300;
		if (blockNum >= 6)
			blockNum = 0;
		Block block(blockNum, s);
		blockList.push_back(block);
		blockNum++;
		for (int i = 0; i < blockList.size(); i++) {
			if (blockList[i].pos[0].y <= -330) {
				blockList.erase(blockList.begin() + i);
				i--;
			}
		}
		x = 0;

		Xpos++;
		if (Xpos > 9)
			Xpos = 0;
	}
	for (int i = 0; i < blockList.size(); i++) 
		blockList[i].downY(5);

 	switch (state)
 	{
 		case 0:
			SpriteIndex = 0;
 			break;
 		case 1:
			if (moveRight == 1 && moveNow)
				move_x += player_Speed;
			else if (moveNow)
				move_x -= player_Speed;
			SpriteIndex = (SpriteIndex + 1 >= 6) ? 1 : SpriteIndex + 1;
 				break;
 		}

	glutTimerFunc(30, Timer, x+1);
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


	glutDisplayFunc(display);
	glutKeyboardFunc(PressKey);
	glutTimerFunc(30, Timer, 0);

	init();
	glutMainLoop();

	system("Pause");
	return 0;
}
