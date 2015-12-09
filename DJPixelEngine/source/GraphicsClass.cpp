/*
	Filename: GraphicsClass.cpp
	Description: Source implementation of graphics class
*/

#include "GraphicsClass.h"

GraphicsClass::GraphicsClass()
{
	bFullScreen = false;
	bVSyncEnabled = true;
	m_screenWidth = 640;
	m_screenHeight = 480;

#ifdef WIN32
	m_D3D = 0;
#endif
	m_Camera = 0;
	m_font = 0;
	m_textObject = 0;
}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}

GraphicsClass::~GraphicsClass()
{
}

#ifdef WIN32
bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
#else
bool GraphicsClass::Initialize(int screenWidth, int screenHeight)
#endif
{
	bool result;

#ifdef WIN32
	// Create the Direct3D object
	m_D3D = new D3DClass();
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object
	result = m_D3D->Initialize(screenWidth, screenHeight, bVSyncEnabled, hwnd, bFullScreen, screenDepth, screenNear);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}
#endif

	// Create the camera object
	m_Camera = new CameraClass();
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	// Create the texture manager
	m_textureManager = new TextureManager();
#ifdef WIN32
	// Add a texture created from a file
	if(!m_textureManager->AddTexture(m_D3D, L"data/GameFont01_0.png"))
	{
		MessageBox(hwnd, L"DJPixelEngine Error", L"Could not find font texture", MB_ICONERROR | MB_OK);
	}
	if(!m_textureManager->AddTexture(m_D3D, L"data/testSprite.png"))
	{
		MessageBox(hwnd, L"DJPixelEngine Error", L"Could not find texture", MB_ICONERROR | MB_OK);
	}
#endif

	// Create the sprite manager
	m_spriteManager = new SpriteManager();

	// Add two temporary test sprites, assign textures to them and add them to the sprite manager.
	SpriteClass* m_TestSprite = new SpriteClass();
	m_TestSprite->Initialize(m_textureManager, 1);
	m_TestSprite->SetPosition(16.0f, 32.0f, 0.0f);
	m_TestSprite->SetClipRect(0, 0, 32, 64);
	SpriteClass* m_TestSprite2 = new SpriteClass();
	m_TestSprite2->Initialize(m_textureManager, 1);
	m_TestSprite2->SetPosition(40.0f, 50.0f, 0.0f);
	m_TestSprite2->SetClipRect(32, 0, 64, 64);

	m_spriteManager->AddSprite(m_TestSprite);
	m_spriteManager->AddSprite(m_TestSprite2);

	// Create font object
	m_font = new FontClass();
	if(!m_font) {
		return false;
	}
	m_font->Initialize(m_textureManager, 0, "data/GameFont01.fnt");
	m_font->SetTextEncoding(UTF8);
	//m_font->SetTextEncoding(UTF16);

	// Create the text object
	m_textObject = new TextClass();
	if(!m_textObject) {
		return false;
	}
	m_textObject->SetPosition(5.0f, 5.0f);

	// Initialize the text object
	result = m_textObject->Initialize(m_font);
	if(!result) {
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}
	// Write text
	m_textObject->AddTextLine(20);
	m_textObject->AddTextLine(20);
	m_textObject->AddTextLine(20);
	m_textObject->AddTextLine(20);
	m_textObject->AddTextLine(20);
	m_textObject->UpdateTextLine(0, "This is a test text.");
	// Testing Shift-JIS to Unicode conversion
	/*
	char japLine[20] = "これはテストです";
	char japBuf[20];
	unsigned int cl, jlen = 0;
	unsigned char tempCharacter = (*(char*)japLine) & 0xFF;
	unsigned char tempCharacter2 = japLine[1] & 0xFF;
	//unsigned short tempResult = ((short)tempCharacter2 << 8) + ((short)tempCharacter);
	unsigned short kanaTest = ((short)japLine[0]) + ((short)japLine[1] << 8);
	unsigned char tempCharacter3 = japLine[2] & 0xFF;
	unsigned char tempCharacter4 = japLine[3] & 0xFF;
	//unsigned short tempResult2 = ((short)tempCharacter4 << 8) + ((short)tempCharacter3);
	unsigned short kanaTest2 = ((short)japLine[2]) + ((short)japLine[3] << 8);
	unsigned char tempCharacter5 = japLine[4] & 0xFF;
	unsigned char tempCharacter6 = japLine[5] & 0xFF;
	//unsigned short tempResult3 = ((short)tempCharacter6 << 8) + ((short)tempCharacter5);
	unsigned short kanaTest3 = ((short)japLine[4]) + ((short)japLine[5] << 8);
	//tempCharacter = tempCharacter << 32;
	//jlen += m_font->EncodeUTF8(84, japBuf, &cl);
	//japBuf[jlen] = 0;
	//m_textObject->UpdateTextLine(1, japBuf);
	//m_textObject->UpdateTextLine(1, "Second line test.");
	//m_textObject->UpdateTextLine(1, "これはテストです");
	*/

	return true;
}

