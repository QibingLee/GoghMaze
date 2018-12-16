
#include "particle.h"

ParticleSystem::ParticleSystem ()
{

	// TODO: add member initialization code here
	slowdown = 0.5f;
	zoom = -40.0f;
	col = 0;
	xspeed = 0;
	yspeed = 0;
	zspeed = 0;
}

void ParticleSystem::InitParticles()												// All Setup For OpenGL Goes Here
{

	for (loop = 0; loop<MAX_PARTICLES; loop++)				// Initials All The Textures
	{
		particle[loop].active = true;						// Make All The Particles Active 粒子一开始都是活动状态
		particle[loop].life = 1.0f;						// Give All The Particles Full Life 所有粒子寿命取整个生命值
		particle[loop].fade = float(rand() % 100) / 1000.0f + 0.003f;	// Random Fade Speed  随机衰竭速度
		particle[loop].r = colors[(loop + 1) / (MAX_PARTICLES / 12)][0];	// Select Red Rainbow Color 从颜色表中取红色
		particle[loop].g = colors[(loop + 1) / (MAX_PARTICLES / 12)][1];	// Select Green Rainbow Color 从颜色表中取绿色
		particle[loop].b = colors[(loop + 1) / (MAX_PARTICLES / 12)][2];	// Select Blue Rainbow Color 从颜色表中取蓝色
		particle[loop].x = float(rand() % 20 - 10);             //粒子初始位置x值
		particle[loop].y = float(rand() % 20 - 10);             //粒子初始位置y值
		particle[loop].z = float(rand() % 20 - 10);             //粒子初始位置z值
		particle[loop].xi = float((rand() % 50) - 26.0f)*10.0f;	// Random Speed On X Axis  X方向速度取随机的值
		particle[loop].yi = float((rand() % 50) - 25.0f)*10.0f;	// Random Speed On Y Axis  Y方向速度取随机的值
		particle[loop].zi = float((rand() % 50) - 25.0f)*10.0f;	// Random Speed On Z Axis  Z方向速度取随机的值
		particle[loop].xg = 0.0f;							// Set Horizontal Pull To Zero 将水平拉力（加速度）设为0
		particle[loop].yg = 0.0f;						// Set Vertical Pull Downward  设置向下的拉力（加速度为0）
		particle[loop].zg = 18.8f;							// Set Pull On Z Axis To Zero 设置Z向的拉力为0
	}
}


