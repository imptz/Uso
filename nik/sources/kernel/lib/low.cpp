#include "low.h"

void outPort(unsigned short port, unsigned char value){
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

unsigned char inPort(unsigned short port){
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

extern "C" int __cdecl _purecall(void){
    return 0;
}

extern "C" _declspec(naked) void _chkstk(){
	_asm ret 0
}


