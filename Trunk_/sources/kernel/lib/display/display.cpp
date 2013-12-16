#include "display.h"

void Display::modifyCharTable1()
{
	outPort(REGISTERS_VGA_SEQUENCER_INDEX, 0);
	outPort(REGISTERS_VGA_SEQUENCER_DATA, 1);
	outPort(REGISTERS_VGA_SEQUENCER_INDEX, 0);
	outPort(REGISTERS_VGA_SEQUENCER_DATA, 3);
	
	outPort(REGISTERS_VGA_SEQUENCER_INDEX, 2);
	unsigned char seq2 = inPort(REGISTERS_VGA_SEQUENCER_DATA);

	outPort(REGISTERS_VGA_SEQUENCER_INDEX, 4);
	unsigned char seq4 = inPort(REGISTERS_VGA_SEQUENCER_DATA);

	outPort(REGISTERS_VGA_SEQUENCER_DATA, seq4 | 0x04);

	outPort(REGISTERS_VGA_GRAPHICS_CONTROLLER_INDEX, 4);
	unsigned char gc4 = inPort(REGISTERS_VGA_GRAPHICS_CONTROLLER_DATA);

	outPort(REGISTERS_VGA_GRAPHICS_CONTROLLER_INDEX, 5);
	unsigned char gc5 = inPort(REGISTERS_VGA_GRAPHICS_CONTROLLER_DATA);

	outPort(REGISTERS_VGA_GRAPHICS_CONTROLLER_DATA, gc5 & ~0x10);

	outPort(REGISTERS_VGA_GRAPHICS_CONTROLLER_INDEX, 6);
	unsigned char gc6 = inPort(REGISTERS_VGA_GRAPHICS_CONTROLLER_DATA);

	outPort(REGISTERS_VGA_GRAPHICS_CONTROLLER_DATA, gc6 & ~0x02);

	unsigned char pmask;
	unsigned char p = 2;
	
	pmask = 1 << p;

	outPort(REGISTERS_VGA_GRAPHICS_CONTROLLER_INDEX, 4);
	outPort(REGISTERS_VGA_GRAPHICS_CONTROLLER_DATA, p);

	outPort(REGISTERS_VGA_SEQUENCER_INDEX, 2);
	outPort(REGISTERS_VGA_SEQUENCER_DATA, pmask);

	unsigned int pFont = FONT_AREA_ADDRESS;
	for (int j = 0; j < 256; j++)
	{
		for (int i = 0; i < 16; i++)
			*reinterpret_cast<unsigned char*>(pFont++) = HV8X16_Z[j * 16 + i];

		pFont += 16;
	}

	outPort(REGISTERS_VGA_SEQUENCER_INDEX, 0);
	outPort(REGISTERS_VGA_SEQUENCER_DATA, 1);
	outPort(REGISTERS_VGA_SEQUENCER_INDEX, 0);
	outPort(REGISTERS_VGA_SEQUENCER_DATA, 3);
	outPort(REGISTERS_VGA_SEQUENCER_INDEX, 2);
	outPort(REGISTERS_VGA_SEQUENCER_DATA, seq2);
	outPort(REGISTERS_VGA_SEQUENCER_INDEX, 4);
	outPort(REGISTERS_VGA_SEQUENCER_DATA, seq4);
	outPort(REGISTERS_VGA_GRAPHICS_CONTROLLER_INDEX, 4);
	outPort(REGISTERS_VGA_GRAPHICS_CONTROLLER_DATA, gc4);
	outPort(REGISTERS_VGA_GRAPHICS_CONTROLLER_INDEX, 5);
	outPort(REGISTERS_VGA_GRAPHICS_CONTROLLER_DATA, gc5);
	outPort(REGISTERS_VGA_GRAPHICS_CONTROLLER_INDEX, 6);
	outPort(REGISTERS_VGA_GRAPHICS_CONTROLLER_DATA, gc6);
}

