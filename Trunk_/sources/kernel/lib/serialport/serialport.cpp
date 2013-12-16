#include "SerialPort.h"
#include "../low.h"
#include "../interrupt/interrupt.h"


Fifo<unsigned char>* SerialPort::getRecvFifo()
{
	return fifoRecv;
}

Fifo<unsigned char>* SerialPort::getSendFifo()
{
	return fifoSend;
}

SerialPort::SerialPort(SERIAL_PORT_BASE_ADDRESS _baseAddress, SERIAL_PORT_IRQ _irq)
	:	baseAddress(_baseAddress), irq(_irq), fifoRecv(new Fifo<unsigned char>(RECV_FIFO_SIZE)), fifoSend(new Fifo<unsigned char>(SEND_FIFO_SIZE)), pSendData(nullptr), sendDataSize(0), sendDataCount(0),
	portSpeed(SERIAL_PORT_SPEED_57600)
{
	switch (baseAddress)
	{
		case SERIAL_PORT_BASE_ADDRESS_1:
			SET_INT_HANDLER(Interrupts::HARDWARE_VECTOR_OFFSET + irq, SerialPortManager::irqCom1);
			break;
		case SERIAL_PORT_BASE_ADDRESS_2:
			SET_INT_HANDLER(Interrupts::HARDWARE_VECTOR_OFFSET + irq, SerialPortManager::irqCom2);
			break;
		case SERIAL_PORT_BASE_ADDRESS_3:
			SET_INT_HANDLER(Interrupts::HARDWARE_VECTOR_OFFSET + irq, SerialPortManager::irqCom3);
			break;
	}

	outPort(baseAddress + SERIAL_PORT_REG_LCR, 0x80);
	outPort(baseAddress + 0, SERIAL_PORT_SPEED_57600); 
	outPort(baseAddress + SERIAL_PORT_REG_IER, SERIAL_PORT_SPEED_57600 >> 8);
	outPort(baseAddress + SERIAL_PORT_REG_LCR, 0x03);
	outPort(baseAddress + SERIAL_PORT_REG_IIR, 0x00);
	outPort(baseAddress + SERIAL_PORT_REG_MCR, 0x0b);
	outPort(baseAddress + SERIAL_PORT_REG_IER, 0x03);

	close();
}

SerialPort::~SerialPort()
{
	close();
	delete fifoSend;
	delete fifoRecv;
}


void SerialPort::open()
{
	Interrupts::enablingHardwareInterrupt(irq);
}

void SerialPort::close()
{
	Interrupts::disablingHardwareInterrupt(irq);
}

void SerialPort::setSpeed(SERIAL_PORT_SPEED speed)
{
	portSpeed = speed;
	_asm cli
	outPort(baseAddress + SERIAL_PORT_REG_LCR, 0x80);
	outPort(baseAddress + 0, speed); 
	outPort(baseAddress + SERIAL_PORT_REG_IER, speed >> 8);
	outPort(baseAddress + SERIAL_PORT_REG_LCR, 0x03);
	_asm sti
}

// если не выключить оптимизация для этого метода, то ошибки при выполнении
#pragma optimize("", off)
void SerialPort::irqHandlerRecv()
{
	unsigned char lsr = inPort(baseAddress + SERIAL_PORT_REG_LSR);

	while ((lsr & SERIAL_PORT_LSR_DR) != 0)
	{
		unsigned char value = inPort(baseAddress);
		if ((lsr & (SERIAL_PORT_LSR_OE | SERIAL_PORT_LSR_FE)) == 0)
			fifoRecv->put(reinterpret_cast<unsigned char*>(&value));

		lsr = inPort(baseAddress + SERIAL_PORT_REG_LSR);
	}
}

void SerialPort::irqHandlerSend()
{
	if (sendDataCount == sendDataSize)
	{
		sendActive = false;
	}
	else
	{
		outPort(baseAddress, pSendData[sendDataCount]);
		sendDataCount++;
	}
}
#pragma optimize("", on)

bool SerialPort::isSendActive()
{
	return sendActive;
}

