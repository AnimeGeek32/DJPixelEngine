/*
	Filename: FontClass.h
	Description: Header file for the font class
*/

#ifndef _FONTCLASS_H_
#define _FONTCLASS_H_

#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include "TextureManager.h"
using namespace std;

/*
	Struct name: DJCharDescriptor
*/
struct DJCharDescriptor
{
	unsigned short x, y;
	unsigned short width, height;
	unsigned short xOffset, yOffset;
	unsigned short xAdvance;
	unsigned short page;
	unsigned int channel;

	DJCharDescriptor() : x(0), y(0), width(0), height(0), xOffset(0), yOffset(0), xAdvance(0), page(0), channel(0) {}
};

/*
	Struct name: DJKerningInfo
*/
/*
struct DJKearningInfo
{
	unsigned short first;
	unsigned short second;
	unsigned short amount;

	DJKearningInfo() : first(0), second(0), amount(0) {}
};
*/

/*
	Struct name: DJFontRect
*/
struct DJFontRect
{
	float positionX;
	float positionY;
	long left;
	long top;
	long right;
	long bottom;
};

/*
	Enum name: DJFontTextEncoding
*/
enum DJFontTextEncoding
{
	NONE,
	ASCII,
	UTF8,
	UTF16
};

/*
	Enum name: DJUnicodeByteOrder
*/
enum DJUnicodeByteOrder
{
	LITTLE_ENDIAN,
	BIG_ENDIAN,
};

/*
	Class name: FontClass
*/
class FontClass
{
public:
	FontClass();
	FontClass(const FontClass& other);
	~FontClass();

	bool Initialize(TextureManager* textureManager, unsigned int texID, char* fontFilename);
	void Shutdown();

	// This function will attempt to decode a UTF-8 encoded character in the buffer.
	// If the encoding is invalid, the function returns -1.
	int DecodeUTF8(const char *encodedBuffer, unsigned int *outCharLength);
	// This function will encode the value into the buffer.
	// If the value is invalid, the function returns -1, else the encoded length.
	int EncodeUTF8(unsigned int value, char *outEncodedBuffer, unsigned int *outCharLength);
	// This function will attempt to decode a UTF-16 encoded character in the buffer.
	// If the encoding is invalid, the function returns -1.
	int DecodeUTF16(const char *encodedBuffer, unsigned int *outCharLength, DJUnicodeByteOrder byteOrder = LITTLE_ENDIAN);
	// This function will encode the value into the buffer.
	// If the value is invalid, the function returns -1, else the encoded length.
	int EncodeUTF16(unsigned int value, char *outEncodedBuffer, unsigned int *outCharLength, DJUnicodeByteOrder byteOrder = LITTLE_ENDIAN);

	void SetTextEncoding(DJFontTextEncoding encoding);
	void BuildRectArray(DJFontRect* rects, float x, float y, const char *text, float spacing);

	int GetTextLength(const char *text);
	unsigned short GetLineHeight();

#ifdef WIN32
	IDirect3DTexture9* GetFontTexture();
#endif

private:
	bool LoadFontData(char* fontFilename);
	bool ParseFont(istream& fileStream);

	DJCharDescriptor* GetChar(int id);

	int GetTextChar(const char *text, int pos, int *nextPos = 0);
	int FindTextChar(const char *text, int start, int length, int ch);

	unsigned short m_charSetLineHeight;
	unsigned short m_charSetBase;
	unsigned short m_charSetWidth;
	unsigned short m_charSetHeight;
	unsigned short m_charSetPages;
	unsigned short m_charSetOutline;
	//unsigned short m_kernCount;
	//bool m_bUseKern;
	DJCharDescriptor defChar;
	DJFontTextEncoding m_encoding;
	map<int, DJCharDescriptor*> m_chars;
	//vector<DJKearningInfo> m_kern;

#ifdef WIN32
	IDirect3DTexture9* m_texture;
#endif
};

#endif