void Display::modifyCharTable()
{
	outPort(REGISTERS_VGA_SEQUENCER_INDEX, 0);
	outPort(REGISTERS_VGA_SEQUENCER_DATA, 1);
	outPort(REGISTERS_VGA_SEQUENCER_INDEX, 0);
	outPort(REGISTERS_VGA_SEQUENCER_DATA, 3);
	outPort(REGISTERS_VGA_SEQUENCER_INDEX, 4);
	outPort(REGISTERS_VGA_SEQUENCER_DATA, 7);
	outPort(REGISTERS_VGA_SEQUENCER_INDEX, 2);
	outPort(REGISTERS_VGA_SEQUENCER_DATA, 4);
	outPort(REGISTERS_VGA_GRAPHICS_CONTROLLER_INDEX, 4);
	outPort(REGISTERS_VGA_GRAPHICS_CONTROLLER_DATA, 2);
	outPort(REGISTERS_VGA_GRAPHICS_CONTROLLER_INDEX, 5);
	outPort(REGISTERS_VGA_GRAPHICS_CONTROLLER_DATA, 0);
	outPort(REGISTERS_VGA_GRAPHICS_CONTROLLER_INDEX, 6);
	outPort(REGISTERS_VGA_GRAPHICS_CONTROLLER_DATA, 4);

	unsigned int pFont = FONT_AREA_ADDRESS;
	for (int j = 0; j < 256; j++)
	{
		for (int i = 0; i < 32; i++)
			*reinterpret_cast<unsigned char*>(pFont++) = HV8X16_Z[j * 16 + i];

		//pFont += 16;
	}

	outPort(REGISTERS_VGA_SEQUENCER_INDEX, 0);
	outPort(REGISTERS_VGA_SEQUENCER_DATA, 1);
	outPort(REGISTERS_VGA_SEQUENCER_INDEX, 0);
	outPort(REGISTERS_VGA_SEQUENCER_DATA, 3);
	outPort(REGISTERS_VGA_SEQUENCER_INDEX, 4);
	outPort(REGISTERS_VGA_SEQUENCER_DATA, 3);
	outPort(REGISTERS_VGA_SEQUENCER_INDEX, 2);
	outPort(REGISTERS_VGA_SEQUENCER_DATA, 3);
	outPort(REGISTERS_VGA_GRAPHICS_CONTROLLER_INDEX, 4);
	outPort(REGISTERS_VGA_GRAPHICS_CONTROLLER_DATA, 0);
	outPort(REGISTERS_VGA_GRAPHICS_CONTROLLER_INDEX, 5);
	outPort(REGISTERS_VGA_GRAPHICS_CONTROLLER_DATA, 0x10);
	outPort(REGISTERS_VGA_GRAPHICS_CONTROLLER_INDEX, 6);
	outPort(REGISTERS_VGA_GRAPHICS_CONTROLLER_DATA, 0x0e);
}

void Display::disableCursor()
{
	outPort(REGISTERS_VGA_CRTC_INDEX, 0x0a);
	outPort(REGISTERS_VGA_CRTC_DATA, 0x20);	
}

Display::Display()
{
	modifyCharTable();
	disableCursor();
	clearScreen();
}

void Display::clearScreen()
{
	unsigned short *t = reinterpret_cast<unsigned short*>(VIDEO_BASE_ADDRESS);
	unsigned int size = 80 * 25;
	unsigned short cl = BACKGROUND_COLOR;
	cl = cl << 4;
	cl += BACKGROUND_COLOR;
	cl = cl << 8;

	while (size-- > 0) 
	{
		*t++ = static_cast<unsigned short>(cl);
	}
}

void Display::print(const char* pStr, int x, int y, bool blinking)
{
	_asm
	{
		_asm	pushfd
		_asm	pushad
	}

	unsigned int vOffset = VIDEO_BASE_ADDRESS +  y * 160 + x * 2;
	unsigned int color =  blinking;
	color = color << 3;
	color += BACKGROUND_COLOR;
	color = color << 4;
	color += CHAR_COLOR;
	color = color << 8;

	_asm
	{
		mov     esi,pStr
		mov	    edi,vOffset
		mov     eax,color
		cld
_printString_12:
		lodsb
		cmp     al,0
		je      _printString_22
		stosw
		jmp		_printString_12
_printString_22: 		
		_asm	popad
		_asm	popfd
	}
}

void Display::printChar(unsigned char _char, unsigned int x, unsigned int y, COLOR color, bool blinking)
{
	unsigned int vOffset = VIDEO_BASE_ADDRESS +  y * 160 + x * 2;
	unsigned int _color =  blinking;
	_color = _color << 3;
	_color += BACKGROUND_COLOR;
	_color = _color << 4;
	_color += color;//CHAR_COLOR;
	_color = _color << 8;
	*reinterpret_cast<unsigned short*>(vOffset) = _color + _char;
}

char Display::toChar(unsigned char value)
{
	if (value < 10)
		return value + 48;
	else
		return value + 87;
}

char* Display::toString2Num(unsigned char value, char* cStr)
{
	unsigned char n1;

	cStr[0] = 0;
	cStr[1] = 0;
	cStr[2] = 0;

	n1 = value / 10;
	value -= (n1 * 10);

	cStr[0] = n1 + 48;
	cStr[1] = value + 48;

	return cStr;
}

