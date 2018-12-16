
#include "Camera.h"

#define PI_ 3.1415926

//  类CCamera的构造函数
CCamera::CCamera()
{
	this->angle = 90.0f;
	vec3 vZero = vec3(13.5f, 0.3f, -16.5f);		// 初始化摄像机位置
	vec3 vView = vec3(13.5f, 0.3f, -15.5f);		// 初始化摄像机方向 
	vec3 vUp = vec3(0.0, 1.0, 0);		// 初始化摄像机的向上方向
	m_vPreviousPos = m_vPosition = vZero;
	m_vView = vView;
	m_vUpVector = vUp;
}

//  获取摄像机的位置
vec3 CCamera::getPos()
{
	return m_vPosition;
}

//  获取摄像机先前的位置
vec3 CCamera::getPrePos()
{
	return m_vPreviousPos;
}

// 根据一定的速度前后移动摄像机
void CCamera::MoveCamera(float speed)
{
	//记录先前节点
	m_vPreviousPos = m_vPosition;
	float rad = angle*(PI_ / 180.0);
	m_vPosition.x += cos(rad) * speed;		// 移动摄像机的位置坐标X
	m_vPosition.z += sin(rad) * speed;		// 移动摄像机的位置坐标Z
}

// 根据一定的角度左右旋转摄像机
void CCamera::RotateCamera(float angle)
{
	this->angle += angle;
	float rad = this->angle*(PI_ / 180.0);
	m_vView.x = m_vPosition.x + cos(rad);
	m_vView.z = m_vPosition.z + sin(rad);
}

// 根据一定的角度上下旋转摄像机
void CCamera::UpDownCamera(float viewY)
{
	m_vView.y += viewY;
}

//  更新摄像机方向和位置
void CCamera::Update(int positionX, int positionY, bool flag)
{
	if (flag == true){
		m_vPosition.x = positionX;
		m_vPosition.z = positionY;
	}
	RotateCamera(0);
}

// ----------------------------------------------------
void CCamera::setCamera()
{
	gluLookAt(m_vPosition.x, m_vPosition.y, m_vPosition.z, m_vView.x, m_vView.y, m_vView.z, m_vUpVector.x, m_vUpVector.y, m_vUpVector.z);

}


