/*
	Filename: D3DClass.h
	Description: Header file for Direct3D class
*/

#ifndef _D3DCLASS_H_
#define _D3DCLASS_H_

// Include(s)
#include <d3d9.h>
#include <d3dx9.h>

// Linking
#pragma comment(lib, "d3d9.lib")
#ifdef _DEBUG
#pragma comment(lib, "d3dx9d.lib")
#else
#pragma comment(lib, "d3dx9.lib")
#endif

/*
	Class name: D3DClass
*/
class D3DClass
{
public:
	D3DClass();
	D3DClass(const D3DClass&);
	~D3DClass();

	bool Initialize(int, int, bool, HWND, bool, float, float);
	void Shutdown();

	void BeginScene(int, int, int, int);
	void EndScene();

	IDirect3D9* GetD3DObject();
	IDirect3DDevice9* GetD3DDevice();
	ID3DXSprite* GetD3DSpriteHandler();

	//void GetProjectionMatrix(D3DXMATRIX&);
	void GetWorldMatrix(D3DXMATRIX&);
	void GetOrthoMatrix(D3DXMATRIX&);

	//void GetVideoCardInfo(char*, int&);
private:
	bool m_vsync_enabled;
	//int m_videoCardMemory;
	//char m_videoCardDescription[128];
	IDirect3D9* m_d3dObject;
	IDirect3DDevice9* m_d3dDevice;
	ID3DXSprite* m_d3dSpriteHandler;
	//D3DXMATRIX m_projectionMatrix;
	D3DXMATRIX m_worldMatrix;
	D3DXMATRIX m_orthoMatrix;
};

#endif