/*
	Filename: CameraClass.cpp
	Description: Source implementation of the camera
*/

#include "CameraClass.h"

CameraClass::CameraClass()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;
}

CameraClass::CameraClass(const CameraClass& other)
{
}

CameraClass::~CameraClass()
{
}

void CameraClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
}

void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
}

#ifdef WIN32
D3DXVECTOR3 CameraClass::GetPosition()
{
	return D3DXVECTOR3(m_positionX, m_positionY, m_positionZ);
}

D3DXVECTOR3 CameraClass::GetRotation()
{
	return D3DXVECTOR3(m_rotationX, m_rotationY, m_rotationZ);
}
#endif

void CameraClass::Render()
{
	float yaw, pitch, roll;
#ifdef WIN32
	D3DXVECTOR3 up, position, lookAt;
	D3DXMATRIX rotationMatrix;

	// Setup the vector that points upwards
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	// Setup where the camera is looking by default
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians (PI / 180.0f or 0.0174532925f)
	pitch = D3DXToRadian(m_rotationX);
	yaw = D3DXToRadian(m_rotationY);
	roll = D3DXToRadian(m_rotationZ);

	// Create the rotation matrix from the yaw, pitch, and roll values
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// Transform the lookAt and up vector by the roation matrix so the view is correctly rotated at teh origin
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// Translate the rotated camera position to the location of the viewer
	lookAt = position + lookAt;

	// Finally create the view matrix from the three updated vectors
	D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);
#endif
}

#ifdef WIN32
void CameraClass::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
}
#endif
