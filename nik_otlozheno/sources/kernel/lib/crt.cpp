#include "crt.h"

#pragma section(".CRT$XCA", read, write)
#pragma data_seg(".CRT$XCA")
_PVFV __xc_a[] = {0};

#pragma section(".CRT$XCZ", read, write)
#pragma data_seg(".CRT$XCZ")
_PVFV __xc_z[] = {0};

#pragma data_seg()
#pragma comment(linker, "/merge:.CRT=.rdata")

BEGIN_EXTERN_C

void _init(_PVFV *pfbegin, _PVFV *pfend){
	while (pfbegin < pfend)
	{
		if (*pfbegin != 0)
			(**pfbegin)();
		++pfbegin;
	}
}

#pragma comment(linker, "/nodefaultlib:libc.lib")
#pragma comment(linker, "/nodefaultlib:libcmt.lib")
#pragma comment(linker, "/nodefaultlib:libcmtd.lib")

EXTERN_C int _main();

extern "C" unsigned int _fltused = 0;

void mainCRTStartup(){
	_init(__xc_a, __xc_z);
    _main();
	static const char *pStr = "mainCRTStartup exit";
	_asm
	{
		mov     esi,pStr
		mov	    edi,0xb8000 + 480
		mov     eax,0x00000200
		cld
	_printString_13:
		lodsb
		cmp     al,0
		je      _printString_23
		stosw
		jmp		_printString_13
	_printString_23:
		jmp		_printString_23
	}
}

END_EXTERN_C