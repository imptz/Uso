/*! \file serialport.h
    \brief Последовательные порты.    
*/

#pragma once

#include "../fifo.h"
#include "../Singleton.h"

#include "../display/display.h"

//! Номера последовательных портов
enum SERIAL_PORT
{
	SERIAL_PORT_1 = 0, /*!< COM1 */
	SERIAL_PORT_2 = 1, /*!< COM2 */
	SERIAL_PORT_3 = 2 /*!< COM3 */
};

//! Базовые адреса последовательных портов
enum SERIAL_PORT_BASE_ADDRESS
{
	SERIAL_PORT_BASE_ADDRESS_1 = 0x3f8, /*!< COM1 */
	SERIAL_PORT_BASE_ADDRESS_2 = 0x2f8, /*!< COM2 */
	SERIAL_PORT_BASE_ADDRESS_3 = 0x2e8 /*!< COM3 */
};

//! Номера векторов прерываний последовательных портов
enum SERIAL_PORT_IRQ
{
	SERIAL_PORT_IRQ_1 = 4, /*!< COM1 */
	SERIAL_PORT_IRQ_2 = 3, /*!< COM2 */
	SERIAL_PORT_IRQ_3 = 7 /*!< COM3 */
};

//! Значения регистра устаногвки скорости последовательного порта
enum SERIAL_PORT_SPEED
{
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

//! Смещения регистров последовательного порта в пространстве ввода-вывода
enum SERIAL_PORT_REG
{
	SERIAL_PORT_REG_0 = 0x00, /*!< ... */
	SERIAL_PORT_REG_IER = 0x01, /*!< IER */
	SERIAL_PORT_REG_IIR = 0x02, /*!< IIR */
	SERIAL_PORT_REG_LCR = 0x03, /*!< ... */
	SERIAL_PORT_REG_MCR = 0x04, /*!< MCR */
	SERIAL_PORT_REG_LSR = 0x05, /*!< LSR */
	SERIAL_PORT_REG_MSR = 0x06 /*!< MSR */
};

//! Маски битов регистра IIR
enum SERIAL_PORT_IIR
{
	SERIAL_PORT_IIR_IP = 0x01, /*!< IP */
	SERIAL_PORT_IIR_THREI = 0x02, /*!< THREI */
	SERIAL_PORT_IIR_RDAI = 0x04, /*!< RDAI */
};

//! Маски битов регистра LSR
enum SERIAL_PORT_LSR
{
	SERIAL_PORT_LSR_DR = 0x01, /*!< DR */
	SERIAL_PORT_LSR_OE = 0x02, /*!< OE */
	SERIAL_PORT_LSR_FE = 0x08, /*!< FE */
	SERIAL_PORT_LSR_ETHR = 0x20, /*!< ETHR */
	SERIAL_PORT_LSR_EDHR = 0x40 /*!< EDHR */
};


class SerialPortManager;

//! Класс последовательного порта
class SerialPort
{
	friend class SerialPortManager;
	
	protected:
		SERIAL_PORT_BASE_ADDRESS baseAddress; /*!< Базовый адрес порта */
		SERIAL_PORT_IRQ irq; /*!< Прерывание порта */
		//! Обработчик прерывания по приему
		/*!
			Вызывается из статического обработчика прерываний определенного порта
		*/
		void irqHandlerRecv();
		//! Обработчик прерывания по передаче
		/*!
			Вызывается из статического обработчика прерываний определенного порта
		*/
		void irqHandlerSend();

		static const unsigned int RECV_FIFO_SIZE = 1024 * 64; /*!< размер буфера приема */
		Fifo<unsigned char> *fifoRecv; /*!< указатель на буфер приема */
	public:
		//! Получить указатель на буфер приема
		/*!
			@return указатель на буфер приема
		*/
		Fifo<unsigned char>* getRecvFifo();

	protected:
		static const unsigned int SEND_FIFO_SIZE = 20000; /*!< размер юуфера передачи */
		Fifo<unsigned char> *fifoSend; /*!< указатель на буфер передачи */
	public:
		//! Получить указатель на буфер передачи
		/*!
			@return указатель на буфер передачи
		*/
		Fifo<unsigned char>* getSendFifo();

	private:
		SerialPort(SERIAL_PORT_BASE_ADDRESS _baseAddress, SERIAL_PORT_IRQ _irq);
		~SerialPort();

	public:
		//! Открыть порт
		void open();
		//! Закрыть порт
		void close();
		//! Установить скорость обмена
		/*!
			@param speed - скорость обмена
		*/
		void setSpeed(SERIAL_PORT_SPEED speed);
	protected:
		bool sendActive; /*!< Флаг активности передачи данных. true - выполняется передача данных, false - передача данных не выполняется */
	public:
		//! Получить активность передачи
		/*!
			@return true - выполняется передача, false - передача не выполняется
		*/
		bool isSendActive();
		//! Старт передачи
		/*!
			Отправляется первый байт данных, устанавливается признак передачи
		*/
		void startSend();

	private:
		unsigned char* pSendData; /*!< указатель на данные для передачи */
		unsigned int sendDataSize; /*!< размер данных для передачи */
		unsigned int sendDataCount; /*!<  */
	public:
		//! Установить новые данные для передачи
		/*!
			Старые данные для передачи удаляются. Распределяется память под новые данные.
			Новые данные копируются из источника (_pData)

			@param _pData - указатель на данные для передачи
			@param _size - размер данных
		*/
		void setNewSendData(unsigned char* _pData, unsigned int _size);
		void setNewSendData(char* _pData, unsigned int _size)
		{
			SAFE_DELETE_ARRAY(pSendData)

			pSendData = new unsigned char[_size];
			memcpy(pSendData, _pData, _size);
			sendDataCount = 0;
			sendDataSize = _size;
		}

		template<typename T>
		void setNewSendData(Fifo<T>* pFifo)
		{
			SAFE_DELETE_ARRAY(pSendData)

			unsigned int _size = pFifo->getDataSize();
			pSendData = new unsigned char[_size];
			pFifo->get(reinterpret_cast<char*>(pSendData), _size, true);

			sendDataCount = 0;
			sendDataSize = _size;
		}

	private:
		SERIAL_PORT_SPEED portSpeed; /*!< Скорость обмена */
	public:
		//! Получить скорость обмена
		/*!
			@return установленная скорость обмена
		*/
		unsigned int getSpeed();
};

//! Класс менеджера последовательных портов
/*!
	Синглетон класс. Управляет всеми последовательными портами в системе
*/
class SerialPortManager : public Singleton<SerialPortManager>
{
	private:
		static SerialPort* pSerialPorts[3]; /*!< Статический массив указателей на последовательные порты системы */
		//! Статический обработчик прерываний Com1
		static void irqCom1();
		//! Статический обработчик прерываний Com2
		static void irqCom2();
		//! Статический обработчик прерываний Com3
		static void irqCom3();

	public:
		SerialPortManager();
		//! Получить порт
		/*!
			@param port - последовательный порт

			@return указатель на порт
		*/
		SerialPort* getPort(SERIAL_PORT port);
};

