#include "serialDebug.h"
#include "../display/display.h"
#include "../string.h"
#include "../controls/ui.h"
#include "../logic/logic.h"
#include "../systems/io.h"

const SERIAL_PORT_SPEED SerialDebug::PORT_SPEED = SERIAL_PORT_SPEED_57600;
const SERIAL_PORT SerialDebug::PORT_NAME = SERIAL_PORT_1;

SerialDebug::SerialDebug()
	:	ITimer(TIMER_PERIOD), serialPort(SerialPortManager::getSingleton().getPort(PORT_NAME)), fifo(new Fifo<char>(FIFO_SIZE)), silentDebugOn(false), debugOn(false), debugInput(true)
{	
	pTimer->start();
	serialPort->setSpeed(PORT_SPEED);
	serialPort->open();
}

SerialDebug::~SerialDebug(){
	serialPort->close();
}

void SerialDebug::timerHandler(){
	if (!silentDebugOn){
		if (!serialPort->isSendActive() && !fifo->isEmpty()){
			serialPort->setNewSendData(fifo);
			serialPort->startSend();
		}
	}

	if (debugInput){
		Fifo<unsigned char>* rFifo = serialPort->getRecvFifo();
		if (!rFifo->isEmpty()){
			unsigned char ch;
			if (rFifo->get(&ch) != 0)
				debugCommand(ch);
		}
	}
}

/* Format

specifier	Output							Example
s			String of characters			sample
u			Unsigned decimal integer		7235
x			Unsigned hexadecimal integer	0x145c07fa
i			Signed decimal integer			-392
f			float							-392.0972

*/

