/*! \file low.h
    \brief Содержит различные низкоуровневые функции и макросы.    
*/
#include "low.h"

/**
* Пишет один байт в порт ввода-вывода.
* @param port номер порта.
* @param value байт для записи в порт.
*/
void outPort(unsigned short port, unsigned char value)
{
	_asm pushfd
	_asm push eax
	_asm push edx
	_asm mov dx, port
	_asm mov al, value
	_asm out dx, al
	_asm pop edx
	_asm pop eax
	_asm popfd
}

/**
* Читает один байт из порта ввода-вывода.
* @param port номер порта.
* @return прочитанный байт
*/
unsigned char inPort(unsigned short port)
{
	unsigned char value;

	_asm pushfd
	_asm push eax
	_asm push edx
	_asm mov dx, port
	_asm in al, dx
	_asm mov value,al
	_asm pop edx
	_asm pop eax
	_asm popfd

	return value;
}

/**
* Функция-заглушка.
*/
extern "C" int __cdecl _purecall(void)
{
    return 0;
}

/**
* Функция-заглушка.
*/
extern "C" _declspec(naked) void _chkstk()
{
	_asm ret 0
}


