#include "actionMoveToPoint.h"
#include "../extension/subsystems/rpk/rpkSubsystem.h"

ActionMoveToPoint::ActionMoveToPoint()
{
}

ActionMoveToPoint::ActionMoveToPoint(unsigned char _deviceAddress, Point2<unsigned int> _point, unsigned int _nasadok)
	:	Action(_deviceAddress), phase(PHASE_COMMAND), point(_point), nasadok(_nasadok)
{

}

ActionMoveToPoint::~ActionMoveToPoint()
{
}

void ActionMoveToPoint::timerHandler()
{
	if (timeCounter < TEST_TIME_OUT)
		timeCounter++;
}

void ActionMoveToPoint::step()
{
	unsigned char* pFrame = nullptr;
	unsigned char frame[20];

	unsigned int pointX = 360 - point.x;

	IRpkDevice::FRAME_RESULT result;

	switch (phase)
	{
		case PHASE_COMMAND:
			frame[0] = deviceAddress;
			frame[1] = 0;
			frame[2] = RPK_COMMANDS_MOVE_TO_POINT;
			frame[3] = 6;
			frame[4] = pointX >> 8;
			frame[5] = pointX;
			frame[6] = point.y >> 8;
			frame[7] = point.y;
			frame[8] = nasadok >> 8;
			frame[9] = nasadok;

//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("ActionMoveToPoint::step: ");
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_CHAR_ARRAY(frame, 10);
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("\n");

			frameId = RpkSubsystem::getSingleton().write(frame);
			if (frameId == IRpkDevice::BAD_FRAME_ID)
				error();
			else
				phase = PHASE_COMMAND_WAIT;
			break;
		case PHASE_COMMAND_WAIT:
			result = RpkSubsystem::getSingleton().read(frameId, &pFrame);
			SAFE_DELETE_ARRAY(pFrame)
			switch (result)
			{
				case IRpkDevice::FRAME_RESULT_ID_NOT_FOUND:
					error();
					break;
				case IRpkDevice::FRAME_RESULT_ERROR:
					error();
					break;
				case IRpkDevice::FRAME_RESULT_READY:
					phase = PHASE_TEST;
					break;
			}
			break;
		case PHASE_TEST:
			frame[0] = deviceAddress;
			frame[1] = 0;
			frame[2] = RPK_COMMANDS_GET_STATUS_MOVE_TO_POINT;
			frame[3] = 0;
			frameId = RpkSubsystem::getSingleton().write(frame);
			if (frameId == IRpkDevice::BAD_FRAME_ID)
				error();
			else
				phase = PHASE_TEST_WAIT;
			break;
		case PHASE_TEST_WAIT:
			result = RpkSubsystem::getSingleton().read(frameId, &pFrame);
			switch (result)
			{
				case IRpkDevice::FRAME_RESULT_ID_NOT_FOUND:
					error();
					break;
				case IRpkDevice::FRAME_RESULT_ERROR:
					error();
					break;
				case IRpkDevice::FRAME_RESULT_READY:
					switch (pFrame[5])
					{
						case 3:
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING3("IRpkDevice::FRAME_RESULT_READY: ", deviceAddress, pFrame[5], frameId);
							finish();
							break;
						case 2:
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING3("IRpkDevice::FRAME_RESULT_ERROR: ", deviceAddress, pFrame[5], frameId);
							error();
							break;
						default:
							if (timeCounter < TEST_TIME_OUT)
							{
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING3("IRpkDevice::FRAME_RESULT_TIME_OUT: ", deviceAddress, pFrame[5], frameId);
								phase = PHASE_TEST;
							}
							else
							{
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING3("IRpkDevice::FRAME_RESULT_TIME_OUT_ERROR: ", deviceAddress, pFrame[5], frameId);
								error();
							}
							break;
					}
			}

			SAFE_DELETE_ARRAY(pFrame)

			break;
	}
}

void ActionMoveToPoint::onMessage(Message message)
{
}

void ActionMoveToPoint::error()
{
	state = STATE_ERROR;
	phase = PHASE_STOP;
}

void ActionMoveToPoint::finish()
{
	state = STATE_READY;
	phase = PHASE_STOP;
}

Point2<unsigned int> ActionMoveToPoint::getPoint()
{
	return point;
}
