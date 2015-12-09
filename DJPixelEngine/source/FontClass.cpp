/*
	Filename: FontClass.cpp
	Description: Source implementation of font class
*/

#include "FontClass.h"

FontClass::FontClass()
{
	m_texture = 0;
	//m_kernCount = 0;
	//m_bUseKern = true;
	m_encoding = NONE;
}

FontClass::FontClass(const FontClass& other)
{
}

FontClass::~FontClass()
{
}

bool FontClass::Initialize(TextureManager* textureManager, unsigned int texID, char* fontFilename)
{
	bool result;

	// Load the font data file
	result = LoadFontData(fontFilename);
	if(!result)
	{
		return false;
	}

	// Assign texture from texture manager
	m_texture = textureManager->GetTexture(texID);
	if(!m_texture)
	{
		return false;
	}

	return true;
}

void FontClass::Shutdown()
{
	//m_kern.clear();
	map<int, DJCharDescriptor*>::iterator it = m_chars.begin();
	while( it != m_chars.end() )
	{
		delete it->second;
		it++;
	}
	m_chars.clear();
	m_texture = 0;
}

void FontClass::SetTextEncoding(DJFontTextEncoding encoding)
{
	m_encoding = encoding;
}

void FontClass::BuildRectArray(DJFontRect* rects, float x, float y, const char *text, float spacing)
{
	int page = -1;
	int numLetters, i, j;

	// Get the number of letters in the sentence
	numLetters = GetTextLength(text);

	// Draw each letter on a rect
	j = 0;
	for(i = 0; i < numLetters;)
	{
		int charID = GetTextChar(text, i, &i);
		DJCharDescriptor *ch = GetChar(charID);
		if (ch == 0) ch = &defChar;

		rects[j].positionX = x + ch->xOffset;
		rects[j].positionY = y + ch->yOffset;
		rects[j].left = ch->x;
		rects[j].top = ch->y;
		rects[j].right = ch->x + ch->width;
		rects[j].bottom = ch->y + ch->height;

		x += ch->xAdvance;
		if(charID == ' ') {
			x += spacing;
		}
		j++;
	}
}

bool FontClass::LoadFontData(char* fontFilename)
{
	ifstream fileStream(fontFilename);
	if(!fileStream.is_open())
	{
		return false;
	}

	// Start parsing font
	ParseFont(fileStream);
	// Get Kearning Infos
	//m_kernCount = (int)m_kern.size();

	fileStream.close();

	return true;
}

bool FontClass::ParseFont(istream& fileStream)
{
	string Line;
	string Read, Key, Value;
	size_t i;
	//int first, second, amount;

	//DJKearningInfo K;
	

	while(!fileStream.eof())
	{
		stringstream LineStream;
		getline(fileStream, Line);
		LineStream << Line;

		// Read the line's type
		LineStream >> Read;
		if( Read == "info" )
		{
			// This holds info data
			while(!LineStream.eof())
			{
				stringstream Converter;
				LineStream >> Read;
				i = Read.find( '=' );
				Key = Read.substr(0, i);
				Value = Read.substr(i + 1);

				// Assign the correct value
				Converter << Value;
				if(Key == "outline")
				{
					Converter >> m_charSetOutline;
				}
			}
		}
		else if( Read == "common" )
		{
			// This holds common data
			while(!LineStream.eof())
			{
				stringstream Converter;
				LineStream >> Read;
				i = Read.find( '=' );
				Key = Read.substr(0, i);
				Value = Read.substr(i + 1);

				// Assign the correct value
				Converter << Value;
				if(Key == "lineHeight")
				{
					Converter >> m_charSetLineHeight;
				}
				else if(Key == "base")
				{
					Converter >> m_charSetBase;
				}
				else if(Key == "scaleW")
				{
					Converter >> m_charSetWidth;
				}
				else if(Key == "scaleH")
				{
					Converter >> m_charSetHeight;
				}
				else if(Key == "pages")
				{
					Converter >> m_charSetPages;
				}
			}
		}
		else if( Read == "char" )
		{
			// ID for each character
			int CharID = 0;
			DJCharDescriptor *charDescriptor = new DJCharDescriptor();

			while(!LineStream.eof())
			{
				stringstream Converter;
				LineStream >> Read;
				i = Read.find( '=' );
				Key = Read.substr(0, i);
				Value = Read.substr(i + 1);

				// Assign the correct value
				Converter << Value;
				if(Key == "id")
				{
					Converter >> CharID;
				}
				else if(Key == "x")
				{
					Converter >> charDescriptor->x;
				}
				else if(Key == "y")
				{
					Converter >> charDescriptor->y;
				}
				else if(Key == "width")
				{
					Converter >> charDescriptor->width;
				}
				else if(Key == "height")
				{
					Converter >> charDescriptor->height;
				}
				else if(Key == "xoffset")
				{
					Converter >> charDescriptor->xOffset;
				}
				else if(Key == "yoffset")
				{
					Converter >> charDescriptor->yOffset;
				}
				else if(Key == "xadvance")
				{
					Converter >> charDescriptor->xAdvance;
				}
				else if(Key == "page")
				{
					Converter >> charDescriptor->page;
				}
			}

			
			this->m_chars.insert(map<int, DJCharDescriptor*>::value_type( CharID, charDescriptor ));
		}
		/*
		else if( Read == "kernings" )
		{
			while(!LineStream.eof())
			{
				stringstream Converter;
				LineStream >> Read;
				i = Read.find( '=' );
				Key = Read.substr(0, i);
				Value = Read.substr(i + 1);

				// Assign the correct value
				Converter << Value;
				if(Key == "count")
				{
					Converter >> m_kernCount;
				}
			}
		}
		else if( Read == "kerning" )
		{
			while(!LineStream.eof())
			{
				stringstream Converter;
				LineStream >> Read;
				i = Read.find( '=' );
				Key = Read.substr(0, i);
				Value = Read.substr(i + 1);

				// Assign the correct value
				Converter << Value;
				if(Key == "first")
				{
					Converter >> K.first;
					Converter >> first;
				}
				else if(Key == "second")
				{
					Converter >> K.second;
					Converter >> second;
				}
				else if(Key == "amount")
				{
					Converter >> K.amount;
					Converter >> amount;
				}
			}

			m_kern.push_back(K);
		}
		*/
	}

	return true;
}

