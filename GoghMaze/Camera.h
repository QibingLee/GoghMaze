
#ifndef _CAMERA_H
#define _CAMERA_H

#include "Angel.h"

// 摄像机类
class CCamera {

public:

	//  摄像机类的构造函数
	CCamera();
	vec3 getPos();
	vec3 getPrePos();
	mat4 getProjmat();
	mat4 getViewjmat();
	//  移动摄像机
	void MoveCamera(float speed);
	//  旋转摄像机
	void RotateCamera(float angle);
	//  上扬摄像机
	void UpDownCamera(float viewY);
	void Update(int positionX, int positionY, bool flag);
	//  设置矩阵
	void setCamera();

private:
	//  照相机旋转角度
	float angle;
	//  摄像机的位置
	vec3 m_vPosition;
	//  摄像机先前的位置
	vec3 m_vPreviousPos;
	//  摄像机的视野
	vec3 m_vView;
	//  摄像机的向上的位置
	vec3 m_vUpVector;
};

#endif

