#include "main.h"

void playerInit() {
	player[0] = Point(-300, -300);
	player[1] = Point(-300 + player_Width, -300);

	player[2] = Point(-300 + player_Width, -300 + player_Height);
	player[3] = Point(-300, -300 + player_Height);

	player_UV[0] = Point(0, 0);
	player_UV[1] = Point(1, 0);
	player_UV[2] = Point(1, 1);
	player_UV[3] = Point(0, 1);
	playerArrayTex = stbiloader::GenArray_tex("Texture/1x.png", 1, 7);
	float tri_pos[] = {
		//position					//UV
		player[0].x, player[0].y,  player_UV[0].x, player_UV[0].y,
		player[1].x, player[1].y,  player_UV[1].x, player_UV[1].y,
		player[2].x, player[2].y,  player_UV[2].x, player_UV[2].y,

		player[0].x, player[0].y,  player_UV[0].x, player_UV[0].y,
		player[2].x, player[2].y,  player_UV[2].x, player_UV[2].y,
		player[3].x, player[3].y,  player_UV[3].x, player_UV[3].y,
	};
	glUseProgram(playerArray);
	glBindTexture(GL_TEXTURE_2D_ARRAY, playerArrayTex);
	glUniform1i(glGetUniformLocation(playerArray, "Array_tex"), 0);
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tri_pos), &tri_pos, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FLOAT, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glDrawArrays(GL_TRIANGLES, 0, 3 * 2);
}
void TexInit() {
	BGTex = stbiloader::Gentexture("Texture/bg.png");
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

	playerArray = LoadShaders(playerArrayShader);
	blockArray = LoadShaders(blockArrayShader);
	BG = LoadShaders(backgroundShader);
	TexInit();
	playerInit();
	BgInit();
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
	glm::mat4 Translation = glm::mat4(1.0f);
	glm::mat4 Scale = glm::mat4(1.0f);
	glm::mat4 Rotate = glm::mat4(1.0f);
	Model = glm::mat4(1.0f);
	Model = translate(move_x, 0., 0.0);
}

void DrawPlayer()
{
	glUseProgram(playerArray);
	_Proj = glGetUniformLocation(playerArray, "proj");
	_View = glGetUniformLocation(playerArray, "view");
	_Model = glGetUniformLocation(playerArray, "model");

	glUniformMatrix4fv(_Proj, 1, GL_FALSE, &Projection[0][0]);
	glUniformMatrix4fv(_View, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(_Model, 1, GL_FALSE, &Model[0][0]);

	if (state == 0) 
		glUniform1i(glGetUniformLocation(playerArray, "SpriteIndex"), 0);
	else if (state == 1) 
		glUniform1i(glGetUniformLocation(playerArray, "SpriteIndex"), SpriteIndex);
	
	float tri_pos[] = {
		//position					//UV
		player[0].x, player[0].y,  player_UV[0].x, player_UV[0].y,
		player[1].x, player[1].y,  player_UV[1].x, player_UV[1].y,
		player[2].x, player[2].y,  player_UV[2].x, player_UV[2].y,

		player[0].x, player[0].y,  player_UV[0].x, player_UV[0].y,
		player[2].x, player[2].y,  player_UV[2].x, player_UV[2].y,
		player[3].x, player[3].y,  player_UV[3].x, player_UV[3].y,
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(tri_pos), &tri_pos, GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, 3 * 2);
}

void Drawblock() {
	for (int i = 0; i < blockList.size(); i++) {
		cout << blockList.size() << endl;
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

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(tri_pos), &tri_pos, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FLOAT, 4 * sizeof(float), (void*)(2 * sizeof(float)));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, blockTex[blockList[i].index]);
		glUniform1i(glGetUniformLocation(blockTex[blockList[i].index], "Tex"), 0);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3 * 2);
	}
}
void DrawBG() {
	glUseProgram(BG);
	glBindTexture(GL_TEXTURE_2D, BGTex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), &quad, GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, 3 * 2);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	updateModels();
	DrawPlayer();
	//Drawblock();
	DrawBG();
	glFlush();//強制執行上次的OpenGL commands
	glutSwapBuffers();//調換前台和後台buffer ,當後臺buffer畫完和前台buffer交換使我們看見它
}


void PressKey(unsigned char key, int x, int y) {

	switch (key)
	{
	case 'd':
		player_UV[0] = Point(0, 0);
		player_UV[1] = Point(1, 0);
		player_UV[2] = Point(1, 1);
		player_UV[3] = Point(0, 1);
		moveRight = true;
		moveNow = true;
		state = 1;
		break;
	case 'a':
		player_UV[0] = Point(1, 0);
		player_UV[1] = Point(0, 0);
		player_UV[2] = Point(0, 1);
		player_UV[3] = Point(1, 1);
		moveRight = false;
		moveNow = true;
		state = 1;
		break;
	case 's':
		moveNow = false;
		state = 0;
		break;
	}
	glutPostRedisplay();
}


void Timer(int x) {

	/*if (x % 100000000 == 0) {
		int s = rand() % 560 - 300;
		if (blockNum >= 6)
			blockNum = 0;
		Block block(blockNum, s);
		blockList.push_back(block);
		blockNum++;
		for (int i = 0; i < blockList.size(); i++) {
			if (blockList[i].downY(20)<=-330){
				blockList.erase(blockList.begin()+i);
				i--;
			}
		}
	}*/

 	switch (state)
 	{
 		case 0:
			SpriteIndex = 0;
 			break;
 		case 1:
			if (moveRight && moveNow)
				move_x += player_Speed;
			else if (moveNow)
				move_x -= player_Speed;
			SpriteIndex = (SpriteIndex + 1 >= 6) ? 1 : SpriteIndex + 1;
 			break;
 	}

	glutTimerFunc(50, Timer, 0);
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
	glutTimerFunc(50, Timer, 0);

	init();
	glutMainLoop();

	system("Pause");
	return 0;
}