// Retrieve the character according to the given id
DJCharDescriptor* FontClass::GetChar(int id)
{
	map<int, DJCharDescriptor*>::iterator it = m_chars.find(id);
	if( it == m_chars.end() ) return 0;

	return it->second;
}

// Returns the number of bytes in the string until the null char
int FontClass::GetTextLength(const char *text)
{
	if( m_encoding == UTF16 )
	{
		int textLen = 0;
		for(;;)
		{
			unsigned int len;
			int r = DecodeUTF16(&text[textLen], &len);
			if( r > 0 )
				textLen += len;
			else if( r < 0 )
				textLen++;
			else
				return textLen;
		}
	}

	// Both UTF8 and standard ASCII strings can use strlen
	return (int)strlen(text);
}

// Returns the line height of the font
unsigned short FontClass::GetLineHeight()
{
	return m_charSetLineHeight;
}

#ifdef WIN32
IDirect3DTexture9* FontClass::GetFontTexture()
{
	return m_texture;
}
#endif

int FontClass::GetTextChar(const char *text, int pos, int *nextPos)
{
	int ch;
	unsigned int len;
	if( m_encoding == UTF8 )
	{
		ch = DecodeUTF8(&text[pos], &len);
		if( ch == -1 ) len = 1;
	}
	else if( m_encoding == UTF16 )
	{
		ch = DecodeUTF16(&text[pos], &len);
		if( ch == -1 ) len = 2;
	}
	else
	{
		len = 1;
		ch = (unsigned char)text[pos];
	}

	if( nextPos ) *nextPos = pos + len;
	return ch;
}

int FontClass::FindTextChar(const char *text, int start, int length, int ch)
{
	int pos = start;
	int nextPos;
	int currChar = -1;
    while( pos < length )
	{
		currChar = GetTextChar(text, pos, &nextPos);
		if( currChar == ch )
			return pos;
		pos = nextPos;
	} 

	return -1;
}

int FontClass::DecodeUTF8(const char *encodedBuffer, unsigned int *outLength)
{
	const unsigned char *buf = (const unsigned char*)encodedBuffer;
	
	int value = 0;
	int length = -1;
	unsigned char byte = buf[0];
	if( (byte & 0x80) == 0 )
	{
		// This is the only byte
		if( outLength ) *outLength = 1;
		return byte;
	}
	else if( (byte & 0xE0) == 0xC0 )
	{
		// There is one more byte
		value = int(byte & 0x1F);
		length = 2;

		// The value at this moment must not be less than 2, because 
		// that should have been encoded with one byte only.
		if( value < 2 )
			length = -1;
	}
	else if( (byte & 0xF0) == 0xE0 )
	{
		// There are two more bytes
		value = int(byte & 0x0F);
		length = 3;
	}
	else if( (byte & 0xF8) == 0xF0 )
	{
		// There are three more bytes
		value = int(byte & 0x07);
		length = 4;
	}

	int n = 1;
	for( ; n < length; n++ )
	{
		byte = buf[n];
		if( (byte & 0xC0) == 0x80 )
			value = (value << 6) + int(byte & 0x3F);
		else 
			break;
	}

	if( n == length )
	{
		if( outLength ) *outLength = (unsigned)length;
		return value;
	}

	// The byte sequence isn't a valid UTF-8 byte sequence.
	return -1;
}

