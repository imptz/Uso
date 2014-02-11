#include "interrupt.h"
#include "../Local.h"

unsigned int intVar;

_declspec(naked) void defaultProcessorInterruptHandler()
{
	Display::getSingleton().getSingleton().print(LOCAL_INTERRUPT_DEFAULT_PROCESSOR_INTERRUPT_HANDLER, 0, 0);
	for (;;);
	//_asm iretd
}

_declspec(naked) void defaultProcessorInterruptHandler0()
{
	Display::getSingleton().print(LOCAL_INTERRUPT_DEFAULT_PROCESSOR_INTERRUPT_HANDLER0, 0, 0);
	for (;;);
	//_asm iretd
}

_declspec(naked) void defaultProcessorInterruptHandler1()
{
	Display::getSingleton().print(LOCAL_INTERRUPT_DEFAULT_PROCESSOR_INTERRUPT_HANDLER1, 0, 0);
	__asm	pop ecx
	__asm	mov intVar,ecx
	Display::getSingleton().printUInt(intVar,0,1);
	__asm	pop ecx
	__asm	mov intVar,ecx
	Display::getSingleton().printUInt(intVar,10,1);
	__asm	pop ecx
	__asm	mov intVar,ecx
	Display::getSingleton().printUInt(intVar,20,1);
	__asm	mov eax, dr6
	__asm	mov intVar,eax
	Display::getSingleton().printUInt(intVar,30,1);
	__asm	mov eax, dr7
	__asm	mov intVar,eax
	Display::getSingleton().printUInt(intVar,40,1);
	for (;;);
	//_asm iretd
}

_declspec(naked) void defaultProcessorInterruptHandler2()
{
	Display::getSingleton().print(LOCAL_INTERRUPT_DEFAULT_PROCESSOR_INTERRUPT_HANDLER2, 0, 0);
	for (;;);
	//_asm iretd
}

_declspec(naked) void defaultProcessorInterruptHandler3()
{
	Display::getSingleton().print(LOCAL_INTERRUPT_DEFAULT_PROCESSOR_INTERRUPT_HANDLER3, 0, 0);
	for (;;);
	//_asm iretd
}

_declspec(naked) void defaultProcessorInterruptHandler4()
{
	Display::getSingleton().print(LOCAL_INTERRUPT_DEFAULT_PROCESSOR_INTERRUPT_HANDLER4, 0, 0);
	for (;;);
	//_asm iretd
}

_declspec(naked) void defaultProcessorInterruptHandler5()
{
	Display::getSingleton().print(LOCAL_INTERRUPT_DEFAULT_PROCESSOR_INTERRUPT_HANDLER5, 0, 0);
	for (;;);
	//_asm iretd
}

_declspec(naked) void defaultProcessorInterruptHandler6()
{
	Display::getSingleton().print(LOCAL_INTERRUPT_DEFAULT_PROCESSOR_INTERRUPT_HANDLER6, 0, 0);
	__asm	pop ecx
	__asm	mov intVar,ecx
	Display::getSingleton().printUInt(intVar,0,1);
	__asm	pop ecx
	__asm	mov intVar,ecx
	Display::getSingleton().printUInt(intVar,10,1);
	__asm	pop ecx
	__asm	mov intVar,ecx
	Display::getSingleton().printUInt(intVar,20,1);
	__asm	mov eax, dr6
	__asm	mov intVar,eax
	Display::getSingleton().printUInt(intVar,30,1);
	__asm	mov eax, dr7
	__asm	mov intVar,eax
	Display::getSingleton().printUInt(intVar,40,1);
	for (;;);
	//_asm iretd
}

_declspec(naked) void defaultProcessorInterruptHandler7()
{
	Display::getSingleton().print(LOCAL_INTERRUPT_DEFAULT_PROCESSOR_INTERRUPT_HANDLER7, 0, 0);
	for (;;);
	//_asm iretd
}

_declspec(naked) void defaultProcessorInterruptHandler8()
{
	Display::getSingleton().print(LOCAL_INTERRUPT_DEFAULT_PROCESSOR_INTERRUPT_HANDLER8, 0, 0);
	for (;;);
	//_asm iretd
}

_declspec(naked) void defaultProcessorInterruptHandler9()
{
	Display::getSingleton().print(LOCAL_INTERRUPT_DEFAULT_PROCESSOR_INTERRUPT_HANDLER9, 0, 0);
	for (;;);
	//_asm iretd
}