void GraphicsClass::Shutdown()
{
	// Release the text object
	if(m_textObject)
	{
		m_textObject->Shutdown();
		delete m_textObject;
		m_textObject = 0;
	}

	// Release the font object
	if(m_font)
	{
		m_font->Shutdown();
		delete m_font;
		m_font = 0;
	}

	// Release the sprite manager
	if(m_spriteManager)
	{
		m_spriteManager->Shutdown();
		delete m_spriteManager;
		m_spriteManager = 0;
	}

	// Release the texture manager
	if(m_textureManager)
	{
		m_textureManager->Shutdown();
		delete m_textureManager;
		m_textureManager = 0;
	}

	// Release the camera object
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

#ifdef WIN32
	// Release the Direct3D object
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}
#endif
}

bool GraphicsClass::Frame(InputClass* inputClass)
{
	char tempString[16];
	char mouseString[16];
	int mouseX, mouseY;
	float leftAX, leftAY, rightAX, rightAY;
	bool result;
	
	// Retrieve mouse cursor location
	inputClass->GetMouseLocation(mouseX, mouseY);
	// Retrive joystick input info
	inputClass->GetJoystickState(leftAX, leftAY, rightAX, rightAY);

	// Convert the mouseX integer to string format
	_itoa_s(mouseX, tempString, 10);

	// Setup the mouseX string
	strcpy_s(mouseString, "Mouse X: ");
	strcat_s(mouseString, tempString);

	// Update the mouseX text line
	result = m_textObject->UpdateTextLine(1, mouseString);
	if(!result)
	{
		return false;
	}

	// Convert the mouseY integer to string format
	_itoa_s(mouseY, tempString, 10);

	// Setup the mouseY string
	strcpy_s(mouseString, "Mouse Y: ");
	strcat_s(mouseString, tempString);

	// Update the mouseY text line
	result = m_textObject->UpdateTextLine(2, mouseString);
	if(!result)
	{
		return false;
	}

	// Update player's position
	float playerX = m_spriteManager->GetSprite(0)->GetPosition().x;
	float playerY = m_spriteManager->GetSprite(0)->GetPosition().y;
	playerX += leftAX;
	playerY += leftAY;
	//playerX += 0;
	//playerY += 0;
	m_spriteManager->GetSprite(0)->SetPosition(playerX, playerY, 0.0f);

	// Set the position of the camera
	float cameraX = m_Camera->GetPosition().x;
	float cameraY = m_Camera->GetPosition().y;

	// Output camera's position
	_itoa_s(cameraX, tempString, 10);
	strcpy_s(mouseString, "Camera X: ");
	strcat_s(mouseString, tempString);
	result = m_textObject->UpdateTextLine(3, mouseString);
	if(!result)
	{
		return false;
	}
	_itoa_s(cameraY, tempString, 10);
	strcpy_s(mouseString, "Camera Y: ");
	strcat_s(mouseString, tempString);
	result = m_textObject->UpdateTextLine(4, mouseString);
	if(!result)
	{
		return false;
	}

	cameraX += rightAX;
	cameraY += rightAY;
	m_Camera->SetPosition(cameraX, cameraY, -10.0f);
	return true;
}

