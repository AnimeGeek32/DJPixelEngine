/*
	Filename: CameraClass.h
	Description: Header file for the camera
*/

#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_

// Include(s)
#ifdef WIN32
#include <d3dx9.h>
#endif

/*
	Class name: CameraClass
*/
class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

#ifdef WIN32
	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();
#else
	// TODO: OpenGL counterparts of 3D vectors
#endif

	void Render();
#ifdef WIN32
	void GetViewMatrix(D3DXMATRIX&);
#else
	// TODO: OpenGL counterpart of view matrix getter
#endif

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
#ifdef WIN32
	D3DXMATRIX m_viewMatrix;
#else
	// TODO: OpenGL counterpart of the view matrix
#endif
};

#endif