_declspec(naked) void defaultProcessorInterruptHandler10()
{
	Display::getSingleton().print(LOCAL_INTERRUPT_DEFAULT_PROCESSOR_INTERRUPT_HANDLER10, 0, 0);
	__asm	pop ecx
	__asm	mov intVar,ecx
	Display::getSingleton().printUInt(intVar,0,1);
	__asm	pop ecx
	__asm	mov intVar,ecx
	Display::getSingleton().printUInt(intVar,10,1);
	__asm	pop ecx
	__asm	mov intVar,ecx
	Display::getSingleton().printUInt(intVar,20,1);
	for (;;);
	//_asm iretd
}

_declspec(naked) void defaultProcessorInterruptHandler11()
{
	Display::getSingleton().print(LOCAL_INTERRUPT_DEFAULT_PROCESSOR_INTERRUPT_HANDLER11, 0, 0);
	for (;;);
	//_asm iretd
}

_declspec(naked) void defaultProcessorInterruptHandler12()
{
	Display::getSingleton().print(LOCAL_INTERRUPT_DEFAULT_PROCESSOR_INTERRUPT_HANDLER12, 0, 0);
	for (;;);
	//_asm iretd
}

unsigned int regis;

_declspec(naked) void defaultProcessorInterruptHandler13()
{
	_asm push eax
	_asm push ebx
	_asm push ecx
	_asm push edx
	_asm push edi
	_asm push esi

	_asm pop regis
	Display::getSingleton().printUInt(regis,0,2);
	_asm pop regis
	Display::getSingleton().printUInt(regis,0,3);
	_asm pop regis
	Display::getSingleton().printUInt(regis,0,4);
	_asm pop regis
	Display::getSingleton().printUInt(regis,0,5);
	_asm pop regis
	Display::getSingleton().printUInt(regis,0,6);
	_asm pop regis
	Display::getSingleton().printUInt(regis,0,7);

	Display::getSingleton().print(LOCAL_INTERRUPT_DEFAULT_PROCESSOR_INTERRUPT_HANDLER13, 0, 0);
	__asm	pop ecx
	__asm	mov intVar,ecx
	Display::getSingleton().printUInt(intVar,0,1);
	__asm	pop ecx
	__asm	mov intVar,ecx
	Display::getSingleton().printUInt(intVar,10,1);
	__asm	pop ecx
	__asm	mov intVar,ecx
	Display::getSingleton().printUInt(intVar,20,1);
	__asm	pop ecx
	__asm	mov intVar,ecx
	Display::getSingleton().printUInt(intVar,30,1);
	_asm mov intVar,esp
	Display::getSingleton().printUInt(intVar,50,22);
	for (;;);
	//_asm iretd
}

_declspec(naked) void defaultProcessorInterruptHandler14()
{
	Display::getSingleton().print(LOCAL_INTERRUPT_DEFAULT_PROCESSOR_INTERRUPT_HANDLER, 0, 0);
	for (;;);
	//_asm iretd
}

_declspec(naked) void defaultProcessorInterruptHandler15()
{
	Display::getSingleton().print(LOCAL_INTERRUPT_DEFAULT_PROCESSOR_INTERRUPT_HANDLER15, 0, 0);
	for (;;);
	//_asm iretd
}

_declspec(naked) void defaultProcessorInterruptHandler16()
{
	Display::getSingleton().print(LOCAL_INTERRUPT_DEFAULT_PROCESSOR_INTERRUPT_HANDLER16, 0, 0);
	for (;;);
	//_asm iretd
}

_declspec(naked) void defaultProcessorInterruptHandler17()
{
	Display::getSingleton().print(LOCAL_INTERRUPT_DEFAULT_PROCESSOR_INTERRUPT_HANDLER17, 0, 0);
	for (;;);
	//_asm iretd
}

_declspec(naked) void defaultProcessorInterruptHandler18()
{
	Display::getSingleton().print(LOCAL_INTERRUPT_DEFAULT_PROCESSOR_INTERRUPT_HANDLER18, 0, 0);
	for (;;);
	//_asm iretd
}

_declspec(naked) void defaultProcessorInterruptHandler19()
{
	Display::getSingleton().print(LOCAL_INTERRUPT_DEFAULT_PROCESSOR_INTERRUPT_HANDLER19, 0, 0);
	for (;;);
	//_asm iretd
}