bool GraphicsClass::getFullscreen()
{
	return bFullScreen;
}

bool GraphicsClass::getVSyncEnabled()
{
	return bVSyncEnabled;
}

int GraphicsClass::getScreenWidth()
{
	return m_screenWidth;
}

int GraphicsClass::getScreenHeight()
{
	return m_screenHeight;
}

bool GraphicsClass::Render()
{
	//bool result;
#ifdef WIN32
	D3DXMATRIX viewMatrix, orthoMatrix, worldMatrix;

	// Clear the buffers to begin the scene
	m_D3D->BeginScene(0, 40, 100, 0xff);

	// Generate the view matrix based on the camera's position
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and D3D objects
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// Put game scene objects here
	// Build our matrix to rotate, scale and position our sprite
	D3DXMATRIX tempMat;
	D3DXVECTOR2 scaling(2.0f, 2.0f);
	D3DXVECTOR2 cameraPosition(m_Camera->GetPosition().x, m_Camera->GetPosition().y);
	//D3DXMatrixTransformation2D(&tempMat, NULL, 0.0f, &scaling, NULL, 0.0f, NULL);
	//m_D3D->GetD3DSpriteHandler()->SetTransform(&tempMat);
	for(int i = 0; i < m_spriteManager->GetSize(); i++)
	{
		D3DXVECTOR2 tempVector2 = m_spriteManager->GetSprite(i)->GetCenter();
		tempVector2.x *= 2.0f;
		tempVector2.y *= 2.0f;
		D3DXMatrixTransformation2D(&tempMat, &tempVector2, 0.0f, &scaling, NULL, 0.0f, &cameraPosition);
		m_D3D->GetD3DSpriteHandler()->SetTransform(&tempMat);
		m_D3D->GetD3DSpriteHandler()->Draw(m_spriteManager->GetSprite(i)->GetTexture(), &m_spriteManager->GetSprite(i)->GetClipRect(), NULL, &m_spriteManager->GetSprite(i)->GetPosition(), 0xFFFFFFFF);
	}

	// Render text strings
	for(int j = 0; j < m_textObject->GetTextLinesSize(); j++)
	{
		int rectLength = m_textObject->GetTextLine(j)->maxLength;

		for(int k = 0; k < rectLength; k++)
		{
			RECT tempFontRect;
			D3DXVECTOR3 tempFontRectPosition;
			D3DXVECTOR2 tempVector2;
			tempVector2.x = 1.0f;
			tempVector2.y = 1.0f;
			tempFontRect.left = m_textObject->GetTextLine(j)->rectBuffer[k].left;
			tempFontRect.top = m_textObject->GetTextLine(j)->rectBuffer[k].top;
			tempFontRect.right = m_textObject->GetTextLine(j)->rectBuffer[k].right;
			tempFontRect.bottom = m_textObject->GetTextLine(j)->rectBuffer[k].bottom;
			tempFontRectPosition.x = m_textObject->GetTextLine(j)->rectBuffer[k].positionX;
			tempFontRectPosition.y = m_textObject->GetTextLine(j)->rectBuffer[k].positionY;
			tempFontRectPosition.z = 0.0f;

			D3DXMatrixTransformation2D(&tempMat, NULL, 0.0f, &scaling, NULL, 0.0f, NULL);
			m_D3D->GetD3DSpriteHandler()->SetTransform(&tempMat);
			m_D3D->GetD3DSpriteHandler()->Draw(m_textObject->GetFont()->GetFontTexture(), &tempFontRect, NULL, &tempFontRectPosition, 0xFFFFFFFF);
		}
	}

	// Set sampler states to point so they look pixely
	m_D3D->GetD3DDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_D3D->GetD3DDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_D3D->GetD3DDevice()->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	// Present the rendered scene to the screen
	m_D3D->EndScene();
#endif
	return true;
}
