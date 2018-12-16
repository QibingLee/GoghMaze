
#ifndef _CAMERA_H
#define _CAMERA_H

#include "Angel.h"

// �������
class CCamera {

public:

	//  �������Ĺ��캯��
	CCamera();
	vec3 getPos();
	vec3 getPrePos();
	mat4 getProjmat();
	mat4 getViewjmat();
	//  �ƶ������
	void MoveCamera(float speed);
	//  ��ת�����
	void RotateCamera(float angle);
	//  ���������
	void UpDownCamera(float viewY);
	void Update(int positionX, int positionY, bool flag);
	//  ���þ���
	void setCamera();

private:
	//  �������ת�Ƕ�
	float angle;
	//  �������λ��
	vec3 m_vPosition;
	//  �������ǰ��λ��
	vec3 m_vPreviousPos;
	//  ���������Ұ
	vec3 m_vView;
	//  ����������ϵ�λ��
	vec3 m_vUpVector;
};

#endif

