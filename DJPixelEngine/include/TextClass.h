/*
	Filename: TextClass.h
	Description: Header file for the text object.
*/

#ifndef _TEXTCLASS_H_
#define _TEXTCLASS_H_

#include "FontClass.h"

/*
	Struct name: DJTextLine
*/
struct DJTextLine
{
	int maxLength;
	DJFontRect *rectBuffer;

	DJTextLine() : maxLength(0) {}
};

/*
	Class name: TextClass
*/
class TextClass
{
public:
	TextClass();
	TextClass(const TextClass& other);
	~TextClass();

	bool Initialize( FontClass *targetFont );
	void Shutdown();
	void SetPosition( float x, float y );
	bool AddTextLine( int maxLength );
	bool UpdateTextLine( int lineID, const char* text );
	void RemoveTextLine( int lineID );

	FontClass* GetFont();
	DJTextLine* GetTextLine( int lineID );
	int GetTextLinesSize();
#ifdef WIN32
	D3DXVECTOR3 GetPosition();
#endif

private:
	FontClass *m_Font;
	float m_positionX, m_positionY;
	vector<DJTextLine*> m_textLines;
};

#endif
