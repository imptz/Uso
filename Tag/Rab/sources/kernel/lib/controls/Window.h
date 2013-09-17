#ifndef WINDOW_H 
#define WINDOW_H

#include "../display/display.h"

const char SYMBOL_INVISIBLE_LEFT_TOP		= ' ';
const char SYMBOL_INVISIBLE_RIGHT_TOP		= ' ';
const char SYMBOL_INVISIBLE_LEFT_BOTTOM		= ' ';
const char SYMBOL_INVISIBLE_RIGHT_BOTTOM	= ' ';
const char SYMBOL_INVISIBLE_VERTICAL		= ' ';
const char SYMBOL_INVISIBLE_HORIZONTAL		= ' ';

#pragma warning (disable : 4309)
const char SYMBOL_SINGLE_LEFT_TOP			= 0xda;
const char SYMBOL_SINGLE_RIGHT_TOP			= 0xbf;
const char SYMBOL_SINGLE_LEFT_BOTTOM		= 0xc0;
const char SYMBOL_SINGLE_RIGHT_BOTTOM		= 0xd9;
const char SYMBOL_SINGLE_VERTICAL			= 0xb3;
const char SYMBOL_SINGLE_HORIZONTAL			= 0xc4;

const char SYMBOL_DOUBLE_LEFT_TOP			= 0xc9;
const char SYMBOL_DOUBLE_RIGHT_TOP			= 0xbb;
const char SYMBOL_DOUBLE_LEFT_BOTTOM		= 0xc8;
const char SYMBOL_DOUBLE_RIGHT_BOTTOM		= 0xbc;
const char SYMBOL_DOUBLE_VERTICAL			= 0xba;
const char SYMBOL_DOUBLE_HORIZONTAL			= 0xcd;
#pragma warning (default : 4309)

class Window
{
	public:
		enum BORDER_STYLE
		{
			BORDER_STYLE_INVISIBLE = 1,
			BORDER_STYLE_SINGLE = 2,
			BORDER_STYLE_DOUBLE = 3
		};

	private:
		static inline void drawWindowBorderStyleInvisible(unsigned int positionX, unsigned int positionY, unsigned int width, unsigned int height, bool blinking)
		{
			//Display::getSingleton().printChar(positionX, positionY, SYMBOL_INVISIBLE_LEFT_TOP, _activeVideoPage);
			//Display::getSingleton().printChar(positionX + width - 1, positionY, SYMBOL_INVISIBLE_RIGHT_TOP, _activeVideoPage);
			//Display::getSingleton().printChar(positionX, positionY + height - 1, SYMBOL_INVISIBLE_LEFT_BOTTOM, _activeVideoPage);
			//Display::getSingleton().printChar(positionX + width - 1, positionY + height - 1, SYMBOL_INVISIBLE_RIGHT_BOTTOM, _activeVideoPage);

			//for (int i = 1; i < width - 1; i++)
			//{
			//	Display::getSingleton().printChar(positionX + i, positionY, SYMBOL_INVISIBLE_HORIZONTAL, _activeVideoPage);
			//	Display::getSingleton().printChar(positionX + i, positionY + height - 1, SYMBOL_INVISIBLE_HORIZONTAL, _activeVideoPage);
			//}

			//for (int i = 1; i < height - 1; i++)
			//{
			//	Display::getSingleton().printChar(positionX, positionY + i, SYMBOL_INVISIBLE_VERTICAL, _activeVideoPage);
			//	Display::getSingleton().printChar(positionX + width - 1, positionY + i, SYMBOL_INVISIBLE_VERTICAL, _activeVideoPage);
			//}
		}

		static inline void drawWindowBorderStyleSingle(unsigned int positionX, unsigned int positionY, unsigned int width, unsigned int height, Display::COLOR color, bool blinking)
		{
			Display::getSingleton().printChar(SYMBOL_SINGLE_LEFT_TOP, positionX, positionY, color, blinking);
			Display::getSingleton().printChar(SYMBOL_SINGLE_RIGHT_TOP, positionX + width - 1, positionY, color, blinking);
			Display::getSingleton().printChar(SYMBOL_SINGLE_LEFT_BOTTOM, positionX, positionY + height - 1, color, blinking);
			Display::getSingleton().printChar(SYMBOL_SINGLE_RIGHT_BOTTOM, positionX + width - 1, positionY + height - 1, color, blinking);

			for (unsigned int i = 1; i < width - 1; i++)
			{
				Display::getSingleton().printChar(SYMBOL_SINGLE_HORIZONTAL, positionX + i, positionY, color, blinking);
				Display::getSingleton().printChar(SYMBOL_SINGLE_HORIZONTAL, positionX + i, positionY + height - 1, color, blinking);
			}

			for (unsigned int i = 1; i < height - 1; i++)
			{
				Display::getSingleton().printChar(SYMBOL_SINGLE_VERTICAL, positionX, positionY + i, color, blinking);
				Display::getSingleton().printChar(SYMBOL_SINGLE_VERTICAL, positionX + width - 1, positionY + i, color, blinking);
			}
		}

		static inline void drawWindowBorderStyleDouble(unsigned int positionX, unsigned int positionY, unsigned int width, unsigned int height, Display::COLOR color, bool blinking)
		{
			Display::getSingleton().printChar(SYMBOL_DOUBLE_LEFT_TOP, positionX, positionY, color, blinking);
			Display::getSingleton().printChar(SYMBOL_DOUBLE_RIGHT_TOP, positionX + width - 1, positionY, color, blinking);
			Display::getSingleton().printChar(SYMBOL_DOUBLE_LEFT_BOTTOM, positionX, positionY + height - 1, color, blinking);
			Display::getSingleton().printChar(SYMBOL_DOUBLE_RIGHT_BOTTOM, positionX + width - 1, positionY + height - 1, color, blinking);

			for (unsigned int i = 1; i < width - 1; i++)
			{
				Display::getSingleton().printChar(SYMBOL_DOUBLE_HORIZONTAL, positionX + i, positionY, color, blinking);
				Display::getSingleton().printChar(SYMBOL_DOUBLE_HORIZONTAL, positionX + i, positionY + height - 1, color, blinking);
			}

			for (unsigned int i = 1; i < height - 1; i++)
			{
				Display::getSingleton().printChar(SYMBOL_DOUBLE_VERTICAL, positionX, positionY + i, color, blinking);
				Display::getSingleton().printChar(SYMBOL_DOUBLE_VERTICAL, positionX + width - 1, positionY + i, color, blinking);
			}
		}

	public:
		static void drawWindow(unsigned int positionX, unsigned int positionY, unsigned int width, unsigned int height, BORDER_STYLE borderStyle, Display::COLOR color, bool blinking)
		{
			switch (borderStyle)
			{
				case BORDER_STYLE_INVISIBLE:
					drawWindowBorderStyleInvisible(positionX, positionY, width, height, blinking);
					break;
				case BORDER_STYLE_SINGLE:
					drawWindowBorderStyleSingle(positionX, positionY, width, height, color, blinking);
					break;
				case BORDER_STYLE_DOUBLE:
					drawWindowBorderStyleDouble(positionX, positionY, width, height, color, blinking);
					break;
			}
		}
};

#endif