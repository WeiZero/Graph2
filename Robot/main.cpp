#include "main.h"



void inittexture() {

	TexArray = TextureApp::GenTextureArray("./Texture/1x.png", 1, 7);

	Tex[0] = TextureApp::GenTexture("./Texture/0.png");
	Tex[1] = TextureApp::GenTexture("./Texture/1.png");
	Tex[2] = TextureApp::GenTexture("./Texture/2.png");
	Tex[3] = TextureApp::GenTexture("./Texture/3.png");
	Tex[4] = TextureApp::GenTexture("./Texture/4.png");
	Tex[5] = TextureApp::GenTexture("./Texture/5.png");
	Tex[6] = TextureApp::GenTexture("./Texture/6.png");
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

	Model = translate(move_x, 0.0, 0.0);
 }

void checkplayer() {
	for (int i = 0; i < 4; i++) {
		playerPos[i] = vec4(player[i].x, player[i].y, 0, 1);
		playerPos[i] = Model * playerPos[i];
		cout << i << " : " << playerPos[0].x << " " << playerPos[0].y << endl;
	}
}

void Drawplayer() {

	glUseProgram(program);

	_Proj = glGetUniformLocation(program, "proj");
	_View = glGetUniformLocation(program, "view");
	_Model = glGetUniformLocation(program, "model");

	glUniformMatrix4fv(_Proj, 1, GL_FALSE, &Projection[0][0]);
	glUniformMatrix4fv(_View, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(_Model, 1, GL_FALSE, &Model[0][0]);

	
	if (state == 0)
		glBindTexture(GL_TEXTURE_2D, Tex[0]);
	else
		glBindTexture(GL_TEXTURE_2D, Tex[texnum]);
	cout << Tex[texnum] << endl;
	float tri_pos[] = {
		//position					//UV
		player[0].x, player[0].y,  player_UV[0].x, player_UV[0].y,
		player[1].x, player[1].y,  player_UV[1].x, player_UV[1].y,
		player[2].x, player[2].y,  player_UV[2].x, player_UV[2].y,

		player[0].x, player[0].y,  player_UV[0].x, player_UV[0].y,
		player[2].x, player[2].y,  player_UV[2].x, player_UV[2].y,
		player[3].x, player[3].y,  player_UV[3].x, player_UV[3].y,
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

	glUniform1i(glGetUniformLocation(program, "TextureM"), 0);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3 * 2);
	glBindVertexArray(0);

}

void DrawplayerArray() {

	glUseProgram(programArray);

	_Proj = glGetUniformLocation(programArray, "proj");
	_View = glGetUniformLocation(programArray, "view");
	_Model = glGetUniformLocation(programArray, "model");

	glUniformMatrix4fv(_Proj, 1, GL_FALSE, &Projection[0][0]);
	glUniformMatrix4fv(_View, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(_Model, 1, GL_FALSE, &Model[0][0]);

	float tri_pos[] = {
		//position					//UV
		player[0].x, player[0].y,  player_UV[0].x, player_UV[0].y,
		player[1].x, player[1].y,  player_UV[1].x, player_UV[1].y,
		player[2].x, player[2].y,  player_UV[2].x, player_UV[2].y,

		player[0].x, player[0].y,  player_UV[0].x, player_UV[0].y,
		player[2].x, player[2].y,  player_UV[2].x, player_UV[2].y,
		player[3].x, player[3].y,  player_UV[3].x, player_UV[3].y,
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
	
	if (texnum > 1)texnum = 1;
	glUniform1i(glGetUniformLocation(programArray, "SpriteIndex"),texnum);
	cout << TexArray << endl;
	glActiveTexture(GL_TEXTURE0 + TexArray);
	//glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	glUniform1i(glGetUniformLocation(programArray, "TexArray"), TexArray);
	//glUniform1i(index, 0);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3 * 2);
	glBindVertexArray(0);
}

void display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/////////////////////////設定相機/////////////////////////
	CameraPos = vec3(0.0f, 0.0f, 10.0f);
	Projection = ortho(-300.0f, 300.0f, -300.0f, 300.0f, 0.1f, 100.0f);
	View = lookAt(CameraPos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

	updateModels();	
	DrawplayerArray();

	glutSwapBuffers();
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
		{ GL_FRAGMENT_SHADER, "Shader/ArrayTexture2.fs" },//fragment shader
		{ GL_NONE, NULL } };

	program = LoadShaders(Sshaders);//讀取shader
	programArray = LoadShaders(ArrayTex);

	player[0] = Point(-300, -300);
	player[1] = Point(-300 + player_WIDTH, -300);

	player[2] = Point(-300 + player_WIDTH, -300 + player_HEIGHT);
	player[3] = Point(-300 , -300 + player_HEIGHT);

	player_UV[0] = Point(0, 0);
	player_UV[1] = Point(1, 0);
	player_UV[2] = Point(1, 1);
	player_UV[3] = Point(0, 1);
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

 	if (moveRight && moveNow) 
		move_x += player_Speed;
 	else if(moveNow)
		move_x -= player_Speed;
 
 	switch (state)
 	{
 		case 0:
 			texnum = 0;
 			break;
 		case 1:
 			texnum = (texnum + 1 >= 6) ? 1 : texnum + 1;
 			break;
 	}

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
	glutCreateWindow("Project2");

	glewExperimental = GL_TRUE; //置於glewInit()之前
	if (glewInit()) {
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;//c error
		exit(EXIT_FAILURE);
	}

	init();
	inittexture();

	glutDisplayFunc(display);
	glutKeyboardFunc(PressKey);
	glutTimerFunc(50, Timer, 0);

	glutMainLoop();
	return 0;
}