char* Display::toString2Num3(unsigned char value, char* cStr)
{
	unsigned char n1;
	unsigned char n2;

	cStr[0] = 0;
	cStr[1] = 0;
	cStr[2] = 0;
	cStr[3] = 0;

	n1 = value / 100;
	value -= (n1 * 100);
	n2 = value / 10;
	value -= (n2 * 10);

	unsigned char index = 0;
	if (n1 != 0)
	{
		cStr[index++] = n1 + 48;
	}
	else
		cStr[index++] = ' ';

	if (n2 != 0)
	{
		cStr[index++] = n2 + 48;
	}
	else
		cStr[index++] = ' ';

	cStr[2] = value + 48;

	return cStr;
}

char* Display::toString2NumHex(unsigned char value, char* cStr)
{
	unsigned char n1;

	cStr[0] = 0;
	cStr[1] = 0;
	cStr[2] = 0;

	n1 = value / 16;
	value -= (n1 * 16);

	cStr[0] = toChar(n1);
	cStr[1] = toChar(value);

	return cStr;
}

char* Display::toString4Num(int value, char* cStr)
{
	unsigned char n0, n1, n2;

	cStr[0] = 0;
	cStr[1] = 0;
	cStr[2] = 0;
	cStr[3] = 0;
	cStr[4] = 0;

	n0 = value / 1000;
	value -= (n0 * 1000);

	n1 = value / 100;
	value -= (n1 * 100);

	n2 = value / 10;
	value -= (n2 * 10);

	cStr[0] = n0 + 48;
	cStr[1] = n1 + 48;
	cStr[2] = n2 + 48;
	cStr[3] = value + 48;

	return cStr;
}

char* Display::toString3Num(unsigned char value, char* cStr)
{
	unsigned char n0, n1;

	cStr[0] = 0;
	cStr[1] = 0;
	cStr[2] = 0;
	cStr[3] = 0;

	n0 = value / 100;
	value -= (n0 * 100);

	n1 = value / 10;
	value -= (n1 * 10);

	cStr[0] = n0 + 48;
	cStr[1] = n1 + 48;
	cStr[2] = value + 48;

	return cStr;
}

char* Display::toString(unsigned char value, char* cStr)
{
	cStr[0] = toChar(((value & 0xf0) >> 4));
	cStr[1] = toChar((value & 0x0f));
	cStr[2] = 0;

	return cStr;
}

char* Display::toString(unsigned int value, char* cStr)
{
	cStr[0] = toChar(((value & 0xf0000000) >> 28));
	cStr[1] = toChar(((value & 0x0f000000) >> 24));
	cStr[2] = toChar(((value & 0x00f00000) >> 20));
	cStr[3] = toChar(((value & 0x000f0000) >> 16));
	cStr[4] = toChar(((value & 0x0000f000) >> 12));
	cStr[5] = toChar(((value & 0x00000f00) >> 8));
	cStr[6] = toChar(((value & 0x000000f0) >> 4));
	cStr[7] = toChar((value & 0x0000000f));
	cStr[8] = 0;

	return cStr;
}

char* Display::toStringDecimal(unsigned int value, char* cStr)
{
	int position = 0;
	unsigned char num = value / 1000000000;
	
	if (num != 0)
		cStr[position++] = toChar(num);

	value = value % 1000000000;

	num = value / 100000000;
	if ((position > 0) || (num != 0))
		cStr[position++] = toChar(num);

	value = value % 100000000;

	num = value / 10000000;
	if ((position > 0) || (num != 0))
		cStr[position++] = toChar(num);

	value = value % 10000000;

	num = value / 1000000;
	if ((position > 0) || (num != 0))
		cStr[position++] = toChar(num);

	value = value % 1000000;

	num = value / 100000;
	if ((position > 0) || (num != 0))
		cStr[position++] = toChar(num);

	value = value % 100000;

	num = value / 10000;
	if ((position > 0) || (num != 0))
		cStr[position++] = toChar(num);

	value = value % 10000;

	num = value / 1000;
	if ((position > 0) || (num != 0))
		cStr[position++] = toChar(num);

	value = value % 1000;

	num = value / 100;
	if ((position > 0) || (num != 0))
		cStr[position++] = toChar(num);

	value = value % 100;

	num = value / 10;
	if ((position > 0) || (num != 0))
		cStr[position++] = toChar(num);

	value = value % 10;

	cStr[position++] = toChar(value);

	return cStr;
}

char* Display::toStringDecimalSigned(int value, char* cStr)
{
	if (value >= 0)
		return toStringDecimal(value, cStr);

	int position = 0;

	cStr[0] = '-';
	toStringDecimal(value * -1, ++cStr);
	return --cStr;
}

