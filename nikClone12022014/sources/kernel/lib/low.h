#pragma once

extern void outPort(unsigned short port, unsigned char value);
extern unsigned char inPort(unsigned short port);

/**
* Во встроенном ассемблере Visual c++ нет mov eax, cr4.
*/
#define MOV_EAX_CR4 \
    __asm _emit 0x0F \
    __asm _emit 0x20 \
    __asm _emit 0xE0

/**
* Во встроенном ассемблере Visual c++ нет mov cr4, eax.
*/
#define MOV_CR4_EAX \
    __asm _emit 0x0F \
    __asm _emit 0x22 \
    __asm _emit 0xE0

extern "C" int __cdecl _purecall(void);
extern "C" void _chkstk();