_declspec(naked) void defaultProcessorInterruptHandler20()
{
	Display::getSingleton().print(LOCAL_INTERRUPT_DEFAULT_PROCESSOR_INTERRUPT_HANDLER20, 0, 0);
	for (;;);
	//_asm iretd
}

_declspec(naked) void defaultProcessorInterruptHandler21()
{
	Display::getSingleton().print(LOCAL_INTERRUPT_DEFAULT_PROCESSOR_INTERRUPT_HANDLER21, 0, 0);
	for (;;);
	//_asm iretd
}

_declspec(naked) void defaultProcessorInterruptHandler22()
{
	Display::getSingleton().print(LOCAL_INTERRUPT_DEFAULT_PROCESSOR_INTERRUPT_HANDLER22, 0, 0);
	for (;;);
	//_asm iretd
}

_declspec(naked) void defaultProcessorInterruptHandler23()
{
	Display::getSingleton().print(LOCAL_INTERRUPT_DEFAULT_PROCESSOR_INTERRUPT_HANDLER23, 0, 0);
	for (;;);
	//_asm iretd
}

_declspec(naked) void defaultProcessorInterruptHandler24()
{
	Display::getSingleton().print(LOCAL_INTERRUPT_DEFAULT_PROCESSOR_INTERRUPT_HANDLER24, 0, 0);
	for (;;);
	//_asm iretd
}

_declspec(naked) void defaultProcessorInterruptHandler25()
{
	Display::getSingleton().print(LOCAL_INTERRUPT_DEFAULT_PROCESSOR_INTERRUPT_HANDLER25, 0, 0);
	for (;;);
	//_asm iretd
}

_declspec(naked) void defaultProcessorInterruptHandler26()
{
	Display::getSingleton().print(LOCAL_INTERRUPT_DEFAULT_PROCESSOR_INTERRUPT_HANDLER26, 0, 0);
	for (;;);
	//_asm iretd
}

_declspec(naked) void defaultProcessorInterruptHandler27()
{
	Display::getSingleton().print(LOCAL_INTERRUPT_DEFAULT_PROCESSOR_INTERRUPT_HANDLER27, 0, 0);
	for (;;);
	//_asm iretd
}

_declspec(naked) void defaultProcessorInterruptHandler28()
{
	Display::getSingleton().print(LOCAL_INTERRUPT_DEFAULT_PROCESSOR_INTERRUPT_HANDLER28, 0, 0);
	for (;;);
	//_asm iretd
}

_declspec(naked) void defaultProcessorInterruptHandler29()
{
	Display::getSingleton().print(LOCAL_INTERRUPT_DEFAULT_PROCESSOR_INTERRUPT_HANDLER29, 0, 0);
	for (;;);
	//_asm iretd
}

_declspec(naked) void defaultProcessorInterruptHandler30()
{
	Display::getSingleton().print(LOCAL_INTERRUPT_DEFAULT_PROCESSOR_INTERRUPT_HANDLER30, 0, 0);
	for (;;);
	//_asm iretd
}

_declspec(naked) void defaultProcessorInterruptHandler31()
{
	Display::getSingleton().print(LOCAL_INTERRUPT_DEFAULT_PROCESSOR_INTERRUPT_HANDLER31, 0, 0);
	for (;;);
	//_asm iretd
}

//_declspec(naked) void defaultInterruptHandlerIRQ7()
//{
	//_asm pushfd
	//_asm pushad

	//Display::getSingleton().print("defaultInterruptHandlerIRQ7", 30, 5);

	//_asm popad
	//_asm popfd

//	_asm iretd
//}

_declspec(naked) void defaultHardware0InterruptHandler()
{
	_asm pushfd
	_asm push	eax
	_asm mov	al,0x20
	_asm out	0x20,al
	_asm pop	eax
	_asm popfd
	_asm iretd
}

_declspec(naked) void defaultHardware1InterruptHandler()
{
	_asm pushfd
	_asm push	eax
	_asm mov	al,0x20
	_asm out	0xa0,al
	_asm pop	eax
	_asm popfd
	_asm iretd
}

_declspec(naked) void disablingNmi()
{
	_asm pushfd
	_asm push eax
	_asm mov al,0x80
	_asm out 0x70,al
	_asm pop eax
	_asm popfd
	_asm ret
}

_declspec(naked) void enablingNmi()
{
	_asm pushfd
	_asm push eax
	_asm in al,0x70
	_asm and al,0x7f
	_asm out 0x70,al
	_asm pop eax
	_asm popfd
	_asm ret
}

unsigned int interrupts_funcOffset;
