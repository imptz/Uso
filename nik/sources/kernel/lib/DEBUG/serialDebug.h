#ifndef SERIAL_DEBUG_H
#define SERIAL_DEBUG_H

#include "../singleton.h"
#include "../timer/timer.h"
#include "../serialport/serialport.h"
#include "../fifo.h"
#include <stdarg.h>
#include "../message/messages.h"

#define DEBUG_PUT(...) SerialDebug::getSingleton().put(__VA_ARGS__);
#define DEBUG_IF_PUT(...) {								\
	if (debug)											\
		SerialDebug::getSingleton().put(__VA_ARGS__);}	\

#define DEBUG_RESET_COLOR_ATTR() {						\
	DEBUG_SET_TEXT_COLOR(SerialDebug::COLOR_WHITE)		\
	DEBUG_SET_TEXT_ATTR(SerialDebug::TEXT_ATTR_RESET)}						

#define DEBUG_PUT_COLOR_ATTR(COLOR, ATTR, ...) {	\
	DEBUG_SET_TEXT_COLOR(COLOR)						\
	DEBUG_SET_TEXT_ATTR(ATTR)						\
	DEBUG_PUT(__VA_ARGS__)							\
	DEBUG_RESET_COLOR_ATTR()}
	
#define DEBUG_PUT_COLOR(COLOR, ...) {	\
	DEBUG_SET_TEXT_COLOR(COLOR)						\
	DEBUG_PUT(__VA_ARGS__)							\
	DEBUG_RESET_COLOR_ATTR()}
	
#define DEBUG_IF_PUT_COLOR_ATTR(COLOR, ATTR, ...) {		\
	if (debug)											\
		DEBUG_PUT_COLOR_ATTR(COLOR, ATTR, __VA_ARGS__) }		

#define DEBUG_IF_PUT_COLOR(COLOR, ...) {		\
	if (debug)											\
		DEBUG_PUT_COLOR(COLOR, __VA_ARGS__) }		

#define DEBUG_PUT_METHOD(...) {		\
	DEBUG_PUT_COLOR(SerialDebug::COLOR_YELLOW, "%s: ", __FUNCTION__)\
	DEBUG_PUT(__VA_ARGS__) }						\

#define DEBUG_IF_PUT_METHOD(...) {		\
	if (debug)											\
	{													\
		DEBUG_PUT_COLOR(SerialDebug::COLOR_YELLOW, "%s: ", __FUNCTION__)\
		DEBUG_PUT(__VA_ARGS__) }						\
	}

#define DEBUG_IFS_PUT_METHOD(...) {		\
	if (sdebug)											\
	{													\
		DEBUG_PUT_COLOR(SerialDebug::COLOR_YELLOW, "%s: ", __FUNCTION__)\
		DEBUG_PUT(__VA_ARGS__) }						\
	}

class SerialDebug : public ITimer, public MessageSender, public Singleton<SerialDebug>{
private:
	virtual void timerHandler();
	static const unsigned int TIMER_PERIOD = 10;

	static const SERIAL_PORT PORT_NAME;
	static const SERIAL_PORT_SPEED PORT_SPEED;
	SerialPort* serialPort;
		
	static const unsigned int FIFO_SIZE = 1024 * 64;
	Fifo<char> *fifo;

public:
	SerialDebug();
	~SerialDebug();

	void put(char* str, ...);

	enum TEXT_ATTR{
		TEXT_ATTR_RESET = 0,
		TEXT_ATTR_BOLD = 1,
		TEXT_ATTR_DIM = 2,
		TEXT_ATTR_BLINK = 5
	};

	enum COLOR{
		COLOR_BLACK = 0,
		COLOR_RED = 1,
		COLOR_GREEN = 2,
		COLOR_YELLOW = 3,
		COLOR_BLUE = 4,
		COLOR_MAGENTA = 5,
		COLOR_CYAN = 6,
		COLOR_WHITE = 7,
		COLOR_DEFAULT = 9
	};

	#define DEBUG_SET_TEXT_ATTR(attr){DEBUG_PUT("\x1b[%um", attr);}
	#define DEBUG_SET_TEXT_COLOR(color){DEBUG_PUT("\x1b[3%um", color);}
	#define DEBUG_SET_BACKGROUND_COLOR(color){DEBUG_PUT("\x1b[4%um", color);}

	enum SERIAL_DEBUG_MESSAGE{
		SERIAL_DEBUG_MESSAGE_RECV_COMMAND = 49
	};
	enum COMMAND{
		COMMAND_GET_SETTINGS,
		COMMAND_DEBUG_CONFIG,
		COMMAND_DEBUG_LOGIC,
		COMMAND_DEBUG_ACTIONS_ON,
		COMMAND_DEBUG_ACTIONS_OFF,
		COMMAND_DEBUG_ACTIONS_NASADOK_ON_OFF,
		COMMAND_DEBUG_EXTENSION_SYSTEM,
		COMMAND_DEBUG_IOSUBSYSTEM,
		COMMAND_DEBUG_BK16_SET_INPUTS,
		COMMAND_DEBUG_BK16_RESET_INPUTS,
		COMMAND_DEBUG_MONITORINGSUBSYSTEM,
		COMMAND_DEBUG_RPKSUBSYSTEM,
		COMMAND_DEBUG_DETECTIONSUBSYSTEM,
		COMMAND_DEBUG_TOUCHPAD_CALIBRATION_ON,
		COMMAND_DEBUG_TOUCHPAD_CALIBRATION_CANCEL,
		COMMAND_DEBUG_TOUCHPAD_CALIBRATION_SAVE,
		COMMAND_DEBUG_TOUCHPAD_CALIBRATION_SET_LEFT_UP_POINT,
		COMMAND_DEBUG_TOUCHPAD_CALIBRATION_SET_RIGHT_DOWN_POINT,
		COMMAND_DEBUG_TOUCHPAD_CALIBRATION_GET_DATA,
		COMMAND_DEBUG_TEST_CONFIG_UPDATE
	};

private:
	void debugCommand(unsigned char command);

	bool silentDebugOn;
	bool debugOn;

	#define CTRL_KEY(c) (c - 0x60)

public:
	bool isOn();
	void on();
	void off();

private:
	bool debugInput;
public:
	void enableDebugInput(){
		debugInput = true;
	}

	void disableDebugInput(){
		debugInput = false;
	}
};

#endif
