//一步步.存在各种问题：迷宫递归生成 从CPU->GPU  基本图元不支持QUADS 顶点变化 照相机坐标系变化 版本问题 光源问题 3D`2D GLSL输入输出 检测碰撞 显示列表 VAO/VBO 光照法向量
#include "Maze.h"
#include "Camera.h"

#define kAngle 10.0f              //照相机转动的角度
#define upAngle 0.02f             //照相机上扬的角度

//光照结构体
struct gl_LightParameters {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 position;
	vec4 halfVector;
	vec3 spotDirection;
	float spotExponent;
	float spotCutoff;
	float spotCosCutoff;
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
}Light;

//材质结构体
struct gl_MaterialParameters {
	vec4 emission;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float* shininess;
}Material;

CMaze   maze;                     //创建迷宫对象
CCamera camera;                   //创建照相机对象
GLfloat mouth_X;                  //鼠标点击X坐标
GLfloat mouth_Y;                  //鼠标点击Y坐标
GLboolean Is3D = true;            //3D模式或2D平面图模式
GLfloat kSpeed = 0.2f;            //照相机移动的速度

// 光照材质初始化
void InitLights()
{
	Light.position = vec4(8.0, 2.0, 2.0, 1.0);
	Light.diffuse = vec4(1.0, 1.0, 1.0, 1.0);
	Light.ambient = vec4(0.2, 0.2, 0.2, 1.0);
	Light.specular = vec4(0.0, 0.0, 1.0, 1.0);
	Light.spotDirection = vec3(-1.0f, -1.0f, 0.0f);
	Light.spotExponent = 8.0f;
	Light.spotCutoff = 60.0f;
	Light.constantAttenuation = 0.001f;
	Light.linearAttenuation = 0.001f;
	Light.quadraticAttenuation = 0.001f;

	Material.shininess = new float[]{ 100 };
	Material.diffuse = vec4(1.0, 1.0, 1.0, 1.0);
	Material.ambient = vec4(0.0, 1.0, 1.0, 1.0);
	Material.specular = vec4(1.0, 1.0, 0.0, 1.0);

	glLightfv(GL_LIGHT0, GL_SPECULAR, Light.specular);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Light.diffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, Light.ambient);

	glLightfv(GL_LIGHT0, GL_POSITION, Light.specular);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, Light.constantAttenuation);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, Light.linearAttenuation);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, Light.quadraticAttenuation);

	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, Light.specular);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, Light.spotExponent);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, Light.spotCutoff);

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Material.diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Material.ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Material.specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, Material.shininess);
}

//墙壁检测    还有一些错误未改正
void Forward(float bf = 0.2f)
{
	vec3 pos = camera.getPos();
	vec3 pre = camera.getPrePos();
	int x = ((int)pre.x);
	int y = abs(((int)pre.z));
	GLfloat px = pos.x;
	GLfloat py = abs(pos.z);
	bool flag = false;
	if ((px> x + 1.0f - bf) && maze.Wall(x + 1, y))  {
		px = (float)(x)+1.0f - bf;
		flag = true;
	}
	if ((py> y + 1.0f - bf) && maze.Wall(x, y + 1))  {
		py = (float)(y)+1.0f - bf;
		flag = true;
	}
	if ((px< x + bf) && maze.Wall(x - 1, y)) {
		px = (float)(x)+bf;
		flag = true;
	}
	if ((py< y + bf) && maze.Wall(x, y - 1)) {
		py = (float)(y)+bf;
		flag = true;
	}
	flag = false;
	camera.Update(px, py, flag);
}

//渲染回调函数
void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLightfv(GL_LIGHT0, GL_POSITION, Light.position);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, Light.spotDirection);

	if (Is3D == true)
	{
		glLoadIdentity();
		glPushMatrix();
		camera.setCamera();
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		maze.Maze3D();
		glPopMatrix();
	}
	if (Is3D == false)
	{
		glLoadIdentity();
		glOrtho(-16.0, 16.0, -16.0, 16.0, -0.0, 20.0);
		glPushMatrix();
		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
		glTranslatef(-MAZE_WIDTH / 2, -MAZE_HEIGHT / 2, -0.5f);
		maze.Maze2D();
		glPopMatrix();
	}
	glFlush();
	glutSwapBuffers();
}