char* Display::toStringWithoutNull(unsigned int value, char* cStr)
{
	int position = 0;
	char num = value / 1000000000;
	if (num != 0)
		cStr[position++] = toChar(num);

	value = value % 1000000000;

	num = value / 100000000;
	if ((position > 0) || (num != 0))
		cStr[position++] = toChar(num);

	value = value % 100000000;

	num = value / 10000000;
	if ((position > 0) || (num != 0))
		cStr[position++] = toChar(num);

	value = value % 10000000;

	num = value / 1000000;
	if ((position > 0) || (num != 0))
		cStr[position++] = toChar(num);

	value = value % 1000000;

	num = value / 100000;
	if ((position > 0) || (num != 0))
		cStr[position++] = toChar(num);

	value = value % 100000;

	num = value / 10000;
	if ((position > 0) || (num != 0))
		cStr[position++] = toChar(num);

	value = value % 10000;

	num = value / 1000;
	if ((position > 0) || (num != 0))
		cStr[position++] = toChar(num);

	value = value % 1000;

	num = value / 100;
	if ((position > 0) || (num != 0))
		cStr[position++] = toChar(num);

	value = value % 100;

	num = value / 10;
	if ((position > 0) || (num != 0))
		cStr[position++] = toChar(num);

	value = value % 10;

	cStr[position++] = toChar(value);

	return cStr;
}

char* Display::toStringFloat(float value, char* cStr)
{
	char* _cStr = cStr;
	bool sign = (value < 0.0f);
	if (sign)
	{
		cStr[0] = '-';
		_cStr++;
		value *= -1;
	}

	unsigned int leftNum = value;
	unsigned int d = (value - leftNum) * 10000;

	toStringDecimal(leftNum, _cStr);
	while (*_cStr++ != 0) {}
	_cStr--;
	*_cStr = '.';
	_cStr++;
	toStringDecimal(d, _cStr);
	char* __cStr = _cStr;

	while (*__cStr++ != 0) {}
	while (__cStr != _cStr) 
	{
		if ((*(__cStr - 1) == '0') && (*(__cStr - 2) != '.'))
			*(__cStr - 1) = 0;

		__cStr--;
	}

	return cStr;
}

char* Display::toStringTime(unsigned int value, char* cStr)
{
	cStr[0] = toChar(((value & 0x00f00000) >> 20));
	cStr[1] = toChar(((value & 0x000f0000) >> 16));
	cStr[2] = ':';
	cStr[3] = toChar(((value & 0x0000f000) >> 12));
	cStr[4] = toChar(((value & 0x00000f00) >> 8));
	cStr[5] = ':';
	cStr[6] = toChar(((value & 0x000000f0) >> 4));
	cStr[7] = toChar((value & 0x0000000f));
	cStr[8] = 0;

	return cStr;
}

char* Display::toStringDate(unsigned int value, char* cStr)
{
	cStr[0] = toChar(((value & 0x000000f0) >> 4));
	cStr[1] = toChar((value & 0x0000000f));
	cStr[2] = '.';
	cStr[3] = toChar(((value & 0x0000f000) >> 12));
	cStr[4] = toChar(((value & 0x00000f00) >> 8));
	cStr[5] = '.';
	cStr[6] = toChar(((value & 0x00f00000) >> 20));
	cStr[7] = toChar(((value & 0x000f0000) >> 16));
	cStr[8] = 0;

	return cStr;
}

void Display::printUInt(unsigned int value, unsigned int x, unsigned int y, bool blinking)
{
	char cStr[12];
	print(toString(value, cStr), x, y, blinking);
}

void Display::printUInt(unsigned int value, unsigned int x, unsigned int y, char* cStr, bool blinking)
{
	print(toString(value, cStr), x, y, blinking);
}

void Display::printMemoryDump(unsigned int address, unsigned int count, unsigned int x, unsigned int y)
{
	char cStr[12];
	for (unsigned i = 0; i < count / 16; i++)
		for (unsigned j = 0; j < 16; j++)
			print(toString(*reinterpret_cast<unsigned char*>(address++), cStr), x + j * 4, y + i);

	for (unsigned i = 0; i < count % 16; i++)
		print(toString(*reinterpret_cast<unsigned char*>(address++), cStr), x + i * 4, y + count / 16);
}

void Display::printCharTable(unsigned int x, unsigned int y)
{
	char sym[2] = {0, 0};
	for (unsigned char i = 0; i < 8; i++)
		for (unsigned char j = 0; j < 32; j++)
		{
			sym[0] = i * 32 + j;
			print(sym, x + j * 1, y + i);
		}
}


