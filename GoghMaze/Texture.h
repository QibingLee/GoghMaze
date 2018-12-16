
// tex.Load("texture.bmp"); // Loads a bitmap
// tex.Use();				// Binds the bitmap for use
// tex3.BuildColorTexture(255, 0, 0);	// Builds a solid red texture
// tex3.Use();				 // Binds the targa for use
//
//////////////////////////////////////////////////////////////////////

#ifndef GLTEXTURE_H
#define GLTEXTURE_H
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glaux.h>

#pragma comment(lib, "opengl.lib" )
#pragma comment(lib, "glaux.lib")	 //��Ӻ���ܳ���SAFESEHӳ���ǲ���ȫ����ʾ
#pragma comment(lib, "glew32.lib")	 //��Ӻ���ܳ���SAFESEHӳ���ǲ���ȫ����ʾ

class GLTexture
{
public:
	unsigned int texture[1];						// OpenGL's number for the texture
	void Use();										// Binds the texture for use
	void BuildColorTexture(unsigned char r, unsigned char g, unsigned char b);	// Sometimes we want a texture of uniform color
	void LoadBMP(char *name);						// Loads a bitmap file
	GLTexture();									// Constructor
	virtual ~GLTexture();							// Destructor

};

#endif GLTEXTURE_H