void SerialDebug::put(char* str, ...){
	if (!debugOn)
		return;

	va_list args;
	va_start(args, str);

	while (*str != 0){
		if (*str == '%'){
			++str;
			
			if (*str == 0) 
				break;

			if (*str == 's'){
				char *substr = va_arg(args, char*);
		
				if (substr != nullptr){
					while (*substr != 0){
						fifo->put(*substr);
						++substr;
					}
				}
			}
			else 
				if (*str == 'u'){
					unsigned int value = va_arg(args, unsigned int);
					char _str[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
							
					char* _pstr = Display::getSingleton().toStringDecimal(value, _str);

					while (*_pstr != 0)
						fifo->put(*_pstr++);
				}
				else 
					if (*str == 'x'){
						unsigned int value = va_arg(args, unsigned int);
						char _str[9];
		
						Display::getSingleton().toString(value, _str);

						fifo->put('0');
						fifo->put('x');

						for (unsigned int i = 0; i < 8; ++i)
							fifo->put(_str[i]);
					}
					else 
						if (*str == 'i'){
							int value = va_arg(args, int);
							char _str[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
							
							char* _pstr = Display::getSingleton().toStringDecimalSigned(value, _str);

							while (*_pstr != 0)
								fifo->put(*_pstr++);
						}
						else 
							if (*str == 'f'){
								float value = static_cast<float>(va_arg(args, double));
								char _str[18] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
							
								char* _pstr = Display::getSingleton().toStringFloat(value, _str);

								while (*_pstr != 0)
									fifo->put(*_pstr++);
							}
							else 
								fifo->put(*str); 
		}
		else 
			fifo->put(*str); 

		++str;
	}
	   
	va_end(args);
}

void SerialDebug::debugCommand(unsigned char command){
	switch (command){
		case CTRL_KEY('d'): // включение/выключение отладочных сообщений
			if (debugOn){
				DEBUG_PUT_COLOR_ATTR(SerialDebug::COLOR_CYAN, SerialDebug::TEXT_ATTR_BOLD, "Вывод отладочных сообщений выключен.\n");
				debugOn = false;
			}else{
				debugOn = true;
				DEBUG_PUT_COLOR_ATTR(SerialDebug::COLOR_CYAN, SerialDebug::TEXT_ATTR_BOLD, "Вывод отладочных сообщений включен.\n");
			}
			break;
		case 'D': // включение/выключение тихого режима отладочных сообщений (сообщения не отправляются на терминал, но запоминаются в буфере)
			if (silentDebugOn){
				silentDebugOn = false;
			}else{
				silentDebugOn = true;
			}
			break;
		case CTRL_KEY('c'): // включение/выключение вывода сообщений Config
			sendMessage(Message(MESSAGE_FROM_OFFSET_SERIAL_DEBUG, MESSAGE_SERIAL_DEBUG_RECV_COMMAND, COMMAND_DEBUG_CONFIG, 0));
			break;
		case 'c': // вывод данных конфигурации
			sendMessage(Message(MESSAGE_FROM_OFFSET_SERIAL_DEBUG, MESSAGE_SERIAL_DEBUG_RECV_COMMAND, COMMAND_GET_SETTINGS, 0));
			break;
		case CTRL_KEY('l'): // включение/выключение вывода сообщений логики
			sendMessage(Message(MESSAGE_FROM_OFFSET_SERIAL_DEBUG, MESSAGE_SERIAL_DEBUG_RECV_COMMAND, COMMAND_DEBUG_LOGIC, 0));
			break;
		case CTRL_KEY('a'): // включение вывода сообщений Actions
			sendMessage(Message(MESSAGE_FROM_OFFSET_SERIAL_DEBUG, MESSAGE_SERIAL_DEBUG_RECV_COMMAND, COMMAND_DEBUG_ACTIONS_ON, 0));
			break;
		case 'A': // выключение вывода сообщений Actions
			sendMessage(Message(MESSAGE_FROM_OFFSET_SERIAL_DEBUG, MESSAGE_SERIAL_DEBUG_RECV_COMMAND, COMMAND_DEBUG_ACTIONS_OFF, 0));
			break;
		case CTRL_KEY('e'): // включение/выключение вывода сообщений ExtensionSystem
			sendMessage(Message(MESSAGE_FROM_OFFSET_SERIAL_DEBUG, MESSAGE_SERIAL_DEBUG_RECV_COMMAND, COMMAND_DEBUG_EXTENSION_SYSTEM, 0));
			break;
		case CTRL_KEY('i'): // включение/выключение вывода сообщений IoSubSystem
			sendMessage(Message(MESSAGE_FROM_OFFSET_SERIAL_DEBUG, MESSAGE_SERIAL_DEBUG_RECV_COMMAND, COMMAND_DEBUG_IOSUBSYSTEM, 0));
			break;
		case CTRL_KEY('m'): // включение/выключение вывода сообщений MonitoringSubSystem
			sendMessage(Message(MESSAGE_FROM_OFFSET_SERIAL_DEBUG, MESSAGE_SERIAL_DEBUG_RECV_COMMAND, COMMAND_DEBUG_MONITORINGSUBSYSTEM, 0));
			break;
		case CTRL_KEY('r'): // включение/выключение вывода сообщений RpkSubSystem
			sendMessage(Message(MESSAGE_FROM_OFFSET_SERIAL_DEBUG, MESSAGE_SERIAL_DEBUG_RECV_COMMAND, COMMAND_DEBUG_RPKSUBSYSTEM, 0));
			break;
		case CTRL_KEY('s'): // включение/выключение вывода сообщений DetectionSubSystem
			sendMessage(Message(MESSAGE_FROM_OFFSET_SERIAL_DEBUG, MESSAGE_SERIAL_DEBUG_RECV_COMMAND, COMMAND_DEBUG_DETECTIONSUBSYSTEM, 0));
			break;
		case CTRL_KEY('n'): // включение/выключение флага SCAN_PROGRAM_FLAGS в actionStartProgramScanPoint
			sendMessage(Message(MESSAGE_FROM_OFFSET_SERIAL_DEBUG, MESSAGE_SERIAL_DEBUG_RECV_COMMAND, COMMAND_DEBUG_ACTIONS_NASADOK_ON_OFF, 0));
			break;
		case 'm':
			//for (unsigned int i = 0; i < MainMemoryList::BIG_BLOCKS_SIZE; i++)
			//	DEBUG_PUT_METHOD("bigBlocks[%u].address = %x bigBlocks[%u].count = %u\n", i, MemoryAllocator::getSingleton().mainMemoryList.bigBlocks[i].address, i, MemoryAllocator::getSingleton().mainMemoryList.bigBlocks[i].count)

			DEBUG_PUT_METHOD("Allocator::getFreeMemory = %u\n", MemoryAllocator::getSingleton().getFreeMemory())
			break;
		case CTRL_KEY('t'): // включение режима калибровки сенсорной панели
			sendMessage(Message(MESSAGE_FROM_OFFSET_SERIAL_DEBUG, MESSAGE_SERIAL_DEBUG_RECV_COMMAND, COMMAND_DEBUG_TOUCHPAD_CALIBRATION_ON, 0));
			break;
		case 't': // отмена режима калибровки без сохранения результатов
			sendMessage(Message(MESSAGE_FROM_OFFSET_SERIAL_DEBUG, MESSAGE_SERIAL_DEBUG_RECV_COMMAND, COMMAND_DEBUG_TOUCHPAD_CALIBRATION_CANCEL, 0));
			break;
		case 'T': // сохранение результатов калибровки и выключение режима калибровки
			sendMessage(Message(MESSAGE_FROM_OFFSET_SERIAL_DEBUG, MESSAGE_SERIAL_DEBUG_RECV_COMMAND, COMMAND_DEBUG_TOUCHPAD_CALIBRATION_SAVE, 0));
			break;
		case 'y': // вывод данных калибровки
			sendMessage(Message(MESSAGE_FROM_OFFSET_SERIAL_DEBUG, MESSAGE_SERIAL_DEBUG_RECV_COMMAND, COMMAND_DEBUG_TOUCHPAD_CALIBRATION_GET_DATA, 0));
			break;
		case CTRL_KEY('y'): // установка левой верхней точки для калибровки
			sendMessage(Message(MESSAGE_FROM_OFFSET_SERIAL_DEBUG, MESSAGE_SERIAL_DEBUG_RECV_COMMAND, COMMAND_DEBUG_TOUCHPAD_CALIBRATION_SET_LEFT_UP_POINT, 0));
			break;
		case 'Y': // установка правой нижней точки для калибровки
			sendMessage(Message(MESSAGE_FROM_OFFSET_SERIAL_DEBUG, MESSAGE_SERIAL_DEBUG_RECV_COMMAND, COMMAND_DEBUG_TOUCHPAD_CALIBRATION_SET_RIGHT_DOWN_POINT, 0));
			break;
		case '1': // установка входа
			sendMessage(Message(MESSAGE_FROM_OFFSET_SERIAL_DEBUG, MESSAGE_SERIAL_DEBUG_RECV_COMMAND, COMMAND_DEBUG_BK16_SET_INPUTS, 1));
			break;
		case '2': // установка входа
			sendMessage(Message(MESSAGE_FROM_OFFSET_SERIAL_DEBUG, MESSAGE_SERIAL_DEBUG_RECV_COMMAND, COMMAND_DEBUG_BK16_SET_INPUTS, 2));
			break;
		case '3': // установка входа
			sendMessage(Message(MESSAGE_FROM_OFFSET_SERIAL_DEBUG, MESSAGE_SERIAL_DEBUG_RECV_COMMAND, COMMAND_DEBUG_BK16_SET_INPUTS, 3));
			break;
		case '4': // установка входа
			sendMessage(Message(MESSAGE_FROM_OFFSET_SERIAL_DEBUG, MESSAGE_SERIAL_DEBUG_RECV_COMMAND, COMMAND_DEBUG_BK16_SET_INPUTS, 4));
			break;
		case '5': // установка входа
			sendMessage(Message(MESSAGE_FROM_OFFSET_SERIAL_DEBUG, MESSAGE_SERIAL_DEBUG_RECV_COMMAND, COMMAND_DEBUG_BK16_SET_INPUTS, 5));
			break;
		case '6': // установка входа
			sendMessage(Message(MESSAGE_FROM_OFFSET_SERIAL_DEBUG, MESSAGE_SERIAL_DEBUG_RECV_COMMAND, COMMAND_DEBUG_BK16_SET_INPUTS, 6));
			break;
		case '7': // установка входа
			sendMessage(Message(MESSAGE_FROM_OFFSET_SERIAL_DEBUG, MESSAGE_SERIAL_DEBUG_RECV_COMMAND, COMMAND_DEBUG_BK16_SET_INPUTS, 7));
			break;
		case '8': // установка входа
			sendMessage(Message(MESSAGE_FROM_OFFSET_SERIAL_DEBUG, MESSAGE_SERIAL_DEBUG_RECV_COMMAND, COMMAND_DEBUG_BK16_SET_INPUTS, 8));
			break;
		case '9': // установка входа
			sendMessage(Message(MESSAGE_FROM_OFFSET_SERIAL_DEBUG, MESSAGE_SERIAL_DEBUG_RECV_COMMAND, COMMAND_DEBUG_BK16_SET_INPUTS, 9));
			break;
		case CTRL_KEY('k'): // сброс всех входов в неопределенное состояние
			sendMessage(Message(MESSAGE_FROM_OFFSET_SERIAL_DEBUG, MESSAGE_SERIAL_DEBUG_RECV_COMMAND, COMMAND_DEBUG_BK16_RESET_INPUTS, 0));
			break;
		case 'h':
			DEBUG_PUT_COLOR_ATTR(COLOR_BLUE, TEXT_ATTR_BOLD, "Справка:\n\n")
			DEBUG_PUT_COLOR(COLOR_BLUE, "   CTRL + d  -  включение/выключение отладочных сообщений\n")
			DEBUG_PUT_COLOR(COLOR_BLUE, "          D  -  включение/выключение тихого режима отладочных сообщений\n                 (сообщения не отправляются на терминал, но запоминаются в буфере)\n")
			DEBUG_PUT_COLOR(COLOR_BLUE, "   CTRL + l  -  включение/выключение вывода сообщений логики\n")
			DEBUG_PUT_COLOR(COLOR_BLUE, "   CTRL + a  -  включение вывода сообщений Actions\n")
			DEBUG_PUT_COLOR(COLOR_BLUE, "          A  -  выключение вывода сообщений Actions\n")
			DEBUG_PUT_COLOR(COLOR_BLUE, "   CTRL + e  -  включение/выключение вывода сообщений ExtensionSystem\n")
			DEBUG_PUT_COLOR(COLOR_BLUE, "   CTRL + i  -  включение/выключение вывода сообщений IoSubSystem\n")
			DEBUG_PUT_COLOR(COLOR_BLUE, "   CTRL + m  -  включение/выключение вывода сообщений MonitoringSubSystem\n")
			DEBUG_PUT_COLOR(COLOR_BLUE, "   CTRL + r  -  включение/выключение вывода сообщений RpkSubSystem\n")
			DEBUG_PUT_COLOR(COLOR_BLUE, "   CTRL + c  -  включение/выключение вывода сообщений Config\n")
			DEBUG_PUT_COLOR(COLOR_BLUE, "   CTRL + s  -  включение/выключение вывода сообщений DetectionSubSystem\n")
			DEBUG_PUT_COLOR(COLOR_BLUE, "   CTRL + n  -  включение/выключение флага SCAN_PROGRAM_FLAGS_NASADOK_ON/OFF в actionStartProgramScanPoint\n")
			DEBUG_PUT_COLOR(COLOR_BLUE, "          c  -  вывод данных конфигурации\n")
			DEBUG_PUT_COLOR(COLOR_BLUE, "          m  -  вывод информации о памяти\n")
			DEBUG_PUT_COLOR(COLOR_BLUE, "   CTRL + t  -  включение режима калибровки сенсорной панели\n")
			DEBUG_PUT_COLOR(COLOR_BLUE, "          t  -  отмена режима калибровки без сохранения результатов\n")
			DEBUG_PUT_COLOR(COLOR_BLUE, "          T  -  сохранение результатов калибровки и выключение режима калибровки\n")
			DEBUG_PUT_COLOR(COLOR_BLUE, "          y  -  вывод данных калибровки\n")
			DEBUG_PUT_COLOR(COLOR_BLUE, "   CTRL + y  -  установка левой верхней точки для калибровки\n")
			DEBUG_PUT_COLOR(COLOR_BLUE, "          Y  -  установка правой нижней точки для калибровки\n")
			DEBUG_PUT_COLOR(COLOR_BLUE, "       1..9  -  вкл.выкл входа БК16 номер 1..9\n")
			DEBUG_PUT_COLOR(COLOR_BLUE, "   CTRL + k  -  сброс всех входов в неопределенное состояние\n")
			DEBUG_PUT_COLOR(COLOR_BLUE, "          h  -  вывод справки\n")
			break;
		case '=':
			SystemIo::getSingleton().debugIsInputOnSwitch = ~SystemIo::getSingleton().debugIsInputOnSwitch;
			break;
	}
}

bool SerialDebug::isOn(){
	return debugOn;
}

void SerialDebug::on(){
	debugInput = true;
	debugOn = true;
}

void SerialDebug::off(){
	debugInput = false;
	debugOn = false;
}