void SerialPort::startSend()
{
	if (!sendActive)
	{
		outPort(baseAddress, pSendData[0]);
		sendDataCount++;
		sendActive = true;
	}
}

void SerialPort::setNewSendData(unsigned char* _pData, unsigned int _size)
{
	SAFE_DELETE_ARRAY(pSendData)
	pSendData = new unsigned char[_size];
	memcpy(pSendData, _pData, _size);
	sendDataCount = 0;
	sendDataSize = _size;
}

unsigned int SerialPort::getSpeed()
{
	switch (portSpeed)
	{
		case 1:
			return 115200;
			break;
		case 2:
			return 57600;
			break;
		default:
			return 115200;
			break;
	}
}

SerialPort* SerialPortManager::pSerialPorts[3];

SerialPortManager::SerialPortManager()
{
	pSerialPorts[0] = new SerialPort(SERIAL_PORT_BASE_ADDRESS_1, SERIAL_PORT_IRQ_1);
	pSerialPorts[1] = new SerialPort(SERIAL_PORT_BASE_ADDRESS_2, SERIAL_PORT_IRQ_2);
	pSerialPorts[2] = new SerialPort(SERIAL_PORT_BASE_ADDRESS_3, SERIAL_PORT_IRQ_3);
}

SerialPort* SerialPortManager::getPort(SERIAL_PORT port)
{
	return pSerialPorts[port];
}

unsigned char irqCom1_i;
unsigned char irqCom2_i;
unsigned char irqCom3_i;

_declspec(naked) void SerialPortManager::irqCom1()
{
	_asm cli
	_asm pushfd
	_asm pushad

	irqCom1_i = inPort(pSerialPorts[0]->baseAddress + SERIAL_PORT_REG_IIR);

	while ((irqCom1_i & SERIAL_PORT_IIR_IP) == 0)
	{
		if ((irqCom1_i & SERIAL_PORT_IIR_RDAI) != 0) 
			pSerialPorts[0]->irqHandlerRecv();
		if ((irqCom1_i & SERIAL_PORT_IIR_THREI) != 0) 
			pSerialPorts[0]->irqHandlerSend();
		irqCom1_i = inPort(pSerialPorts[0]->baseAddress + SERIAL_PORT_REG_IIR);
	}

	_asm mov	al,0x20
	_asm out	0x20,al
	_asm popad
	_asm popfd
	_asm sti
	_asm iretd
}

_declspec(naked) void SerialPortManager::irqCom2()
{
	_asm cli
	_asm pushfd
	_asm pushad

	irqCom2_i = inPort(pSerialPorts[1]->baseAddress + SERIAL_PORT_REG_IIR);

	while ((irqCom2_i & SERIAL_PORT_IIR_IP) == 0)
	{
		if ((irqCom2_i & SERIAL_PORT_IIR_RDAI) != 0) 
			pSerialPorts[1]->irqHandlerRecv();
		if ((irqCom2_i & SERIAL_PORT_IIR_THREI) != 0) 
			pSerialPorts[1]->irqHandlerSend();
		irqCom2_i = inPort(pSerialPorts[1]->baseAddress + SERIAL_PORT_REG_IIR);
	}

	_asm mov	al,0x20
	_asm out	0x20,al
	_asm popad
	_asm popfd
	_asm sti
	_asm iretd
}

_declspec(naked) void SerialPortManager::irqCom3()
{
	_asm cli
	_asm pushfd
	_asm pushad

	irqCom3_i = inPort(pSerialPorts[2]->baseAddress + SERIAL_PORT_REG_IIR);

	while ((irqCom3_i & SERIAL_PORT_IIR_IP) == 0)
	{
		if ((irqCom3_i & SERIAL_PORT_IIR_RDAI) != 0) 
			pSerialPorts[2]->irqHandlerRecv();
		if ((irqCom3_i & SERIAL_PORT_IIR_THREI) != 0) 
			pSerialPorts[2]->irqHandlerSend();
		irqCom3_i = inPort(pSerialPorts[2]->baseAddress + SERIAL_PORT_REG_IIR);
	}

	_asm mov	al,0x20
	_asm out	0x20,al
	_asm popad
	_asm popfd
	_asm sti
	_asm iretd
}
