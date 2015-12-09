/*
	Filename: TextClass.cpp
	Description: Source implementation of text object
*/

#include "TextClass.h"

TextClass::TextClass()
{
	m_Font = 0;
	m_positionX = 0;
	m_positionY = 0;
}

TextClass::TextClass(const TextClass& other)
{
}

TextClass::~TextClass()
{
}

bool TextClass::Initialize( FontClass *targetFont )
{
	// Point to the existing font object
	m_Font = targetFont;
	if(!m_Font) {
		return false;
	}

	return true;
}

void TextClass::Shutdown()
{
	// Clear text lines
	for(int i = m_textLines.size() - 1; i >= 0; i--) {
		delete[] m_textLines[i]->rectBuffer;
		m_textLines[i]->rectBuffer = 0;
		delete m_textLines[i];
		m_textLines[i] = 0;
		m_textLines.pop_back();
	}

	m_textLines.clear();

	// Unassign the pointing font object
	if(m_Font) {
		m_Font = 0;
	}
}

void TextClass::SetPosition( float x, float y )
{
	m_positionX = x;
	m_positionY = y;
}

bool TextClass::AddTextLine( int maxLength )
{
	// Create a new text line object
	DJTextLine *tempLine = new DJTextLine();
	tempLine->maxLength = maxLength;

	tempLine->rectBuffer = new DJFontRect[maxLength];

	// Initialize the rect array to zeroes
	memset(tempLine->rectBuffer, 0, (sizeof(DJFontRect) * maxLength));

	// Add the just created text line to an array of text lines
	m_textLines.push_back(tempLine);

	tempLine = 0;

	return true;
}

bool TextClass::UpdateTextLine( int userID, const char* text )
{
	int numLetters;
	float currentDrawX, currentDrawY;

	// Check if the userID is valid
	if(userID >= (int)m_textLines.size())
	{
		return false;
	}

	// Get the number of letters in the text line
	numLetters = m_Font->GetTextLength(text);

	// Check for possible buffer overflow
	if(numLetters > m_textLines[userID]->maxLength)
	{
		return false;
	}

	// Initialize the rect array with zeroes
	memset(m_textLines[userID]->rectBuffer, 0, (sizeof(DJFontRect) * m_textLines[userID]->maxLength));

	// Calculate drawing positions
	currentDrawX = m_positionX;
	currentDrawY = m_positionY + (userID * (int)m_Font->GetLineHeight());

	// Build the rect array from the given text and draw location
	m_Font->BuildRectArray(m_textLines[userID]->rectBuffer, currentDrawX, currentDrawY, text, 0.0f);

	return true;
}

void TextClass::RemoveTextLine( int userID )
{
	if(m_textLines[userID]->rectBuffer)
	{
		delete [] m_textLines[userID]->rectBuffer;
		m_textLines[userID]->rectBuffer = 0;

		// Remove the text line from the vector
		delete m_textLines[userID];
		m_textLines[userID] = 0;
		m_textLines.erase(m_textLines.begin() + userID);
	}
}

FontClass* TextClass::GetFont()
{
	return m_Font;
}

DJTextLine* TextClass::GetTextLine( int lineID )
{
	return m_textLines[lineID];
}

int TextClass::GetTextLinesSize()
{
	return m_textLines.size();
}

#ifdef WIN32
D3DXVECTOR3 TextClass::GetPosition()
{
	return D3DXVECTOR3( m_positionX, m_positionY, 0 );
}
#endif
