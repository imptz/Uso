/*! \file main.cpp
    \brief Содержит функцию main.
    
     В main создается объект Root и производится старт приложения (Application::start).
*/
#include "../lib/Root.h"

extern "C" void _main()
{
	_asm mov eax,16
	_asm mov fs, ax
	_asm mov gs, ax

	Root root;
	Application::getSingleton().start();
}
