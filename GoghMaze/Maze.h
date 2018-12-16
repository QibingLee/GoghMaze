#ifndef _Maze_H
#define _Maze_H

#include "Texture.h"
#include <Angel.h>
using namespace std;

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

#define MAZE_HEIGHT (16)
#define MAZE_WIDTH  (16)

class CMaze{

public:
	CMaze();
	void MazeInit();
	bool Wall(int x, int y);
	void Maze3D();
	void Maze2D();

private:
	bool OnOpen(int x, int y);
	void CloseIt(int x, int y);
	bool Neighbor(int x, int y, int w, int *nx, int *ny);
	bool Diagnal(int x, int y, int w, int *nx, int *ny);
	void Dw(int x, int y, int p);
	void Drawtop();
	void Forward(float px, float py, float bf);
	void DrawWalls(void);
	void DrawSkyGround(void);
	void DrawTop(void);
	void DrawBall(void);
};

#endif 



