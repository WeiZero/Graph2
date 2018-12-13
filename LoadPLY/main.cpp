#include "main.h"

void inittexture() {
	//_Tex = stbiloader::Gentexture("Texture/1.jpg");
	//_TexArray = stbiloader::GenArray_tex("Texture/run/run.png", 1, 18);

	_JumpTex = stbiloader::GenArray_tex("Texture/jump/jump.png", 1, 2);
	_RunArray = stbiloader::GenArray_tex("Texture/run/run.png", 1, 18);
	_IdleArray = stbiloader::GenArray_tex("Texture/idle/idle.png", 1, 12);
}

void initPlayer() {
	Player[0].x = 0.0 - 300;
	Player[0].y = 0.0 - 300;
	Player[1].x = Player[0].x + PLAYER_WIDTH;
	Player[1].y = Player[0].y;
	Player[2].x = Player[0].x + PLAYER_WIDTH;
	Player[2].y = Player[0].y + PLAYER_HEIGHT;
	Player[3].x = Player[0].x;
	Player[3].y = Player[0].y + PLAYER_HEIGHT;

	Playeruv[0].x = 0.0; Playeruv[0].y = 0.0;
	Playeruv[1].x = 1.0; Playeruv[1].y = 0.0;
	Playeruv[2].x = 1.0; Playeruv[2].y = 1.0;
	Playeruv[3].x = 0.0; Playeruv[3].y = 1.0;

	tempR[0].x = Playeruv[0].x; tempR[0].y = Playeruv[0].y;
	tempR[1].x = Playeruv[1].x; tempR[1].y = Playeruv[1].y;
	tempR[2].x = Playeruv[2].x; tempR[2].y = Playeruv[2].y;
	tempR[3].x = Playeruv[3].x; tempR[3].y = Playeruv[3].y;

	tempL[0].x = Playeruv[1].x; tempL[0].y = Playeruv[1].y;
	tempL[1].x = Playeruv[0].x; tempL[1].y = Playeruv[0].y;
	tempL[2].x = Playeruv[3].x; tempL[2].y = Playeruv[3].y;
	tempL[3].x = Playeruv[2].x; tempL[3].y = Playeruv[2].y;
}

void init() {
	glClearColor(0.0, 0.0, 0.0, 1);//black screen
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_LIGHTING);
	//glDepthFunc(GL_LESS);
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);

	ShaderInfo ArrayTexShader[] = {
		{ GL_VERTEX_SHADER, "Shader/ArrayTexture.vs" },//vertex shader
		{ GL_FRAGMENT_SHADER, "Shader/ArrayTexture.fs" },//fragment shader
		{ GL_NONE, NULL } };

	_ArrayProgram = LoadShaders(ArrayTexShader);

	initPlayer();
	inittexture();
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

	Model = translate(move_x, jump_y, 0.0);
}

void checkPlayer() {
	for (int i = 0; i < 4; i++) {
		PlayerPos[i] = vec4(Player[i].x, Player[i].y, 0, 1);
		PlayerPos[i] = Model * PlayerPos[i];
		//cout << i << " : " << PlayerPos[0].x << " " << PlayerPos[0].y << endl;
	}
}

void checkground() {

}

