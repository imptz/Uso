#pragma once

#include "../serialport/serialport.h"
#include "../timer/timer.h"
#include "../singleton.h"
#include "../message/messages.h"
#include "../debug/serialDebug.h"

class Touchpad : public ITimer, public Singleton<Touchpad>, public MessageSender, public MessageReceiver{
private:
	static const int TIMER_PERIOD = 5;
	static const SERIAL_PORT TOUCHPAD_SERIAL_PORT;
	static const SERIAL_PORT_SPEED TOUCHPAD_SERIAL_PORT_SPEED;

	virtual void timerHandler();
	unsigned int preXPos;
	unsigned int preYPos;
	unsigned int xPos;
	unsigned int yPos;

public:
	enum PEN_STATE{
		PEN_STATE_UP,
		PEN_STATE_DOWN
	};

private:
	PEN_STATE penState;

	enum TOUCH_COMMAND{
		TOUCH_COMMAND_PEN_UP = 0x00bf,
		TOUCH_COMMAND_PEN_DOWN = 0x00ff
	};

public:
	static const int TOUCHPAD_MESSAGE_FROM = MESSAGE_FROM_OFFSET_SYSTEM + 1;

	enum TOUCHPAD_MESSAGE{
		TOUCHPAD_MESSAGE_PEN_UP = 37,
		TOUCHPAD_MESSAGE_PEN_DOWN = 38
	};
		
public:
	Touchpad();
	virtual ~Touchpad();
	unsigned int get_xPos();
	unsigned int get_yPos();
	PEN_STATE getPenState();

private:
	void correctCoords(int x, int y);

public:
	void setCalibrationData(float leftTopPointX, float leftTopPointY, float rightBottomPointX, float rightBottomPointY);
		
	virtual void onMessage(Message message);
	private:
	bool calibrationMode;
	static const int SCREEN_WIDTH = 640;
	static const int SCREEN_HEIGHT = 400;
	struct CalibrationData{
		float leftTopPointX;
		float leftTopPointY;
		float rightBottomPointX;
		float rightBottomPointY;

		void clear(){
			leftTopPointX = 0.0f;
			leftTopPointY = 0.0f;
			rightBottomPointX = 0.0f;
			rightBottomPointY = 0.0f;
		}

		CalibrationData& operator=(CalibrationData& cd){
			leftTopPointX = cd.leftTopPointX;
			leftTopPointY = cd.leftTopPointY;
			rightBottomPointX = cd.rightBottomPointX;
			rightBottomPointY = cd.rightBottomPointY;

			return *this;
		}
	};

	CalibrationData calibrationData;
	CalibrationData preCalibrationData;

	bool saveCalibration();
	bool loadCalibration();

	static const unsigned int HDD_BUFFER_SIZE = ((sizeof(CalibrationData) / 512) + 1) * 512; 
	unsigned char hddBuffer[HDD_BUFFER_SIZE];
};
