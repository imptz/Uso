#include "hdd.h"
#include "../low.h"
#include "../interrupt/interrupt.h"

unsigned short Hdd::ideBaseAddress;
volatile unsigned int Hdd::actionCounter;
Hdd::RW_FLAG Hdd::rwFlag;
Hdd::PRDF Hdd::prdf;

unsigned char Hdd::sectorsCount;
unsigned char* Hdd::pData;

unsigned char Hdd::buffer[Hdd::PRDF_BUFFER_SIZE];

volatile unsigned char _wwr1;
volatile unsigned char _wwr2;
volatile unsigned char _aassww = 0;

_declspec(naked) void Hdd::staticIrqHandler(){
	_asm cli
	_asm pushfd
	_asm pushad

	_asm{
		mov dx, ideBaseAddress
		mov al,0
		out dx,al
	
		mov dx, IDE_STATUS_COMMAND_R
		in al,dx
	}

	if (rwFlag == RW_FLAG_READ)
		memcpy(pData, buffer, sectorsCount * 512);

	actionCounter--;
	
	_asm mov	al,0x20
	_asm out	0xa0,al
	_asm popad
	_asm popfd
	_asm sti
	_asm iretd
}

Hdd::Hdd(){
	actionCounter = 0;
	rwFlag = RW_FLAG_READ;

	prdf.MRPBA = reinterpret_cast<unsigned int>(buffer);
	prdf.EOT_SIZE = PRDF_EOT;

	_asm{
		mov ecx, 0x51300008
		rdmsr
		and eax, 0xfffffff0
		mov ideBaseAddress, ax
	}
	
	SET_INT_HANDLER(Interrupts::HARDWARE_VECTOR_OFFSET + 14, Hdd::staticIrqHandler);
	Interrupts::enablingHardwareInterrupt(14);
	Interrupts::enablingHardwareInterrupt(2);
}

Hdd::~Hdd(){
	Interrupts::disablingHardwareInterrupt(2);
	Interrupts::disablingHardwareInterrupt(14);
}

unsigned int Hdd::getActionCounter(){
	return actionCounter;
}

/**
ћаксимальное число читаемых секторов: 65536 / 512 = 128
*/
bool Hdd::read(unsigned int _startSectorNumber, unsigned char _sectorsCount, unsigned char* _pData){
	actionCounter++;
	if (actionCounter > 1){
		actionCounter--;
		return false;
	}

	pData = _pData;
	sectorsCount = _sectorsCount;

	if ((_sectorsCount == 0) ||(_sectorsCount > MAX_SECTOR_COUNT)){
		actionCounter--;
		return false;
	}

	rwFlag = RW_FLAG_READ;

	if (_sectorsCount == MAX_SECTOR_COUNT)
		prdf.EOT_SIZE = PRDF_EOT;
	else
		prdf.EOT_SIZE = PRDF_EOT + _sectorsCount * 512;

	PRDF* pPrdf = &prdf;

	 _asm{
		mov dx, IDE_DEVICE_CONTROL_R
		mov al,0x00
		out dx,al

		mov dx, IDE_STATUS_COMMAND_R
L1:
		in al,dx
		test al, IDE_BUSY
		jnz L1

		mov al,0xf0 // slave
//		mov al,0x70 // master
		mov dx, IDE_LBA_3_R
		out dx,al

		mov dx, IDE_STATUS_COMMAND_R
L2:
		in al,dx
		test al, IDE_BUSY
		jnz L2
		test al, IDE_DRDY
		jz L2

		mov eax, _startSectorNumber 
		mov dx, IDE_LBA_0_R
		out dx,al
		shr eax, 8
		mov dx, IDE_LBA_1_R
		out dx,al
		shr eax, 8
		mov dx, IDE_LBA_2_R
		out dx,al

		mov dx, IDE_SECTOR_COUNT_R
		mov al, _sectorsCount
		out dx,al

		mov dx, ideBaseAddress
		add dx, 4
		mov eax, pPrdf
		out dx,eax

		mov dx, ideBaseAddress
		mov al, IDE_BM_CMD_RWCTL
		out dx, al

		mov dx, ideBaseAddress
		add dx, 2
		mov al, 0x06
		out dx, al

		mov dx, IDE_STATUS_COMMAND_R
		mov al, READ_DMA_WITH_RETRY
		out dx, al

		mov dx, ideBaseAddress
		mov al, IDE_BM_CMD_RWCTL | IDE_BM_CMD_BMCTL
		out dx, al
	 }
}

/**
ћаксимальное число записываемых секторов: 65536 / 512 = 128
*/
bool Hdd::write(unsigned int _startSectorNumber, unsigned char _sectorsCount, unsigned char* _pData){
	actionCounter++;
	if (actionCounter > 1){
		actionCounter--;
		return false;
	}

	if ((_sectorsCount == 0) || (_sectorsCount > MAX_SECTOR_COUNT)){
		actionCounter--;
		return false;
	}

	rwFlag = RW_FLAG_WRITE;

	if (_sectorsCount == MAX_SECTOR_COUNT)
		prdf.EOT_SIZE = PRDF_EOT;
	else
		prdf.EOT_SIZE = PRDF_EOT + _sectorsCount * 512;

	PRDF* pPrdf = &prdf;

	memcpy(buffer, _pData, _sectorsCount * 512);

	 _asm{
		mov dx, IDE_DEVICE_CONTROL_R
		mov al,0x00
		out dx,al

		mov dx, IDE_STATUS_COMMAND_R
L1:
		in al,dx
		test al, IDE_BUSY
		jnz L1

		mov al,0xf0 
		mov dx, IDE_LBA_3_R
		out dx,al

		mov dx, IDE_STATUS_COMMAND_R
L2:
		in al,dx
		test al, IDE_BUSY
		jnz L2
		test al, IDE_DRDY
		jz L2

		mov eax, _startSectorNumber 
		mov dx, IDE_LBA_0_R
		out dx,al
		shr eax, 8
		mov dx, IDE_LBA_1_R
		out dx,al
		shr eax, 8
		mov dx, IDE_LBA_2_R
		out dx,al

		mov dx, IDE_SECTOR_COUNT_R
		mov al, _sectorsCount
		out dx,al

		mov dx, ideBaseAddress
		add dx, 4
		mov eax, pPrdf
		out dx,eax

		mov dx, ideBaseAddress
		mov al, 0
		out dx, al

		mov dx, ideBaseAddress
		add dx, 2
		mov al, 0x06
		out dx, al

		mov dx, IDE_STATUS_COMMAND_R
		mov al, WRITE_DMA_WITH_RETRY
		out dx, al

		mov dx, ideBaseAddress
		mov al, IDE_BM_CMD_BMCTL
		out dx, al
	 }
}
