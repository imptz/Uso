#pragma once

#include "../fifo.h"
#include "../singleton.h"

#include "../display/display.h"

enum SERIAL_PORT{
	SERIAL_PORT_1 = 0,
	SERIAL_PORT_2 = 1,
	SERIAL_PORT_3 = 2
};

enum SERIAL_PORT_BASE_ADDRESS{
	SERIAL_PORT_BASE_ADDRESS_1 = 0x3f8,
	SERIAL_PORT_BASE_ADDRESS_2 = 0x2f8,
	SERIAL_PORT_BASE_ADDRESS_3 = 0x2e8
};

enum SERIAL_PORT_IRQ{
	SERIAL_PORT_IRQ_1 = 4,
	SERIAL_PORT_IRQ_2 = 3,
	SERIAL_PORT_IRQ_3 = 7
};

enum SERIAL_PORT_SPEED{
	SERIAL_PORT_SPEED_50 = 0x0900,
	SERIAL_PORT_SPEED_300 = 0x0180,
	SERIAL_PORT_SPEED_600 = 0x00c0,
	SERIAL_PORT_SPEED_2400 = 0x0030,
	SERIAL_PORT_SPEED_4800 = 0x0018,
	SERIAL_PORT_SPEED_9600 = 0x000c,
	SERIAL_PORT_SPEED_19200 = 0x0006,
	SERIAL_PORT_SPEED_38400 = 0x0003,
	SERIAL_PORT_SPEED_57600 = 0x0002,
	SERIAL_PORT_SPEED_115200 = 0x0001
};

enum SERIAL_PORT_REG{
	SERIAL_PORT_REG_0 = 0x00,
	SERIAL_PORT_REG_IER = 0x01,
	SERIAL_PORT_REG_IIR = 0x02,
	SERIAL_PORT_REG_LCR = 0x03,
	SERIAL_PORT_REG_MCR = 0x04,
	SERIAL_PORT_REG_LSR = 0x05,
	SERIAL_PORT_REG_MSR = 0x06
};

enum SERIAL_PORT_IIR{
	SERIAL_PORT_IIR_IP = 0x01,
	SERIAL_PORT_IIR_THREI = 0x02,
	SERIAL_PORT_IIR_RDAI = 0x04,
};

enum SERIAL_PORT_LSR{
	SERIAL_PORT_LSR_DR = 0x01,
	SERIAL_PORT_LSR_OE = 0x02,
	SERIAL_PORT_LSR_FE = 0x08,
	SERIAL_PORT_LSR_ETHR = 0x20,
	SERIAL_PORT_LSR_EDHR = 0x40
};


class SerialPortManager;

class SerialPort{
	friend class SerialPortManager;
	
protected:
	SERIAL_PORT_BASE_ADDRESS baseAddress;
	SERIAL_PORT_IRQ irq;
	void irqHandlerRecv();
	void irqHandlerSend();

	static const unsigned int RECV_FIFO_SIZE = 1024 * 64;
	Fifo<unsigned char> *fifoRecv;
public:
	Fifo<unsigned char>* getRecvFifo();

protected:
	static const unsigned int SEND_FIFO_SIZE = 20000;
	Fifo<unsigned char> *fifoSend;
public:
	Fifo<unsigned char>* getSendFifo();

private:
	SerialPort(SERIAL_PORT_BASE_ADDRESS _baseAddress, SERIAL_PORT_IRQ _irq);
	~SerialPort();

public:
	void open();
	void close();
	void setSpeed(SERIAL_PORT_SPEED speed);
protected:
	bool sendActive;
public:
	bool isSendActive();
	void startSend();

private:
	unsigned char* pSendData;
	unsigned int sendDataSize;
	unsigned int sendDataCount;
public:
	void setNewSendData(unsigned char* _pData, unsigned int _size);
	void setNewSendData(char* _pData, unsigned int _size){
		SAFE_DELETE_ARRAY(pSendData)

		pSendData = new unsigned char[_size];
		memcpy(pSendData, _pData, _size);
		sendDataCount = 0;
		sendDataSize = _size;
	}

	template<typename T>
	void setNewSendData(Fifo<T>* pFifo){
		SAFE_DELETE_ARRAY(pSendData)

		unsigned int _size = pFifo->getDataSize();
		pSendData = new unsigned char[_size];
		pFifo->get(reinterpret_cast<char*>(pSendData), _size, true);

		sendDataCount = 0;
		sendDataSize = _size;
	}

private:
	SERIAL_PORT_SPEED portSpeed;
public:
	unsigned int getSpeed();
};

class SerialPortManager : public Singleton<SerialPortManager>{
private:
	static SerialPort* pSerialPorts[3];
	static void irqCom1();
	static void irqCom2();
	static void irqCom3();

public:
	SerialPortManager();
	SerialPort* getPort(SERIAL_PORT port);
};

