#include "main.h"

GLuint program;
GLuint vao, vbo, Avao, Avbo;

glm::mat4 Model;
GLuint _Model;
float move_x = 0.0, jump_y = 0.0;
bool is_ground = 0;
int is_Move = 0;
int state = 0;

GLuint IdleTex[12],  RunTex[18];
int texnum = 0;

mat4 Projection;
mat4 View;
vec3 CameraPos;

GLuint _Camera, _Proj, _View;

Point Player[4], Playeruv[4];
glm::vec4 PlayerPos[4];
int tempstate;

Point tempR[4], tempL[4];
Point nowground;

GLuint Array_tex;

void inittexture() {

	Array_tex = TextureApp::GenTextureArray("./Texture/Idle/1x.png", 12, 1);

	IdleTex[0] = TextureApp::GenTexture("./Texture/Idle/Idle1.png");
	IdleTex[1] = TextureApp::GenTexture("./Texture/Idle/Idle2.png");
	IdleTex[2] = TextureApp::GenTexture("./Texture/Idle/Idle3.png");
	IdleTex[3] = TextureApp::GenTexture("./Texture/Idle/Idle4.png");
	IdleTex[4] = TextureApp::GenTexture("./Texture/Idle/Idle5.png");
	IdleTex[5] = TextureApp::GenTexture("./Texture/Idle/Idle6.png");
	IdleTex[6] = TextureApp::GenTexture("./Texture/Idle/Idle7.png");
	IdleTex[7] = TextureApp::GenTexture("./Texture/Idle/Idle8.png");
	IdleTex[8] = TextureApp::GenTexture("./Texture/Idle/Idle9.png");
	IdleTex[9] = TextureApp::GenTexture("./Texture/Idle/Idle10.png");
	IdleTex[10] = TextureApp::GenTexture("./Texture/Idle/Idle11.png");
	IdleTex[11] = TextureApp::GenTexture("./Texture/Idle/Idle12.png");

	RunTex[0] = TextureApp::GenTexture("./Texture/run/run1.png");
	RunTex[1] = TextureApp::GenTexture("./Texture/run/run2.png");
	RunTex[2] = TextureApp::GenTexture("./Texture/run/run3.png");
	RunTex[3] = TextureApp::GenTexture("./Texture/run/run4.png");
	RunTex[4] = TextureApp::GenTexture("./Texture/run/run5.png");
	RunTex[5] = TextureApp::GenTexture("./Texture/run/run6.png");
	RunTex[6] = TextureApp::GenTexture("./Texture/run/run7.png");
	RunTex[7] = TextureApp::GenTexture("./Texture/run/run8.png");
	RunTex[8] = TextureApp::GenTexture("./Texture/run/run9.png");
	RunTex[9] = TextureApp::GenTexture("./Texture/run/run10.png");
	RunTex[10] = TextureApp::GenTexture("./Texture/run/run11.png");
	RunTex[11] = TextureApp::GenTexture("./Texture/run/run12.png");
	RunTex[12] = TextureApp::GenTexture("./Texture/run/run13.png");
	RunTex[13] = TextureApp::GenTexture("./Texture/run/run14.png");
	RunTex[14] = TextureApp::GenTexture("./Texture/run/run15.png");
	RunTex[15] = TextureApp::GenTexture("./Texture/run/run16.png");
	RunTex[16] = TextureApp::GenTexture("./Texture/run/run17.png");
	RunTex[17] = TextureApp::GenTexture("./Texture/run/run18.png");
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
		cout << i << " : " << PlayerPos[0].x << " " << PlayerPos[0].y << endl;
	}
}

void checkground() {
	
}


