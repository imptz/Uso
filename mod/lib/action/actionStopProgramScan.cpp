#include "actionStopProgramScan.h"
#include "../extension/subsystems/rpk/rpkSubsystem.h"
#include "../log/Log.h"

ActionStopProgramScan::ActionStopProgramScan()
{
}

ActionStopProgramScan::ActionStopProgramScan(unsigned char _deviceAddress)
	:	Action(_deviceAddress), phase(PHASE_COMMAND)
{
}

ActionStopProgramScan::~ActionStopProgramScan()
{
}

void ActionStopProgramScan::timerHandler()
{
}

void ActionStopProgramScan::step()
{
	unsigned char* pFrame = nullptr;
	unsigned char frame[20];

	IRpkDevice::FRAME_RESULT result;

	switch (phase)
	{
		case PHASE_COMMAND:
			frame [0] = deviceAddress;
			frame [1] = 0;
			frame [2] = RPK_COMMANDS_STOP_SCAN_PROGRAM;
			frame [3] = 0;
			frame [4] = 0;
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
					finish();
					break;
			}
			break;
	}
}

void ActionStopProgramScan::onMessage(Message message)
{
}

void ActionStopProgramScan::error()
{
	state = STATE_ERROR;
	phase = PHASE_STOP;
}

void ActionStopProgramScan::finish()
{
	state = STATE_READY;
	phase = PHASE_STOP;
}