int FontClass::EncodeUTF8(unsigned int value, char *outEncodedBuffer, unsigned int *outLength)
{
	unsigned char *buf = (unsigned char*)outEncodedBuffer;

	int length = -1;

	if( value <= 0x7F )
	{
		buf[0] = value;
		if( outLength ) *outLength = 1;
		return 1;
	}
	else if( value >= 0x80 && value <= 0x7FF )
	{
		// Encode it with 2 characters
		buf[0] = 0xC0 + (value >> 6);
		length = 2;
	}
	else if( value >= 0x800 && value <= 0xD7FF || value >= 0xE000 && value <= 0xFFFF )
	{
		// Note: Values 0xD800 to 0xDFFF are not valid unicode characters
		buf[0] = 0xE0 + (value >> 12);
		length = 3;
	}
	else if( value >= 0x10000 && value <= 0x10FFFF )
	{
		buf[0] = 0xF0 + (value >> 18);
		length = 4;
	}

	int n = length-1;
	for( ; n > 0; n-- )
	{
		buf[n] = 0x80 + (value & 0x3F);
		value >>= 6;
	}

	if( outLength ) *outLength = length;
	return length;
}

int FontClass::DecodeUTF16(const char *encodedBuffer, unsigned int *outLength, DJUnicodeByteOrder byteOrder)
{
	const unsigned char *buf = (const unsigned char *)encodedBuffer;
	int value = 0;
	if( byteOrder == LITTLE_ENDIAN )
	{
		value += buf[0];
		value += (unsigned int)(buf[1]) << 8; 
	}
	else
	{
		value += buf[1];
		value += (unsigned int)(buf[0]) << 8; 
	}

	if( value < 0xD800 || value > 0xDFFF )
	{
		if( outLength ) *outLength = 2;
		return value;
	}
	else if( value < 0xDC00 )
	{
		// We've found the first surrogate word
		value = ((value & 0x3FF)<<10);

		// Read the second surrogate word
		int value2 = 0;
		if( byteOrder == LITTLE_ENDIAN )
		{
			value2 += buf[2];
			value2 += (unsigned int)(buf[3]) << 8; 
		}
		else
		{
			value2 += buf[3];
			value2 += (unsigned int)(buf[2]) << 8; 
		}

		// The second surrogate word must be in the 0xDC00 - 0xDFFF range
		if( value2 < 0xDC00 || value2 > 0xDFFF )
			return -1;

		value = value + (value2 & 0x3FF) + 0x10000;
		if( outLength ) *outLength = 4;
		return value;
	}
	
	// It is an illegal sequence if a character in the 0xDC00-0xDFFF range comes first
	return -1;
}

int FontClass::EncodeUTF16(unsigned int value, char *outEncodedBuffer, unsigned int *outCharLength, DJUnicodeByteOrder byteOrder)
{
	if( value < 0x10000 )
	{
		if( byteOrder == LITTLE_ENDIAN )
		{
			outEncodedBuffer[0] = (value & 0xFF);
			outEncodedBuffer[1] = ((value >> 8) & 0xFF);
		}
		else
		{
			outEncodedBuffer[1] = (value & 0xFF);
			outEncodedBuffer[2] = ((value >> 8) & 0xFF);
		}

		if( outCharLength ) *outCharLength = 2;
		return 2;
	}
	else
	{
		value -= 0x10000;
		int surrogate1 = ((value >> 10) & 0x3FF) + 0xD800;
		int surrogate2 = (value & 0x3FF) + 0xDC00;

		if( byteOrder == LITTLE_ENDIAN )
		{
			outEncodedBuffer[0] = (surrogate1 & 0xFF);
			outEncodedBuffer[1] = ((surrogate1 >> 8) & 0xFF);
			outEncodedBuffer[2] = (surrogate2 & 0xFF);
			outEncodedBuffer[3] = ((surrogate2 >> 8) & 0xFF);
		}
		else
		{
			outEncodedBuffer[1] = (surrogate1 & 0xFF);
			outEncodedBuffer[0] = ((surrogate1 >> 8) & 0xFF);
			outEncodedBuffer[3] = (surrogate2 & 0xFF);
			outEncodedBuffer[2] = ((surrogate2 >> 8) & 0xFF);
		}

		if( outCharLength ) *outCharLength = 4;
		return 4;
	}
}