void ParticleSystem::Renovate()
{

	for (loop = 0; loop<MAX_PARTICLES*0.25; loop++)					// Loop Through All The Particles 
	{
		if (particle[loop].active)							// If The Particle Is Active 如果粒子处于活动状态
		{
			float x = particle[loop].x;						// Grab Our Particle X Position
			float y = particle[loop].y;						// Grab Our Particle Y Position
			float z1 = particle[loop].z;
			float z = particle[loop].z + zoom;					// Particle Z Pos + Zoom

			// Draw The Particle Using Our RGB Values, Fade The Particle Based On It's Life
			// 根据粒子的颜色和衰减值（透明度）绘制粒子
			glColor4f(particle[loop].r, particle[loop].g, particle[loop].b, particle[loop].life);
			glBegin(GL_TRIANGLE_STRIP);						// Build Quad From A Triangle Strip
			glTexCoord2d(1, 1); glVertex3f(x + 0.5f, y + 0.5f, z); // Top Right
			glTexCoord2d(0, 1); glVertex3f(x - 0.5f, y + 0.5f, z); // Top Left
			glTexCoord2d(1, 0); glVertex3f(x + 0.5f, y - 0.5f, z); // Bottom Right
			glTexCoord2d(0, 0); glVertex3f(x - 0.5f, y - 0.5f, z); // Bottom Left
			glEnd();										// Done Building Triangle Strip
			// 根据速度计算下一时刻的位置
			particle[loop].x += particle[loop].xi / (slowdown * 1000);// Move On The X Axis By X Speed
			particle[loop].y += particle[loop].yi / (slowdown * 1000);// Move On The Y Axis By Y Speed
			particle[loop].z += particle[loop].zi / (slowdown * 1000);// Move On The Z Axis By Z Speed

			// 根据拉力（加速度）计算下一时刻的速度
			particle[loop].xi += particle[loop].xg;			// Take Pull On X Axis Into Account
			particle[loop].yi += particle[loop].yg;			// Take Pull On Y Axis Into Account
			particle[loop].zi += particle[loop].zg;			// Take Pull On Z Axis Into Account
			particle[loop].life -= particle[loop].fade;		// Reduce Particles Life By 'Fade'
		}
		if (particle[loop].life<0.0f)					// If Particle Is Burned Out
			// 如果粒子生命值为0（死亡），生成新粒子，并初始化
		{
			particle[loop].life = 1.0f;					// Give It New Life
			particle[loop].fade = float(rand() % 100) / 1000.0f + 0.003f;	// Random Fade Value
			particle[loop].x = float(rand() % 20 - 10);             //粒子初始位置x值
			particle[loop].y = particle[loop].x*particle[loop].x - 10;             //粒子初始位置y值
			particle[loop].z = 0;             //粒子初始位置z值
			particle[loop].xi = xspeed + float((rand() % 60) - 32.0f);	// X Axis Speed And Direction
			particle[loop].yi = yspeed + float((rand() % 60) - 30.0f);	// Y Axis Speed And Direction
			particle[loop].zi = zspeed + float((rand() % 60) - 30.0f);	// Z Axis Speed And Direction
			particle[loop].zi = float((rand() % 60) - 30.0f);	// Z Axis Speed And Direction
			particle[loop].r = colors[col][0];			// Select Red From Color Table
			particle[loop].g = colors[col][1];			// Select Green From Color Table
			particle[loop].b = colors[col][2];			// Select Blue From Color Table
			col++;							// Change The Particle Color
			if (col>110)	col = 0;				// If Color Is To High Reset It
		}
	}

	for (loop = MAX_PARTICLES*0.25; loop<MAX_PARTICLES*0.5; loop++)					// Loop Through All The Particles 
	{
		if (particle[loop].active)							// If The Particle Is Active 如果粒子处于活动状态
		{
			float x = particle[loop].x;						// Grab Our Particle X Position
			float y = particle[loop].y;						// Grab Our Particle Y Position
			float z1 = particle[loop].z;
			float z = particle[loop].z + zoom;					// Particle Z Pos + Zoom

			// Draw The Particle Using Our RGB Values, Fade The Particle Based On It's Life
			// 根据粒子的颜色和衰减值（透明度）绘制粒子
			glColor4f(particle[loop].r, particle[loop].g, particle[loop].b, particle[loop].life);
			glBegin(GL_TRIANGLE_STRIP);						// Build Quad From A Triangle Strip
			glTexCoord2d(1, 1); glVertex3f(x + 0.5f, y + 0.5f, z); // Top Right
			glTexCoord2d(0, 1); glVertex3f(x - 0.5f, y + 0.5f, z); // Top Left
			glTexCoord2d(1, 0); glVertex3f(x + 0.5f, y - 0.5f, z); // Bottom Right
			glTexCoord2d(0, 0); glVertex3f(x - 0.5f, y - 0.5f, z); // Bottom Left
			glEnd();										// Done Building Triangle Strip
			// 根据速度计算下一时刻的位置
			particle[loop].x += particle[loop].xi / (slowdown * 1000);// Move On The X Axis By X Speed
			particle[loop].y += particle[loop].yi / (slowdown * 1000);// Move On The Y Axis By Y Speed
			particle[loop].z += particle[loop].zi / (slowdown * 1000);// Move On The Z Axis By Z Speed

			// 根据拉力（加速度）计算下一时刻的速度
			particle[loop].xi += particle[loop].xg;			// Take Pull On X Axis Into Account
			particle[loop].yi += particle[loop].yg;			// Take Pull On Y Axis Into Account
			particle[loop].zi += particle[loop].zg;			// Take Pull On Z Axis Into Account
			particle[loop].life -= particle[loop].fade;		// Reduce Particles Life By 'Fade'
		}
		if (particle[loop].life<0.0f)					// If Particle Is Burned Out
			// 如果粒子生命值为0（死亡），生成新粒子，并初始化
		{
			particle[loop].life = 1.0f;					// Give It New Life
			particle[loop].fade = float(rand() % 100) / 1000.0f + 0.003f;	// Random Fade Value
			particle[loop].x = -20;             //粒子初始位置x值
			particle[loop].y = float(rand() % 25 - 10);             //粒子初始位置y值
			particle[loop].z = 0;             //粒子初始位置z值
			particle[loop].xi = xspeed + float((rand() % 60) - 32.0f);	// X Axis Speed And Direction
			particle[loop].yi = yspeed + float((rand() % 60) - 30.0f);	// Y Axis Speed And Direction
			particle[loop].zi = zspeed + float((rand() % 60) - 30.0f);	// Z Axis Speed And Direction
			particle[loop].zi = float((rand() % 60) - 30.0f);	// Z Axis Speed And Direction
			particle[loop].r = colors[col][0];			// Select Red From Color Table
			particle[loop].g = colors[col][1];			// Select Green From Color Table
			particle[loop].b = colors[col][2];			// Select Blue From Color Table
			col++;							// Change The Particle Color
			if (col>110)	col = 0;				// If Color Is To High Reset It
		}
	}

	//////////////////////////////////////////////////////////////////////////
	for (loop = MAX_PARTICLES*0.5; loop<MAX_PARTICLES*0.75; loop++)					// Loop Through All The Particles 
	{
		if (particle[loop].active)							// If The Particle Is Active 如果粒子处于活动状态
		{
			float x = particle[loop].x;						// Grab Our Particle X Position
			float y = particle[loop].y;						// Grab Our Particle Y Position
			float z1 = particle[loop].z;
			float z = particle[loop].z + zoom;					// Particle Z Pos + Zoom

			// Draw The Particle Using Our RGB Values, Fade The Particle Based On It's Life
			// 根据粒子的颜色和衰减值（透明度）绘制粒子
			glColor4f(particle[loop].r, particle[loop].g, particle[loop].b, particle[loop].life);
			glBegin(GL_TRIANGLE_STRIP);						// Build Quad From A Triangle Strip
			glTexCoord2d(1, 1); glVertex3f(x + 0.5f, y + 0.5f, z); // Top Right
			glTexCoord2d(0, 1); glVertex3f(x - 0.5f, y + 0.5f, z); // Top Left
			glTexCoord2d(1, 0); glVertex3f(x + 0.5f, y - 0.5f, z); // Bottom Right
			glTexCoord2d(0, 0); glVertex3f(x - 0.5f, y - 0.5f, z); // Bottom Left
			glEnd();										// Done Building Triangle Strip
			// 根据速度计算下一时刻的位置
			particle[loop].x += particle[loop].xi / (slowdown * 1000);// Move On The X Axis By X Speed
			particle[loop].y += particle[loop].yi / (slowdown * 1000);// Move On The Y Axis By Y Speed
			particle[loop].z += particle[loop].zi / (slowdown * 1000);// Move On The Z Axis By Z Speed

			// 根据拉力（加速度）计算下一时刻的速度
			particle[loop].xi += particle[loop].xg;			// Take Pull On X Axis Into Account
			particle[loop].yi += particle[loop].yg;			// Take Pull On Y Axis Into Account
			particle[loop].zi += particle[loop].zg;			// Take Pull On Z Axis Into Account
			particle[loop].life -= particle[loop].fade;		// Reduce Particles Life By 'Fade'
		}
		if (particle[loop].life<0.0f)					// If Particle Is Burned Out
			// 如果粒子生命值为0（死亡），生成新粒子，并初始化
		{
			particle[loop].life = 1.0f;					// Give It New Life
			particle[loop].fade = float(rand() % 100) / 1000.0f + 0.003f;	// Random Fade Value

			int m = 50;
			float angle = loop * 2 * PI / m;
			particle[loop].x = r1*cos(angle) - 12;             //粒子初始位置x值
			particle[loop].y = 2.5*r1*sin(angle) + 2.8;             //粒子初始位置y值
			particle[loop].z = 0;             //粒子初始位置z值
			particle[loop].xi = xspeed + float((rand() % 60) - 32.0f);	// X Axis Speed And Direction
			particle[loop].yi = yspeed + float((rand() % 60) - 30.0f);	// Y Axis Speed And Direction
			particle[loop].zi = zspeed + float((rand() % 60) - 30.0f);	// Z Axis Speed And Direction
			particle[loop].zi = float((rand() % 60) - 30.0f);	// Z Axis Speed And Direction
			particle[loop].r = colors[col][0];			// Select Red From Color Table
			particle[loop].g = colors[col][1];			// Select Green From Color Table
			particle[loop].b = colors[col][2];			// Select Blue From Color Table
			col++;							// Change The Particle Color
			if (col>110)	col = 0;				// If Color Is To High Reset It
		}
	}
}
