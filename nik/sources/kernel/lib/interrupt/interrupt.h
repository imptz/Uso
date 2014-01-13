#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "../memory/memory.h"
#include "../display/display.h"

extern void defaultProcessorInterruptHandler();

extern void defaultProcessorInterruptHandler0();
extern void defaultProcessorInterruptHandler1();
extern void defaultProcessorInterruptHandler2();
extern void defaultProcessorInterruptHandler3();
extern void defaultProcessorInterruptHandler4();
extern void defaultProcessorInterruptHandler5();
extern void defaultProcessorInterruptHandler6();
extern void defaultProcessorInterruptHandler7();
extern void defaultProcessorInterruptHandler8();
extern void defaultProcessorInterruptHandler9();
extern void defaultProcessorInterruptHandler10();
extern void defaultProcessorInterruptHandler11();
extern void defaultProcessorInterruptHandler12();
extern void defaultProcessorInterruptHandler13();
extern void defaultProcessorInterruptHandler14();
extern void defaultProcessorInterruptHandler15();
extern void defaultProcessorInterruptHandler16();
extern void defaultProcessorInterruptHandler17();
extern void defaultProcessorInterruptHandler18();
extern void defaultProcessorInterruptHandler19();
extern void defaultProcessorInterruptHandler20();
extern void defaultProcessorInterruptHandler21();
extern void defaultProcessorInterruptHandler22();
extern void defaultProcessorInterruptHandler23();
extern void defaultProcessorInterruptHandler24();
extern void defaultProcessorInterruptHandler25();
extern void defaultProcessorInterruptHandler26();
extern void defaultProcessorInterruptHandler27();
extern void defaultProcessorInterruptHandler28();
extern void defaultProcessorInterruptHandler29();
extern void defaultProcessorInterruptHandler30();
extern void defaultProcessorInterruptHandler31();
//extern void defaultInterruptHandlerIRQ7();


extern void defaultHardware0InterruptHandler();
extern void defaultHardware1InterruptHandler();
extern void disablingNmi();
extern void enablingNmi();

extern unsigned int interrupts_funcOffset;

#define SET_INT_HANDLER(num, FUNC) \
_asm \
{ \
	_asm lea eax, FUNC \
	_asm mov interrupts_funcOffset, eax \
} \
Interrupts::setIdtItem(num, interrupts_funcOffset);

class Interrupts{
private:
	static const unsigned int IDT_ADDRESS = 0x00002000;
	static const unsigned int IDT_REG_ADDRESS = 0x00003000;
	static const unsigned int IDT_DESCRIPTOR_SIZE = 8;
	static const unsigned int IDT_SIZE = 256 * IDT_DESCRIPTOR_SIZE;