void DrawPlayer()
{
	glUseProgram(_ArrayProgram);
	_Comera = glGetUniformLocation(_ArrayProgram, "cameraPos");
	_Proj = glGetUniformLocation(_ArrayProgram, "proj");
	_View = glGetUniformLocation(_ArrayProgram, "view");
	_Model = glGetUniformLocation(_ArrayProgram, "model");

	glUniform3fv(_Comera, 1, &CameraPos[0]);
	glUniformMatrix4fv(_Proj, 1, GL_FALSE, &Projection[0][0]);
	glUniformMatrix4fv(_View, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(_Model, 1, GL_FALSE, &Model[0][0]);

	glActiveTexture(GL_TEXTURE0);
	if (state == 0) {
		glBindTexture(GL_TEXTURE_2D_ARRAY, _IdleArray);
		glUniform1i(glGetUniformLocation(_ArrayProgram, "SpriteIndex"), SpriteIndex);
	}
	else if (state == 1) {
		glBindTexture(GL_TEXTURE_2D_ARRAY, _RunArray);
		glUniform1i(glGetUniformLocation(_ArrayProgram, "SpriteIndex"), SpriteIndex);
	}
	else if (state == 2) {
		glBindTexture(GL_TEXTURE_2D_ARRAY, _RunArray);
		glUniform1i(glGetUniformLocation(_ArrayProgram, "SpriteIndex"), SpriteIndex);
	}
	else if (state == 3 && is_ground == 0) {
		glBindTexture(GL_TEXTURE_2D_ARRAY, _JumpTex);
		glUniform1i(glGetUniformLocation(_ArrayProgram, "SpriteIndex"), 1);
	}
	else if (state == 3 && is_ground == 1) {
		glBindTexture(GL_TEXTURE_2D_ARRAY, _JumpTex);
		glUniform1i(glGetUniformLocation(_ArrayProgram, "SpriteIndex"), 0);
	}

	glUniform1i(glGetUniformLocation(_ArrayProgram, "Array_tex"), 0);

	float tri_pos[] = {
		//position					//UV
		Player[0].x, Player[0].y,  Playeruv[0].x, Playeruv[0].y,
		Player[1].x, Player[1].y,  Playeruv[1].x, Playeruv[1].y,
		Player[2].x, Player[2].y,  Playeruv[2].x, Playeruv[2].y,

		Player[0].x, Player[0].y,  Playeruv[0].x, Playeruv[0].y,
		Player[2].x, Player[2].y,  Playeruv[2].x, Playeruv[2].y,
		Player[3].x, Player[3].y,  Playeruv[3].x, Playeruv[3].y,
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

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3 * 2);
	glBindVertexArray(0);
	glUseProgram(0);
}


void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/////////////////////////設定相機/////////////////////////
	CameraPos = vec3(0.0f, 0.0f, 10.0f);
	Projection = ortho(-300.0f, 300.0f, -300.0f, 300.0f, 0.1f, 100.0f);
	View = lookAt(CameraPos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

	updateModels();
 	checkPlayer();
 	checkground();
	DrawPlayer();



	glFlush();//強制執行上次的OpenGL commands
	glutSwapBuffers();//調換前台和後台buffer ,當後臺buffer畫完和前台buffer交換使我們看見它
}


void PressKey(unsigned char key, int x, int y) {

	switch (key)
	{
	case 'd':
		Playeruv[0] = tempR[0];
		Playeruv[1] = tempR[1];
		Playeruv[2] = tempR[2];
		Playeruv[3] = tempR[3];
		is_Move = 1;
		if (state != 3)	state = 1;
		break;
	case 'a':
		Playeruv[0] = tempL[0];
		Playeruv[1] = tempL[1];
		Playeruv[2] = tempL[2];
		Playeruv[3] = tempL[3];
		is_Move = 2;
		if (state != 3)	state = 2;
		break;
	case 'w':
		if (is_ground == 0 && state != 3) {
			tempstate = state;
			state = 3;
		}
		break;
	}
	glutPostRedisplay();
}

void PressUp(unsigned char key, int x, int y) {

	switch (key)
	{
	case 'd':
		SpriteIndex = 0;
		is_Move = 0;
		if (state != 3 || jump_y == 0)	state = 0;
		break;
	case 'a':
		SpriteIndex = 0;
		is_Move = 0;
		if (state != 3 || jump_y == 0)	state = 0;
		break;
	}
	glutPostRedisplay();
}

void Timer(int x) {

	if (is_Move == 1) move_x += Player_Speed;
	else if (is_Move == 2) move_x -= Player_Speed;

	switch (state)
	{
	case 0:
		SpriteIndex = (SpriteIndex + 1 == 12) ? 0 : SpriteIndex + 1;
		break;
	case 1:
		SpriteIndex = (SpriteIndex + 1 == 18) ? 0 : SpriteIndex + 1;
		break;
	case 2:
		SpriteIndex = (SpriteIndex + 1 == 18) ? 0 : SpriteIndex + 1;
		break;
	case 3:
		if (is_ground == 0) {
			if (jump_y < 50) {
				jump_y += Player_Speed;
			}
			else if (jump_y == 50) {
				is_ground = 1;
			}
		}
		else if (is_ground == 1) {
			if (jump_y > 0) {
				jump_y -= Player_Speed;
			}
			else if (jump_y == 0) {
				if (is_Move != 0)state = tempstate;
				else if (is_Move == 0)state = 0;
				is_ground = 0;
			}
		}
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
	glutCreateWindow("Hello World");

	glewExperimental = GL_TRUE; //置於glewInit()之前
	if (glewInit()) {
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;//c error
		exit(EXIT_FAILURE);
	}


	glutDisplayFunc(display);
	glutKeyboardFunc(PressKey);
	glutKeyboardUpFunc(PressUp);
	glutTimerFunc(50, Timer, 0);

	init();


	glutMainLoop();

	system("Pause");
	return 0;
}