void DrawPlayer() {


	glUseProgram(program);
	_Camera = glGetUniformLocation(program, "cameraPos");
	_Proj = glGetUniformLocation(program, "proj");
	_View = glGetUniformLocation(program, "view");
	_Model = glGetUniformLocation(program, "model");

	glUniform3fv(_Camera, 1, &CameraPos[0]);
	glUniformMatrix4fv(_Proj, 1, GL_FALSE, &Projection[0][0]);
	glUniformMatrix4fv(_View, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(_Model, 1, GL_FALSE, &Model[0][0]);

	glActiveTexture(GL_TEXTURE1);
	if (state == 0)
		glBindTexture(GL_TEXTURE_2D, IdleTex[texnum]);
	else if (state == 1)
		glBindTexture(GL_TEXTURE_2D, RunTex[texnum]);
	else if (state == 2)
		glBindTexture(GL_TEXTURE_2D, RunTex[texnum]);

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

	glUniform1i(glGetUniformLocation(program, "TextureM"), 1);
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

	//cout << is_ground << endl;

	glutSwapBuffers();//調換前台和後台buffer ,當後臺buffer畫完和前台buffer交換使我們看見它
}

void init() {
	glClearColor(0.0, 0.0, 0.0, 1);//black screen
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ShaderInfo Sshaders[] = {
		{ GL_VERTEX_SHADER, "Shader/trirobot.vs" },//vertex shader
		{ GL_FRAGMENT_SHADER, "Shader/trirobot.fs" },//fragment shader
		{ GL_NONE, NULL } };

	ShaderInfo ArrayTex[] = {
	{ GL_VERTEX_SHADER, "Shader/ArrayTexture.vs" },//vertex shader
	{ GL_FRAGMENT_SHADER, "Shader/ArrayTexture.fs" },//fragment shader
	{ GL_NONE, NULL } };

	program = LoadShaders(Sshaders);//讀取shader


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

void PressKey(unsigned char key, int x, int y) {

	switch (key)
	{
	case 'd':
		Playeruv[0] = tempR[0];
		Playeruv[1] = tempR[1];
		Playeruv[2] = tempR[2];
		Playeruv[3] = tempR[3];
		is_Move = 1;
		state = 1;
		break;
	case 'a':
		Playeruv[0] = tempL[0];
		Playeruv[1] = tempL[1];
		Playeruv[2] = tempL[2];
		Playeruv[3] = tempL[3];
		is_Move = 2;
		state = 2;
		break;
	}
	glutPostRedisplay();
}

void PressUp(unsigned char key, int x, int y) {
	
	switch (key)
	{
	case 'd':
		texnum = 0;
		is_Move = 0;
		state = 0;
		break;
	case 'a':
		texnum = 0;
		is_Move = 0;
		state = 0;
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
 		texnum = (texnum + 1 == 12) ? 0 : texnum + 1;
 		break;
 	case 1:
 		texnum = (texnum + 1 == 18) ? 0 : texnum + 1;
 		break;
 	case 2:
 		texnum = (texnum + 1 == 18) ? 0 : texnum + 1;
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
 	//cout << is_Move << endl;

// 	currentTime = glutGet(GLUT_ELAPSED_TIME);
// 	spritePlayTime += (currentTime - lastTime) / 1000.0f;
// 	spriteIndex = (int)(spritePlayTime * 1 + 0) % 12;
// 	lastTime = currentTime;

	glutTimerFunc(50, Timer, 0);
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitContextVersion(4, 3);//以OpenGL version4.3版本為基準
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);//是否向下相容,GLUT_FORWARD_COMPATIBLE不支援(?
	glutInitContextProfile(GLUT_CORE_PROFILE);

	//multisample for golygons smooth
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("OpenGL 4.3 - Robot");

	glewExperimental = GL_TRUE; //置於glewInit()之前
	if (glewInit()) {
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;//c error
		exit(EXIT_FAILURE);
	}

	init();
	inittexture();

	glutDisplayFunc(display);
	glutKeyboardFunc(PressKey);
	glutKeyboardUpFunc(PressUp);
	glutTimerFunc(50, Timer, 0);

	glutMainLoop();
	return 0;
}
