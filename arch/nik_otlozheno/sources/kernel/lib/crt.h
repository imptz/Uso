#pragma once

#define EXTERN_C			extern "C"
#define BEGIN_EXTERN_C		extern "C" {
#define END_EXTERN_C		}

BEGIN_EXTERN_C
	
typedef void (__cdecl *_PVFV)();
extern _PVFV __xc_a[], __xc_z[];  

void _init(_PVFV *pfbegin, _PVFV *pfend);

END_EXTERN_C

