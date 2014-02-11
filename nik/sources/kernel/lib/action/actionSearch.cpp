#include "actionSearch.h"
#include "../extension/subsystems/rpk/rpkSubsystem.h"

ActionSearch::ActionSearch()
{
}

ActionSearch::ActionSearch(unsigned char _deviceAddress, Point2<unsigned int> _point1, Point2<unsigned int> _point2, int _numberSourcesFlicker)
	:	Action(_deviceAddress), phase(PHASE_COMMAND), point1(_point1), point2(_point2), numberSourcesFlicker(_numberSourcesFlicker)
{
}

ActionSearch::~ActionSearch()
{
}

void ActionSearch::timerHandler()
{
	if (timeCounter < TEST_TIME_OUT)
		timeCounter++;
}

void ActionSearch::step()
{
	unsigned char* pFrame = nullptr;
	unsigned char frame[20];

	IRpkDevice::FRAME_RESULT result;

	switch (phase)
	{
		case PHASE_COMMAND:

			point1.x = 360 - point1.x;
			point2.x = 360 - point2.x;

			frame[0] = deviceAddress;
			frame[1] = 0;
			frame[2] = RPK_COMMANDS_START_SEARCH;
			frame[3] = 9;
			frame[4] = point1.x >> 8;
			frame[5] = point1.x;
			frame[6] = point2.x >> 8;
			frame[7] = point2.x;
			frame[8] = point1.y >> 8;
			frame[9] = point1.y;
			frame[10] = point2.y >> 8;
			frame[11] = point2.y;
			frame[12] = numberSourcesFlicker;

			point1.x = 360 - point1.x;
			point2.x = 360 - point2.x;

			frameId = RpkSubsystem::getSingleton().write(frame);
			if (frameId == IRpkDevice::BAD_FRAME_ID)
				error(1);
			else
				phase = PHASE_COMMAND_WAIT;
			break;
		case PHASE_COMMAND_WAIT:
			result = RpkSubsystem::getSingleton().read(frameId, &pFrame);
			SAFE_DELETE_ARRAY(pFrame)
			switch (result)
			{
				case IRpkDevice::FRAME_RESULT_ID_NOT_FOUND:
					error(2);
					break;
				case IRpkDevice::FRAME_RESULT_ERROR:
					error(3);
					break;
				case IRpkDevice::FRAME_RESULT_READY:
					phase = PHASE_TEST;
					break;
			}
			break;
		case PHASE_TEST:
			frame[0] = deviceAddress;
			frame[1] = 0;
			frame[2] = RPK_COMMANDS_FLAGS;
			frame[3] = 0;
			frameId = RpkSubsystem::getSingleton().write(frame);

//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1_UINT("PHASE_TEST_frameId: ", frameId);

			if (frameId == IRpkDevice::BAD_FRAME_ID)
				error(4);
			else
				phase = PHASE_TEST_WAIT;
			break;
		case PHASE_TEST_WAIT:
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1_UINT("PHASE_TEST_WAIT_frameId: ", frameId);
			result = RpkSubsystem::getSingleton().read(frameId, &pFrame);
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1_UINT("PHASE_TEST_WAIT_frameId_result: ", result);
			switch (result)
			{
				case IRpkDevice::FRAME_RESULT_ID_NOT_FOUND:
					error(5);
//DEBUG_PUT_METHOD("IRpkDevice::FRAME_RESULT_ID_NOT_FOUND\n");
					break;
				case IRpkDevice::FRAME_RESULT_ERROR:
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("PHASE_TEST_WAIT IRpkDevice::FRAME_RESULT_ERROR\n");
					error(6);
//DEBUG_PUT_METHOD("IRpkDevice::FRAME_RESULT_ERROR\n");
					break;
				case IRpkDevice::FRAME_RESULT_READY:
					if (pFrame[5] == 0)
					{
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("IRpkDevice::FRAME_RESULT_READY: ");
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_CHAR_ARRAY(pFrame, 20);
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("\n");
						if (pFrame[6] == 0){
							error(7);
						}
						else
						{
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("PHASE_TEST_WAIT\n");
							phase = PHASE_GET_RESULT;
//DEBUG_PUT_METHOD("phase = PHASE_GET_RESULT;\n");
						}
					}
					else
						if (timeCounter < TEST_TIME_OUT)
							phase = PHASE_TEST;
						else{
							error(8);
						}
					break;
			}

			SAFE_DELETE_ARRAY(pFrame)

			break;
		case PHASE_GET_RESULT:
			frame[0] = deviceAddress;
			frame[1] = 0;
			frame[2] = RPK_COMMANDS_GET_SEARCH_RESULT;
			frame[3] = 0;
			frameId = RpkSubsystem::getSingleton().write(frame);
			if (frameId == IRpkDevice::BAD_FRAME_ID)
				error(9);
			else
				phase = PHASE_GET_RESULT_WAIT;
			break;
		case PHASE_GET_RESULT_WAIT:
			result = RpkSubsystem::getSingleton().read(frameId, &pFrame);
			switch (result)
			{
				case IRpkDevice::FRAME_RESULT_ID_NOT_FOUND:
					error(10);
					break;
				case IRpkDevice::FRAME_RESULT_ERROR:
					error(11);
					break;
				case IRpkDevice::FRAME_RESULT_READY:
					resPoint1.x = (pFrame[5] << 8) + pFrame[6];
					resPoint1.y = (pFrame[7] << 8) + pFrame[8];
					resPoint2.x = (pFrame[9] << 8) + pFrame[10];
					resPoint2.y = (pFrame[11] << 8) + pFrame[12];

//UPDATE добавлено для инверсии горизонтальной координаты ПР
					resPoint1.x = 360 - resPoint1.x;
					resPoint2.x = 360 - resPoint2.x;

					DEBUG_PUT_METHOD("point1x = %i, point2x = %i, point1y = %i, point2y = %i\n", resPoint1.x, resPoint2.x, resPoint1.y, resPoint2.y)
					finish();
					break;
			}

			SAFE_DELETE_ARRAY(pFrame)

			break;
	}
}

void ActionSearch::onMessage(Message message)
{
}

void ActionSearch::error(int value)
{
	state = STATE_ERROR;
	phase = PHASE_STOP;
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("ActionSearch::error()\n");
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1_UINT("ActionSearch::error()_deviceAddress: ", getDeviceAddress());
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1_UINT("ActionSearch::error()_value:         ", value);
}

void ActionSearch::finish()
{
	state = STATE_READY;
	phase = PHASE_STOP;
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("ActionSearch::finish()\n");
}

bool ActionSearch::getResult(Point2<int>& _point1, Point2<int>& _point2)
{
	if (state == STATE_READY)
	{
		_point1.x = resPoint1.x;
		_point1.y = resPoint1.y;
		_point2.x = resPoint2.x;
		_point2.y = resPoint2.y;

		return true;
	}
	else
		return false;
}