	static void createIdt(){
		memset(reinterpret_cast<unsigned char*>(IDT_ADDRESS), static_cast<unsigned char>(0), IDT_SIZE);

		*(reinterpret_cast<unsigned short*>(IDT_REG_ADDRESS)) = IDT_SIZE - 1;
		*(reinterpret_cast<unsigned int*>(IDT_REG_ADDRESS + 2)) = IDT_ADDRESS;
		_asm{
			mov eax, IDT_REG_ADDRESS
			lidt fword ptr [eax]
		}	
	}

public:
	/*
	/	этот метод НЕ ДОЛЖЕН вызываться напрямую, т.к. offset вычисляется относительный. Следует использовать макрос SET_INT_HANDLER
	*/
	static void setIdtItem(int index, unsigned int offset){
		const unsigned int IDT_DESCRIPTOR_FLAGS = 0x00008e00;
		const unsigned int IDT_SEGMENT_SELECTOR = 0x00080000;

		unsigned int lowDesc = (offset & 0x0000ffff) | IDT_SEGMENT_SELECTOR;
		unsigned int highDesc = IDT_DESCRIPTOR_FLAGS | (offset & 0xffff0000);

		*(reinterpret_cast<unsigned int*>(IDT_ADDRESS + IDT_DESCRIPTOR_SIZE * index)) = lowDesc;
		*(reinterpret_cast<unsigned int*>(IDT_ADDRESS + IDT_DESCRIPTOR_SIZE * index + 4)) = highDesc;
	}

private:
	static void relocationHardwareInterrupts(){
		_asm{
			mov    al, 0x11
			out    0x20, al
			out    0xA0, al        

			mov    al, 0x20
			out    0x21, al
			mov    al, 0x28
			out    0xA1, al

			mov    al, 0x04
			out    0x21, al
			mov    al, 0x02
			out    0xA1, al

			mov    al, 0x11 
			out    0x21, al 
			mov    al,0x01  
			out    0xA1, al

			mov    al, 0   
			mov    dx, 0x04D0
			out    dx, al
			inc    dx
			out    dx, al
		
			mov    al, 0x6B 
			out    0x20, al 
			out    0xA0, al
		}
	}

public:
	static void initInterrupts(){
		createIdt();
	
		SET_INT_HANDLER(0, defaultProcessorInterruptHandler0)
		SET_INT_HANDLER(1, defaultProcessorInterruptHandler1)
		SET_INT_HANDLER(2, defaultProcessorInterruptHandler2)
		SET_INT_HANDLER(3, defaultProcessorInterruptHandler3)
		SET_INT_HANDLER(4, defaultProcessorInterruptHandler4)
		SET_INT_HANDLER(5, defaultProcessorInterruptHandler5)
		SET_INT_HANDLER(6, defaultProcessorInterruptHandler6)
		SET_INT_HANDLER(7, defaultProcessorInterruptHandler7)
		SET_INT_HANDLER(8, defaultProcessorInterruptHandler8)
		SET_INT_HANDLER(9, defaultProcessorInterruptHandler9)
		SET_INT_HANDLER(10, defaultProcessorInterruptHandler10)
		SET_INT_HANDLER(11, defaultProcessorInterruptHandler11)
		SET_INT_HANDLER(12, defaultProcessorInterruptHandler12)
		SET_INT_HANDLER(13, defaultProcessorInterruptHandler13)
		SET_INT_HANDLER(14, defaultProcessorInterruptHandler14)
		SET_INT_HANDLER(15, defaultProcessorInterruptHandler15)
		SET_INT_HANDLER(16, defaultProcessorInterruptHandler16)
		SET_INT_HANDLER(17, defaultProcessorInterruptHandler17)
		SET_INT_HANDLER(18, defaultProcessorInterruptHandler18)
		SET_INT_HANDLER(19, defaultProcessorInterruptHandler19)
		SET_INT_HANDLER(20, defaultProcessorInterruptHandler20)
		SET_INT_HANDLER(21, defaultProcessorInterruptHandler21)
		SET_INT_HANDLER(22, defaultProcessorInterruptHandler22)
		SET_INT_HANDLER(23, defaultProcessorInterruptHandler23)
		SET_INT_HANDLER(24, defaultProcessorInterruptHandler24)
		SET_INT_HANDLER(25, defaultProcessorInterruptHandler25)
		SET_INT_HANDLER(26, defaultProcessorInterruptHandler26)
		SET_INT_HANDLER(27, defaultProcessorInterruptHandler27)
		SET_INT_HANDLER(28, defaultProcessorInterruptHandler28)
		SET_INT_HANDLER(29, defaultProcessorInterruptHandler29)
		SET_INT_HANDLER(30, defaultProcessorInterruptHandler30)
		SET_INT_HANDLER(31, defaultProcessorInterruptHandler31)

		relocationHardwareInterrupts();
		disablingNmi();
		for (int i = 0; i < 16; i++)
			Interrupts::disablingHardwareInterrupt(i);

		SET_INT_HANDLER(HARDWARE_VECTOR_OFFSET + 0, defaultHardware0InterruptHandler);
		SET_INT_HANDLER(HARDWARE_VECTOR_OFFSET + 1, defaultHardware0InterruptHandler);
		SET_INT_HANDLER(HARDWARE_VECTOR_OFFSET + 2, defaultHardware0InterruptHandler);
		SET_INT_HANDLER(HARDWARE_VECTOR_OFFSET + 3, defaultHardware0InterruptHandler);
		SET_INT_HANDLER(HARDWARE_VECTOR_OFFSET + 4, defaultHardware0InterruptHandler);
		SET_INT_HANDLER(HARDWARE_VECTOR_OFFSET + 5, defaultHardware0InterruptHandler);
		SET_INT_HANDLER(HARDWARE_VECTOR_OFFSET + 6, defaultHardware0InterruptHandler);
		SET_INT_HANDLER(HARDWARE_VECTOR_OFFSET + 7, defaultHardware0InterruptHandler);

		SET_INT_HANDLER(HARDWARE_VECTOR_OFFSET + 8, defaultHardware1InterruptHandler);
		SET_INT_HANDLER(HARDWARE_VECTOR_OFFSET + 9, defaultHardware1InterruptHandler);
		SET_INT_HANDLER(HARDWARE_VECTOR_OFFSET + 10, defaultHardware1InterruptHandler);
		SET_INT_HANDLER(HARDWARE_VECTOR_OFFSET + 11, defaultHardware1InterruptHandler);
		SET_INT_HANDLER(HARDWARE_VECTOR_OFFSET + 12, defaultHardware1InterruptHandler);
		SET_INT_HANDLER(HARDWARE_VECTOR_OFFSET + 13, defaultHardware1InterruptHandler);
		SET_INT_HANDLER(HARDWARE_VECTOR_OFFSET + 14, defaultHardware1InterruptHandler);
		SET_INT_HANDLER(HARDWARE_VECTOR_OFFSET + 15, defaultHardware1InterruptHandler);
	}

	static const unsigned int HARDWARE_VECTOR_OFFSET = 32;

	static void enablingHardwareInterrupt(int intNumber){
		unsigned char num;
		if (intNumber < 8){
			num = ~(1 << intNumber);
			_asm in		al,0x21
			_asm and	al,num
			_asm out    0x21, al 
		}else{
			num = ~(1 << (intNumber - 8));
			_asm in		al,0xa1
			_asm and	al,num
			_asm out    0xa1, al 
		}
	}

	static void disablingHardwareInterrupt(int intNumber){
		unsigned char num;
		if (intNumber < 8){
			num = 1 << intNumber;
			_asm in		al,0x21
			_asm or		al,num
			_asm out    0x21, al 
		}else{
			num = 1 << (intNumber - 8);
			_asm in		al,0xa1
			_asm or		al,num
			_asm out    0xa1, al 
		}
	}
};
#endif