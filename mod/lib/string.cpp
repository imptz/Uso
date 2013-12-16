/*! \file string.cpp
    \brief �������� ������� ������ �� �������� � ������� �������� �� �����.
    
*/
#include "string.h"
#include "memory\memory.h"

/**
* ��������� ����� ������.
* @param str ��������� �� ������.
* @return ����� ������
*/
int strlen(char* str)
{
	for (unsigned int i = 0; i < 256; i++)
		if (str[i] == 0)
			return i;

	return -1;
}

/**
* �������� ������.
* @param dest ��������� �� ������ ��������.
* @param src ��������� �� ������ ��������.
*/
void strcpy(char* dest, char* src)
{
	int length = strlen(src);
	memcpy(dest, src, length);
	dest[length] = 0;
}
