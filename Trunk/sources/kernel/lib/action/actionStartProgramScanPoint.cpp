#include "ActionStartProgramScanPoint.h"
#include "../extension/subsystems/rpk/rpkSubsystem.h"
#include "../log/Log.h"

ActionStartProgramScanPoint::ActionStartProgramScanPoint()
{
}

ActionStartProgramScanPoint::ActionStartProgramScanPoint(unsigned int _nPointProgram, unsigned char _deviceAddress)
	:	Action(_deviceAddress), phase(PHASE_COMMAND), nPointProgram(_nPointProgram)
{
}

ActionStartProgramScanPoint::~ActionStartProgramScanPoint()
{
}

void ActionStartProgramScanPoint::timerHandler()
{
	if (timeCounter < TEST_TIME_OUT)
		timeCounter++;
}

void ActionStartProgramScanPoint::step()
{
	unsigned char* pFrame = nullptr;
	unsigned char frame[20];

	IRpkDevice::FRAME_RESULT result;

	switch (phase)
	{
		case PHASE_COMMAND:
			frame[0] = deviceAddress;
			frame[1] = 0; 
			frame[2] = RPK_COMMANDS_START_SCAN_PROGRAM_OP; 
			frame[3] = 2;
			frame[4] = nPointProgram;
			frame[5] = (1 << SCAN_PROGRAM_FLAGS_NASADOK_OFF);
			frame[6] = 0;
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
			frame[2] = RPK_COMMANDS_FLAGS;
			frame[3] = 0;
			frame[4] = 0;
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
					if (pFrame[5] == 1)
						finish();
					else
						if (timeCounter < TEST_TIME_OUT)
							phase = PHASE_TEST;
						else
							error();
					break;
			}

			SAFE_DELETE_ARRAY(pFrame)

			break;
	}
}

void ActionStartProgramScanPoint::onMessage(Message message)
{
}

void ActionStartProgramScanPoint::error()
{
	state = STATE_ERROR;
	phase = PHASE_STOP;
}

void ActionStartProgramScanPoint::finish()
{
	state = STATE_READY;
	phase = PHASE_STOP;
}
