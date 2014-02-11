#ifndef DISPLAY_H
#define DISPLAY_H

#include "../memory/memory.h"
#include "../low.h"
#include "../Singleton.h"
#include "fonts.h"

class Display : public Singleton<Display>
{
	public:
		enum COLOR
		{
			COLOR_BLACK = 0,
			COLOR_BLUE = 1,
			COLOR_GREEN = 2,
			COLOR_TURQUOISE = 3,
			COLOR_RED = 4,
			COLOR_VIOLET = 5,
			COLOR_BROWN = 6,
			COLOR_LIGHT_GRAY = 7,
			COLOR_GRAY = 8,
			COLOR_LIGHT_BLUE = 9,
			COLOR_LIGHT_GREEN = 10,
			COLOR_LIGHT_TURQUOISE = 11,
			COLOR_PINK = 12,
			COLOR_LIGHT_VIOLET = 13,
			COLOR_YELLOW = 14,
			COLOR_WHITE = 15
		};

	private:
		static const unsigned int BACKGROUND_COLOR = COLOR_BLACK;
		static const unsigned int CHAR_COLOR = COLOR_WHITE;//COLOR_GRAY;

		static const unsigned int VIDEO_BASE_ADDRESS = 0x000b8000;
		static const unsigned int FONT_AREA_ADDRESS = 0x000a0000;

		enum REGISTERS
		{
			REGISTERS_VGA_SEQUENCER_INDEX = 0x3c4,
			REGISTERS_VGA_SEQUENCER_DATA = 0x3c5,
			REGISTERS_VGA_GRAPHICS_CONTROLLER_INDEX = 0x3ce,
			REGISTERS_VGA_GRAPHICS_CONTROLLER_DATA = 0x3cf,
			REGISTERS_VGA_CRTC_INDEX = 0x3d4,
			REGISTERS_VGA_CRTC_DATA = 0x3d5
		};

		void modifyCharTable();
		void modifyCharTable1();
		void disableCursor();
		
	public:
		Display();
		void clearScreen();
		char toChar(unsigned char value);
		char* toString2Num(unsigned char value, char* cStr);
		char* toString2Num3(unsigned char value, char* cStr);
		char* toString2NumHex(unsigned char value, char* cStr);
		char* toString3Num(unsigned char value, char* cStr);
		char* toString4Num(int value, char* cStr);
		char* toString(unsigned char value, char* cStr);
		char* toString(unsigned int value, char* cStr);
		char* toStringDecimal(unsigned int value, char* cStr);
		char* toStringDecimalSigned(int value, char* cStr);
		char* toStringFloat(float value, char* cStr);
		char* toStringWithoutNull(unsigned int value, char* cStr);
		char* toStringTime(unsigned int value, char* cStr);
		char* toStringDate(unsigned int value, char* cStr);
		void print(const char* pStr, int x, int y, bool blinking = false);
		void printChar(unsigned char _char, unsigned int x, unsigned int y, COLOR color, bool blinking = false);
		void printUInt(unsigned int value, unsigned int x, unsigned int y, bool blinking = false);
		void printUInt(unsigned int value, unsigned int x, unsigned int y, char* cStr, bool blinking = false);
		void printMemoryDump(unsigned int address, unsigned int count, unsigned int x, unsigned int y);
		void printCharTable(unsigned int x, unsigned int y);
};

#endif