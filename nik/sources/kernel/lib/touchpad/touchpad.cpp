#include "touchpad.h"
#include "../fifo.h"
#include "../display/display.h"
#include "../hdd/hddManager.h"

const SERIAL_PORT Touchpad::TOUCHPAD_SERIAL_PORT = SERIAL_PORT_3;
const SERIAL_PORT_SPEED Touchpad::TOUCHPAD_SERIAL_PORT_SPEED = SERIAL_PORT_SPEED_19200;

Touchpad::Touchpad()
		:       ITimer(TIMER_PERIOD), xPos(0), yPos(0), penState(PEN_STATE_UP)
{
	calibrationMode = false;
	
	if (!loadCalibration()){
		calibrationData.leftTopPointX = 0;
		calibrationData.leftTopPointY = 0;
		calibrationData.rightBottomPointX = 0;
		calibrationData.rightBottomPointY = 0;
	}

	setCalibrationData(calibrationData.leftTopPointX,
		calibrationData.leftTopPointY,
		calibrationData.rightBottomPointX,
		calibrationData.rightBottomPointY);


	SerialDebug::getSingleton().addReceiver(this);

	SerialPortManager::getSingleton().getPort(TOUCHPAD_SERIAL_PORT)->setSpeed(TOUCHPAD_SERIAL_PORT_SPEED);
	SerialPortManager::getSingleton().getPort(TOUCHPAD_SERIAL_PORT)->open();    
	pTimer->start();
	unsigned char frame[5] = {0xf1, 0, 0, 0, 0};
	SerialPortManager::getSingleton().getPort(TOUCHPAD_SERIAL_PORT)->setNewSendData(frame, 5);
	SerialPortManager::getSingleton().getPort(TOUCHPAD_SERIAL_PORT)->startSend();
}

Touchpad::~Touchpad(){
	SerialPortManager::getSingleton().getPort(TOUCHPAD_SERIAL_PORT)->close();
}

void Touchpad::timerHandler(){
	static int gTimer = 0;

	if (gTimer++ > 100){
		if (penState == PEN_STATE_DOWN)
			sendMessage(Message(TOUCHPAD_MESSAGE_FROM, MESSAGE_TOUCHPAD_PEN_UP, xPos, yPos));
		penState = PEN_STATE_UP;
		gTimer = 0;
	}
	Fifo<unsigned char> *fifo = SerialPortManager::getSingleton().getPort(TOUCHPAD_SERIAL_PORT)->getRecvFifo();

	unsigned int viewSize = fifo->getDataSize();
	
	if (viewSize < 5)
		return;

	bool valid = false;
	unsigned char command = 0; 
	for (unsigned int i = 0; i < viewSize; i++){
		fifo->get(&command);
		if ((command == TOUCH_COMMAND_PEN_DOWN) || (command == TOUCH_COMMAND_PEN_UP)){
			valid = true;
			break;
		}
	}

	if (valid){
		gTimer = 0;
		unsigned char d, d1;

		if (command == TOUCH_COMMAND_PEN_DOWN){
			fifo->get(&d);
			preXPos = d;
			preXPos = preXPos << 7;
			fifo->get(&d);
			preXPos += d;
			
			fifo->get(&d);
			preYPos = d;
			preYPos = preYPos << 7;
			fifo->get(&d);
			preYPos += d;

			correctCoords(preXPos, preYPos);

			if (penState == PEN_STATE_UP)
				sendMessage(Message(TOUCHPAD_MESSAGE_FROM, MESSAGE_TOUCHPAD_PEN_DOWN, xPos, yPos));

			penState = PEN_STATE_DOWN;
		}else{
			fifo->get(&d);
			fifo->get(&d1);
			preXPos = d1;
			preXPos = preXPos << 7;
			preXPos += d;
			
			fifo->get(&d);
			fifo->get(&d1);
			preYPos = d1;
			preYPos = preYPos << 7;
			preYPos += d;
			correctCoords(preXPos, preYPos);

			if (penState == PEN_STATE_DOWN)
				sendMessage(Message(TOUCHPAD_MESSAGE_FROM, MESSAGE_TOUCHPAD_PEN_UP, xPos, yPos));

			penState = PEN_STATE_UP;
		}
	}
}

unsigned int Touchpad::get_xPos(){
	return xPos;
}

unsigned int Touchpad::get_yPos(){
	return yPos;
}

Touchpad::PEN_STATE Touchpad::getPenState(){
	return penState;
}
void Touchpad::setCalibrationData(float leftTopPointX, float leftTopPointY, float rightBottomPointX, float rightBottomPointY){
	calibrationData.leftTopPointX = leftTopPointX;
	calibrationData.leftTopPointY = leftTopPointY;
	calibrationData.rightBottomPointX = rightBottomPointX;
	calibrationData.rightBottomPointY = rightBottomPointY;
}

