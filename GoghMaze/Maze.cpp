#include "Maze.h"

GLchar mazedata[MAZE_HEIGHT][MAZE_WIDTH] = {
	{ 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H' },
	{ 'H', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'H', ' ', ' ', ' ', ' ', ' ', ' ', 'H' },
	{ 'H', ' ', 'H', ' ', 'H', 'H', 'H', ' ', 'H', ' ', 'H', ' ', ' ', ' ', ' ', 'H' },
	{ 'H', ' ', 'H', 'H', ' ', ' ', 'H', ' ', 'H', 'H', ' ', 'H', ' ', 'H', ' ', 'H' },
	{ 'H', ' ', ' ', ' ', ' ', ' ', 'H', ' ', ' ', ' ', ' ', ' ', ' ', 'H', ' ', 'H' },
	{ 'H', ' ', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', ' ', 'H', 'H', 'H', ' ', 'H' },
	{ 'H', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'H', ' ', ' ', 'H' },
	{ 'H', ' ', 'H', 'H', 'H', 'H', 'H', ' ', 'H', 'H', 'H', ' ', 'H', 'H', 'H', 'H' },
	{ 'H', ' ', 'H', ' ', ' ', ' ', 'H', ' ', ' ', ' ', 'H', ' ', ' ', ' ', ' ', 'H' },
	{ 'H', ' ', ' ', ' ', 'H', 'H', 'H', 'H', 'H', 'H', 'H', ' ', ' ', ' ', ' ', 'H' },
	{ 'H', ' ', 'H', ' ', ' ', ' ', 'H', ' ', ' ', ' ', 'H', ' ', ' ', 'H', ' ', 'H' },
	{ 'H', ' ', 'H', 'H', 'H', 'H', 'H', ' ', 'H', 'H', 'H', 'H', ' ', 'H', ' ', 'H' },
	{ 'H', ' ', ' ', ' ', ' ', ' ', 'H', ' ', ' ', ' ', ' ', ' ', ' ', 'H', ' ', 'H' },
	{ 'H', ' ', ' ', 'H', 'H', ' ', 'H', 'H', 'H', 'H', ' ', 'H', 'H', 'H', ' ', 'H' },
	{ 'H', ' ', ' ', ' ', 'H', ' ', 'H', ' ', ' ', ' ', ' ', 'H', ' ', ' ', ' ', 'H' },
	{ 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', 'H', ' ', 'H', 'H' },
};

GLuint projMatrixLoc;
GLuint viewMatrixLoc;
GLuint vao[3];

//墙数据
point4  wall_points[500];
vec2 wall_texcoords[500];
point4 wall_normals[500];
GLfloat texcoordX = 0.0f;
GLint  NumVerticesW = 0;

//地板数据
point4  ground_points[500];
vec2 ground_texcoords[500];
point4 ground_normals[500];
GLint  NumVerticesG = 0;

//天空数据
point4  sky_points[500];
vec2 sky_texcoords[500];
GLint  NumVerticesS = 0;

GLTexture texture[10];

CMaze::CMaze(){};

bool CMaze::Wall(int x, int y) {
	return (x >= 0 && y >= 0 && x<MAZE_WIDTH && y<MAZE_HEIGHT && mazedata[y][x] != ' ');
}

bool CMaze::OnOpen(int x, int y){
	if (Wall(x, y)){
		return(mazedata[y][x] == 'H');
	}
}

void CMaze::CloseIt(int x, int y) {
	if (OnOpen(x, y))
	{
		mazedata[y][x] = 'X';
	}
}

bool CMaze::Neighbor(int x, int y, int w, int *nx, int *ny){
	switch (w) {
	case 0:
		*nx = x - 1; *ny = y;   break;
	case 1:
		*nx = x;   *ny = y + 1; break;
	case 2:
		*nx = x + 1; *ny = y;   break;
	case 3:
		*nx = x;   *ny = y - 1; break;
	default:
		break;
	}
	return Wall(*nx, *ny);
}

bool CMaze::Diagnal(int x, int y, int w, int *nx, int *ny){
	switch (w) {
	case 0:
		*nx = x - 1; *ny = y - 1; break;
	case 1:
		*nx = x - 1; *ny = y + 1; break;
	case 2:
		*nx = x + 1; *ny = y + 1; break;
	case 3:
		*nx = x + 1; *ny = y - 1; break;
	default:
		break;
	}
	return Wall(*nx, *ny);
}

//遍历迷宫数组
void CMaze::Dw(int x, int y, int p) {
	int w = p;
	CloseIt(x, y);
	do{
		int x2 = 0;
		int y2 = 0;
		if (Neighbor(x, y, w, &x2, &y2)){
			if (OnOpen(x2, y2)) {
				Dw(x2, y2, (w + 3) % 4);
			}
			else {
				if ((w + 1) % 4 == p)
				{
					return;
				}
			}
		}
		else {
			float fx;
			float fy;
			if (Diagnal(x, y, w, &x2, &y2) && OnOpen(x2, y2)) {
				Dw(x2, y2, (w + 2) % 4);
			}
			texcoordX = (texcoordX<0.5) ? 1.0f : 0.0f;
			fx = (float)x + ((w == 1 || w == 2) ? 1.0f : 0.0f);
			fy = (float)y + ((w == 0 || w == 1) ? 1.0f : 0.0f);
			wall_texcoords[NumVerticesW] = vec2(texcoordX, 0.0f);
			wall_points[NumVerticesW++] = vec4(fx, fy, 0.0f, 1.0f);
			wall_texcoords[NumVerticesW] = vec2(texcoordX, 1.0f);
			wall_points[NumVerticesW++] = vec4(fx, fy, 1.0f, 1.0f);
		}
		w++; w %= 4;
	} while (w != p);
	return;
}

void CMaze::DrawWalls() {
	//画墙壁
	wall_texcoords[NumVerticesW] = vec2(0.0f, 0.0f);
	wall_points[NumVerticesW++] = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	wall_texcoords[NumVerticesW] = vec2(0.0f, 1.0f);
	wall_points[NumVerticesW++] = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	Dw(0, 0, 0);

	//设置顶点法向量
	vec3 Temp_Normals[500];
	GLint NumFaces = 0;
	for (int i = 0; i < NumVerticesW; i++){
		if ((i + 2) != NumVerticesW)
			Temp_Normals[NumFaces++] = cross(wall_points[i + 1] - wall_points[i],
			wall_points[i + 2] - wall_points[i + 1]);
	}
	for (int i = 0; i < NumFaces; i++){
		wall_normals[3 * i] = Temp_Normals[i];
		wall_normals[3 * i + 1] = Temp_Normals[i];
		wall_normals[3 * i + 2] = Temp_Normals[i];
	}
}

void CMaze::DrawSkyGround() {
	//画地板和天空
	for (int y = 0; y < MAZE_HEIGHT; y++) {
		for (int x = 0; x < MAZE_WIDTH; x++) {
			if (!Wall(x, y)) {
				//地板数据	
				ground_texcoords[NumVerticesG] = vec2(0.0f, 0.0f);
				ground_normals[NumVerticesG] = vec3(0.0f, 0.0f, 1.0f);
				ground_points[NumVerticesG++] = vec4(x + 1.0f, y + 0.0f, 0.0f, 1.0f);

				ground_texcoords[NumVerticesG] = vec2(0.0f, 1.0f);
				ground_normals[NumVerticesG] = vec3(0.0f, 0.0f, 1.0f);
				ground_points[NumVerticesG++] = vec4(x + 1.0f, y + 1.0f, 0.0f, 1.0f);

				ground_texcoords[NumVerticesG] = vec2(1.0f, 0.0f);
				ground_normals[NumVerticesG] = vec3(0.0f, 0.0f, 1.0f);
				ground_points[NumVerticesG++] = vec4(x + 0.0f, y + 0.0f, 0.0f, 1.0f);

				ground_texcoords[NumVerticesG] = vec2(1.0f, 1.0f);
				ground_normals[NumVerticesG] = vec3(0.0f, 0.0f, 1.0f);
				ground_points[NumVerticesG++] = vec4(x + 0.0f, y + 1.0f, 0.0f, 1.0f);
				//天空数据
				/**
				sky_texcoords[NumVerticesS] = vec2(0.0f, 0.0f);
				sky_points[NumVerticesS++] = vec4(x + 1.0f, y + 0.0f, 1.0f, 1.0f);
				sky_texcoords[NumVerticesS] = vec2(0.0f, 1.0f);
				sky_points[NumVerticesS++] = vec4(x + 1.0f, y + 1.0f, 1.0f, 1.0f);
				sky_texcoords[NumVerticesS] = vec2(1.0f, 0.0f);
				sky_points[NumVerticesS++] = vec4(x + 0.0f, y + 0.0f, 1.0f, 1.0f);
				sky_texcoords[NumVerticesS] = vec2(1.0f, 1.0f);
				sky_points[NumVerticesS++] = vec4(x + 0.0f, y + 1.0f, 1.0f, 1.0f);
				*/
			}
		}
	}
}

void CMaze::DrawTop() {

	//画顶部
	glBegin(GL_QUADS);
	for (int y = 0; y<MAZE_HEIGHT; y++) {
		for (int x = 0; x<MAZE_WIDTH; x++) {
			if (Wall(x, y)) {
				mazedata[y][x] = 'X';
				glBegin(GL_QUADS);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(x + 0.0f, y + 0.0f, 0.0f);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(x + 1.0f, y + 0.0f, 0.0f);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(x + 1.0f, y + 1.0f, 0.0f);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(x + 0.0f, y + 1.0f, 0.0f);
				glEnd();
			}
		}
	}
	glEnd();
}

void CMaze::DrawBall(){

	//画圆球
	glDisable(GL_TEXTURE_2D);
	glColor4f(1.0, 0.0, 0.0, 0.0);
	glutSolidSphere(0.2f, 15, 15);
}

void CMaze::MazeInit(){

	//顶点、纹理坐标初始化
	DrawWalls();
	DrawSkyGround();
	DrawBall();

	//载入纹理
	texture[0].LoadBMP("Data/20.bmp");
	texture[1].LoadBMP("Data/12.bmp");
	texture[2].LoadBMP("Data/37.bmp");
	texture[3].LoadBMP("Data/1.bmp");
	texture[4].LoadBMP("Data/2.bmp");
	texture[5].LoadBMP("Data/3.bmp");
	texture[6].LoadBMP("Data/4.bmp");
	texture[7].LoadBMP("Data/5.bmp");
	texture[8].LoadBMP("Data/6.bmp");
	texture[9].LoadBMP("Data/7.bmp");
	glActiveTexture(GL_TEXTURE0);

	//顶点缓冲区
	GLuint buffers[3];
	glGenVertexArrays(3, vao);

	//链接程序
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
	GLuint vNormal = glGetAttribLocation(program, "vNormal");

	//绑定墙壁数据
	glBindVertexArray(vao[0]);
	glGenBuffers(3, buffers);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(wall_points), wall_points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, 0, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(wall_texcoords), wall_texcoords, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, 0, 0, 0);
	glUniform1i(glGetUniformLocation(program, "texture"), 0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(wall_normals), wall_normals, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, 0, 0, 0);

	//绑定地板数据
	glBindVertexArray(vao[1]);
	glGenBuffers(3, buffers);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground_points), ground_points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, 0, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground_texcoords), ground_texcoords, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, 0, 0, 0);
	glUniform1i(glGetUniformLocation(program, "texture"), 0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground_normals), ground_normals, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, 0, 0, 0);

	//绑定天空数据
	glBindVertexArray(vao[2]);
	glGenBuffers(3, buffers);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sky_points), sky_points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, 0, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sky_texcoords), sky_texcoords, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, 0, 0, 0);
	glUniform1i(glGetUniformLocation(program, "texture"), 0);


	glEnable(GL_DEPTH_TEST);
	glClearColor(0.5, 0.5, 0.5, 1);

}

void CMaze::Maze3D(){


	glBindVertexArray(vao[0]);
	GLint PerNum = NumVerticesW / 10 + 3;
	for (int i = 0; i < 10; i++){
		texture[i].Use();
		glDrawArrays(GL_TRIANGLE_STRIP, i*PerNum, (i + 1)*PerNum);
	}
	texture[0].Use();
	glBindVertexArray(vao[1]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, NumVerticesG);

	//texture[1].Use();
	//glBindVertexArray(vao[2]);
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, NumVerticesS);
}

void CMaze::Maze2D(){

	//DrawBall();

	glBindVertexArray(vao[0]);
	GLint PerNum = NumVerticesW / 10 + 3;
	for (int i = 0; i < 10; i++){
		texture[i].Use();
		glDrawArrays(GL_TRIANGLE_STRIP, i*PerNum, (i + 1)*PerNum);
	}

}