void Reshape(int w, int h)
{
	float aspect = (float)w / ((h) ? h : 1);
	glViewport(0, 0, w, h);
	aspect = (1.0f * w) / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, aspect, 0.1, 60.0);
	glMatrixMode(GL_MODELVIEW);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

//鼠标点击回调
void onMouseClick(GLint button, GLint state, GLint x, GLint y)
{
	//第一次鼠标按下时,记录鼠标在窗口中的初始坐标
	if (state == GLUT_DOWN)
		mouth_X = x, mouth_Y = y;
}

//鼠标移动回调
void onMouseMove(GLint x, GLint y){
	//判断八个方向
	if (x < mouth_X && fabs(y - mouth_Y) <= 5){
		camera.RotateCamera(-kAngle / 5);
	}
	else if (x > mouth_X && fabs(y - mouth_Y) <= 5) {
		camera.RotateCamera(kAngle / 5);
	}
	else if (fabs(x - mouth_X) <= 5 && y > mouth_Y){
		camera.UpDownCamera(-upAngle);
	}
	else if (fabs(x - mouth_X) <= 5 && y < mouth_Y)  {
		camera.UpDownCamera(upAngle);
	}
	else if (x > mouth_X && y > mouth_Y) {
		camera.RotateCamera(kAngle / 5), camera.UpDownCamera(-upAngle);
	}
	else if (x < mouth_X && y < mouth_Y) {
		camera.RotateCamera(-kAngle / 5), camera.UpDownCamera(upAngle);
	}
	else if (x > mouth_X && y < mouth_Y) {
		camera.RotateCamera(kAngle / 5), camera.UpDownCamera(upAngle);
	}
	else if (x < mouth_X && y > mouth_Y) {
		camera.RotateCamera(-kAngle / 5), camera.UpDownCamera(-upAngle);
	}
	//重设最新点
	mouth_X = x, mouth_Y = y;
	glutPostRedisplay();
}

//方向键回调
void SpecialKeys(int key, int x, int y)
{
	// 是否按下UP箭头键
	if (key == GLUT_KEY_UP) {
		// 移动摄像机
		camera.MoveCamera(kSpeed);
	}
	// 是否按下DOWN键
	if (key == GLUT_KEY_DOWN) {
		// 移动摄像机
		camera.MoveCamera(-kSpeed);
	}
	// 是否按下LEFT键
	if (key == GLUT_KEY_LEFT) {
		// 旋转摄像机
		camera.RotateCamera(-kAngle);
	}
	// 是否按下RIGHT键
	if (key == GLUT_KEY_RIGHT) {
		// 旋转摄像机
		camera.RotateCamera(kAngle);
	}
	if (key == GLUT_KEY_ALT_L) {
		// 3D
		Is3D = true;
	}
	if (key == GLUT_KEY_ALT_R) {
		// 2D
		Is3D = false;
	}
	if (key == GLUT_KEY_PAGE_UP) {
		// 加速
		kSpeed *= 2.0f;
	}
	if (key == GLUT_KEY_PAGE_DOWN) {
		// 减速
		kSpeed /= 2.0f;
	}
	if (key == GLUT_KEY_F1) {
		// 平行光
		Light.position[3] = 0.0f;
	}
	if (key == GLUT_KEY_F2) {
		// 点光源
		Light.position[3] = 1.0f;
		Light.spotDirection = 0.0f;
	}
	if (key == GLUT_KEY_F3) {
		// 聚光灯
		Light.position[3] = 1.0f;
		Light.spotDirection = -1.0f;
	}
	Forward(0.2f);
	glutPostRedisplay();
}

//特殊键回调 光照位置
void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '1':
		Light.position[0] += 3.0f;
		break;
	case '2':
		Light.position[0] -= 3.0f;
		break;
	case '3':
		Light.position[1] += 3.0f;
		break;
	case '4':
		Light.position[1] -= 3.0f;
		break;
	case '5':
		Light.position[2] += 0.5f;
		break;
	case '6':
		Light.position[2] -= 0.5f;
		break;
	default:break;
	}
	glutPostRedisplay();
}

//主函数入口
void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);
	//glutInitContextVersion(4, 3);
	glutCreateWindow("迷宫");
	glewExperimental = GL_TRUE;
	glewInit();
	InitLights();
	maze.MazeInit();
	glutReshapeFunc(Reshape);
	glutDisplayFunc(RenderScene);
	glutMotionFunc(onMouseMove);
	glutMouseFunc(onMouseClick);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKeys);
	glutMainLoop();
}

