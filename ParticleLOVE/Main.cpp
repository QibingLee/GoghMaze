
#include "particle.h"

GLTexture texture;
ParticleSystem Love;

void ReSizeGLScene(GLsizei width, GLsizei height)				
{																			
	if ( height==0) {																							
		height=1;															
	}																																				
	glViewport(0,0,width,height);										
	glMatrixMode(GL_PROJECTION);								
	glLoadIdentity();														
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);		
	glMatrixMode(GL_MODELVIEW);											
	glLoadIdentity();													
}


int InitGL(GLvoid)												
{
	glShadeModel(GL_SMOOTH);												
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);									
	glClearDepth(1.0f);														      
	glEnable(GL_DEPTH_TEST);											
	glDepthFunc(GL_LEQUAL);													

	texture.LoadBMP("Data/Particle4.bmp");
	texture.Use();
	Love.InitParticles();
	return TRUE;															
}


void RenderGLScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
	glLoadIdentity();
	glPushMatrix();														
	glLoadIdentity();												
	glTranslatef(0.0f, 0.0f, -6.0f);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);									
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);					
	Love.Renovate();   //粒子状态更新

	glPopMatrix();													
	glFlush();
	glutSwapBuffers();

}

void TimerFunc(int value)
{
	glutTimerFunc(value, TimerFunc, 1);
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(1000, 700);
	glutCreateWindow("爱心粒子系统");
	InitGL();
	glutReshapeFunc(ReSizeGLScene);
	glutDisplayFunc(RenderGLScene);
	//glutIdleFunc(RenderGLScene);
	glutTimerFunc(10, TimerFunc, 1);
	glutMainLoop();
	return 0;
}
