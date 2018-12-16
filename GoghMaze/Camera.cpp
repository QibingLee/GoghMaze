
#include "Camera.h"

#define PI_ 3.1415926

//  ��CCamera�Ĺ��캯��
CCamera::CCamera()
{
	this->angle = 90.0f;
	vec3 vZero = vec3(13.5f, 0.3f, -16.5f);		// ��ʼ�������λ��
	vec3 vView = vec3(13.5f, 0.3f, -15.5f);		// ��ʼ����������� 
	vec3 vUp = vec3(0.0, 1.0, 0);		// ��ʼ������������Ϸ���
	m_vPreviousPos = m_vPosition = vZero;
	m_vView = vView;
	m_vUpVector = vUp;
}

//  ��ȡ�������λ��
vec3 CCamera::getPos()
{
	return m_vPosition;
}

//  ��ȡ�������ǰ��λ��
vec3 CCamera::getPrePos()
{
	return m_vPreviousPos;
}

// ����һ�����ٶ�ǰ���ƶ������
void CCamera::MoveCamera(float speed)
{
	//��¼��ǰ�ڵ�
	m_vPreviousPos = m_vPosition;
	float rad = angle*(PI_ / 180.0);
	m_vPosition.x += cos(rad) * speed;		// �ƶ��������λ������X
	m_vPosition.z += sin(rad) * speed;		// �ƶ��������λ������Z
}

// ����һ���ĽǶ�������ת�����
void CCamera::RotateCamera(float angle)
{
	this->angle += angle;
	float rad = this->angle*(PI_ / 180.0);
	m_vView.x = m_vPosition.x + cos(rad);
	m_vView.z = m_vPosition.z + sin(rad);
}

// ����һ���ĽǶ�������ת�����
void CCamera::UpDownCamera(float viewY)
{
	m_vView.y += viewY;
}

//  ��������������λ��
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