void Touchpad::correctCoords(int x, int y){
	if ((x >= 0) && (x < 1000) && (y >= 0) && (y < 1000)){
		xPos = static_cast<unsigned int>(static_cast<float>(SCREEN_WIDTH) - (static_cast<float>(x) - calibrationData.rightBottomPointX) * static_cast<float>(SCREEN_WIDTH) / 
			(calibrationData.leftTopPointX - calibrationData.rightBottomPointX) + 4.0f);
		yPos = static_cast<unsigned int>(static_cast<float>(SCREEN_HEIGHT) - (static_cast<float>(y) - calibrationData.rightBottomPointY) * static_cast<float>(SCREEN_HEIGHT) / 
			(calibrationData.leftTopPointY - calibrationData.rightBottomPointY) + 6.0f);
	}

	//char buf[100];
	//Display::getSingleton().printUInt(x, 3, 6, buf); 
	//Display::getSingleton().printUInt(y, 13, 6, buf); 
	//Display::getSingleton().printUInt(xPos, 3, 7, buf); 
	//Display::getSingleton().printUInt(yPos, 13, 7, buf); 
}

void Touchpad::onMessage(Message message){
	if (message.from == MESSAGE_FROM_OFFSET_SERIAL_DEBUG)
		if (message.msg == MESSAGE_SERIAL_DEBUG_RECV_COMMAND)
			switch (message.par1)
			{
				case SerialDebug::COMMAND_DEBUG_TOUCHPAD_CALIBRATION_ON:
					if (!calibrationMode){
						DEBUG_PUT_COLOR_ATTR(SerialDebug::COLOR_GREEN, SerialDebug::TEXT_ATTR_BOLD, "¬ключен режим калибровки сенсорной панели\n")
						calibrationMode = true;
						preCalibrationData.clear();
					}
					break;
				case SerialDebug::COMMAND_DEBUG_TOUCHPAD_CALIBRATION_CANCEL:
					if (calibrationMode){
						DEBUG_PUT_COLOR_ATTR(SerialDebug::COLOR_BLUE, SerialDebug::TEXT_ATTR_BOLD, "ќтменен режим калибровки сенсорной панели\n")
						calibrationMode = false;
					}
					break;
				case SerialDebug::COMMAND_DEBUG_TOUCHPAD_CALIBRATION_SAVE:
					if (calibrationMode){
						if (saveCalibration())
							DEBUG_PUT_COLOR_ATTR(SerialDebug::COLOR_GREEN, SerialDebug::TEXT_ATTR_BOLD, "ƒанные калибровки сенсорной панели сохранены, режим калибровки выключен\n")
						else
							DEBUG_PUT_COLOR_ATTR(SerialDebug::COLOR_RED, SerialDebug::TEXT_ATTR_BOLD, "ќшибка при сохранении данных калибровки сенсорной панели, режим калибровки выключен\n")
					
						calibrationMode = false;
					}
					break;
				case SerialDebug::COMMAND_DEBUG_TOUCHPAD_CALIBRATION_SET_LEFT_UP_POINT:
					if (calibrationMode){
						preCalibrationData.leftTopPointX = static_cast<float>(preXPos);
						preCalibrationData.leftTopPointY = static_cast<float>(preYPos);
						DEBUG_PUT_METHOD("”становлена лева€ верхн€€ точка при калибровке сенсорной панели: x = %f, y = %f\n", preCalibrationData.leftTopPointX, preCalibrationData.leftTopPointY)
					}
					break;
				case SerialDebug::COMMAND_DEBUG_TOUCHPAD_CALIBRATION_SET_RIGHT_DOWN_POINT:
					if (calibrationMode){
						DEBUG_PUT_METHOD("”становлена права€ нижн€€ точка при калибровке сенсорной панели: x = %f, y = %f\n", preCalibrationData.rightBottomPointX, preCalibrationData.rightBottomPointY)
						preCalibrationData.rightBottomPointX = static_cast<float>(preXPos);
						preCalibrationData.rightBottomPointY = static_cast<float>(preYPos);
					}
					break;
				case SerialDebug::COMMAND_DEBUG_TOUCHPAD_CALIBRATION_GET_DATA:
					if (calibrationMode){
						DEBUG_PUT("ƒанные калибровки сенсорной панели:\n")
						DEBUG_PUT("   лева€ верхн€€ (%f, %f), права€ нижн€€ (%f, %f)\n", calibrationData.leftTopPointX, calibrationData.leftTopPointY, calibrationData.rightBottomPointX, calibrationData.rightBottomPointY)
					}
					break;
			}
}

bool Touchpad::loadCalibration(){
	unsigned int _id = HddManager::getSingleton().read(hddBuffer, HddManager::SECTOR_OFFSET_TOUCHPAD, HDD_BUFFER_SIZE / 512);
	
	if (_id != HddManager::UNDEFINED_ID){
		int count = 10000000;
		while (HddManager::getSingleton().isTaskExecute(_id)){
			if (count == 0)
				break;
			else
				count--;
		}

		if (count != 0){
			memcpy(&calibrationData, hddBuffer, sizeof(calibrationData));
			return true;
		}
	}

	return false;
}

bool Touchpad::saveCalibration(){
	calibrationData = preCalibrationData;

	memcpy(hddBuffer, &preCalibrationData, sizeof(preCalibrationData));

	unsigned int _id = HddManager::getSingleton().write(hddBuffer, HddManager::SECTOR_OFFSET_TOUCHPAD, HDD_BUFFER_SIZE / 512);

	if (_id != HddManager::UNDEFINED_ID){
		int count = 10000000;
		while (HddManager::getSingleton().isTaskExecute(_id)){
			if (count == 0)
				break;
			else
				count--;
		}

		if (count != 0)
			return true;
	}

	return false;
}
