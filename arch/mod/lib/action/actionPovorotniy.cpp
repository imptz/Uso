#include "actionPovorotniy.h"
#include "..\DEBUG\serialDebug.h"
#include "..\extension\subsystems\rpk\rpkSubsystem.h"

ActionPovorotniy::ActionPovorotniy()
{
}

ActionPovorotniy::ActionPovorotniy(unsigned char _deviceAddress)
	:	Action(_deviceAddress), phase(PHASE_COMMAND)
{
	DEBUG_PUT_METHOD("address = %i\n", deviceAddress)
}

ActionPovorotniy::~ActionPovorotniy()
{
	DEBUG_PUT_METHOD("address = %i\n", deviceAddress)
}

void ActionPovorotniy::timerHandler()
{
}

void ActionPovorotniy::step()
{
	unsigned char* pFrame = nullptr;
	unsigned char frame[20];

	IRpkDevice::FRAME_RESULT result;

	switch (phase)
	{
		case PHASE_COMMAND:
			frame[0] = deviceAddress;
			frame[1] = 0;
			frame[2] = RPK_COMMANDS_POVOROTNIY;
			frame[3] = 1;
			frame[4] = SUB_COMMAND_SVERNUT;
			
			DEBUG_PUT_METHOD("deviceAddress = %u PHASE_COMMAND frame = %u %u %u %u %u\n", deviceAddress, frame[0], frame[1], frame[2], frame[3], frame[4])

			frameId = RpkSubsystem::getSingleton().write(frame);
			if (frameId == IRpkDevice::BAD_FRAME_ID)
			{
				DEBUG_PUT_METHOD(" BAD_FRAME_ID\n")
				error();
			}
			else
			{
				DEBUG_PUT_METHOD(" done\n")
				phase = PHASE_COMMAND_WAIT;
			}
			break;
		case PHASE_COMMAND_WAIT:
			result = RpkSubsystem::getSingleton().read(frameId, &pFrame);
			SAFE_DELETE_ARRAY(pFrame)
			switch (result)
			{
				case IRpkDevice::FRAME_RESULT_ID_NOT_FOUND:
					DEBUG_PUT_METHOD("deviceAddress = %u PHASE_COMMAND_WAIT result = ", deviceAddress)
					DEBUG_PUT_COLOR(SerialDebug::COLOR_RED, "FRAME_RESULT_ID_NOT_FOUND\n")
					error();
					break;
				case IRpkDevice::FRAME_RESULT_ERROR:
					DEBUG_PUT_METHOD("deviceAddress = %u PHASE_COMMAND_WAIT result = ", deviceAddress)
					DEBUG_PUT_COLOR(SerialDebug::COLOR_RED, "FRAME_RESULT_ERROR\n")
					error();
					break;
				case IRpkDevice::FRAME_RESULT_READY:
					DEBUG_PUT_METHOD("deviceAddress = %u PHASE_COMMAND_WAIT result = ", deviceAddress)
					DEBUG_PUT_COLOR(SerialDebug::COLOR_GREEN, "FRAME_RESULT_READY\n")
					finish();
					break;
			}
			break;
	}
}

void ActionPovorotniy::error()
{
	state = STATE_ERROR;
	phase = PHASE_STOP;
}

void ActionPovorotniy::finish()
{
	state = STATE_READY;
	phase = PHASE_STOP;
}

void ActionPovorotniy::onMessage(Message message)
{
}
