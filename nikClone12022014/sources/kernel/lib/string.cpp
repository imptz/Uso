/*! \file string.cpp
    \brief Содержит функции работы со строками с нулевым символом на конце.
    
*/
#include "string.h"
#include "memory\memory.h"

/**
* Вычисляет длину строки.
* @param str указатель на строку.
* @return длину строки
*/
int strlen(char* str)
{
	for (unsigned int i = 0; i < 256; i++)
		if (str[i] == 0)
			return i;

	return -1;
}

/**
* Копирует строку.
* @param dest указатель на строку приемник.
* @param src указатель на строку источник.
*/
void strcpy(char* dest, char* src)
{
	int length = strlen(src);
	memcpy(dest, src, length);
	dest[length] = 0;
}
