#include "ActionStartProgramScanLine.h"
#include "../extension/subsystems/rpk/rpkSubsystem.h"

#include "../DEBUG/serialDebug.h"
#include "../extension/subsystems/monitoring/monitoringSubsystem.h"

ActionStartProgramScanLine::ActionStartProgramScanLine()
{
}

ActionStartProgramScanLine::ActionStartProgramScanLine(Point2<unsigned int> _point1, Point2<unsigned int> _point2, unsigned int _nasadok, 
	SCAN_PROGRAM_BALLISTICS _ballistic, unsigned char _deviceAddress)
	:	Action(_deviceAddress), phase(PHASE_COMMAND), point1(_point1), point2(_point2), nasadok(_nasadok), ballistic(_ballistic)
{
//	DEBUG_PUT_METHOD("address = %i\n", deviceAddress)
}

ActionStartProgramScanLine::~ActionStartProgramScanLine()
{
}

void ActionStartProgramScanLine::timerHandler()
{
	if (timeCounter < TEST_TIME_OUT)
		timeCounter++;
}

void ActionStartProgramScanLine::step()
{
	unsigned char* pFrame = nullptr;
	unsigned char frame[20];

	IRpkDevice::FRAME_RESULT result;

	unsigned int point1X = 360 - point1.x;
	unsigned int point2X = 360 - point2.x;

	switch (phase)
	{
		case PHASE_COMMAND:
			frame[0] = deviceAddress;
			frame[1] = 0;
			frame[2] = RPK_COMMANDS_START_SCAN_PROGRAM; 
			frame[3] = 12;
			frame[4] = SCAN_PROGRAM_STEP_LOW;
			frame[5] = point1X >> 8;
			frame[6] = point1X;
			frame[7] = point2X >> 8;
			frame[8] = point2X;
			frame[9] = point1.y >> 8;
			frame[10] = point1.y;
			frame[11] = point2.y >> 8;
			frame[12] = point2.y;
			frame[13] = ballistic;
			frame[14] = nasadok >> 8;
			frame[15] = nasadok;
			frameId = RpkSubsystem::getSingleton().write(frame);
			if (frameId == IRpkDevice::BAD_FRAME_ID)
				error();
			else
			{
				phase = PHASE_COMMAND_WAIT;

				unsigned char par2 = 0;
				unsigned char par3 = 0;
				unsigned char par4 = 0;

				par2 = (point1X & 0x0ff0) >> 4;
				par3 = (point1X & 0x000f) << 4;
				par3 |= (point2X & 0x0f00) >> 8;
				par4 = point2X & 0x00ff;

				MonitoringSubsystem::getSingleton().createAndSendMessage(IMonitoringDevice::MESSAGE_NUMBER_START_PROGRAMMI_SCANIROVANIJA, deviceAddress, par2, par3, par4);

				DEBUG_PUT_METHOD("address = %i, point1x = %i, point2x = %i, point1y = %i, point2y = %i\n", deviceAddress, point1X, point2X, point1.y, point2.y)
			}
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

void ActionStartProgramScanLine::onMessage(Message message)
{
}

void ActionStartProgramScanLine::error()
{
	state = STATE_ERROR;
	phase = PHASE_STOP;
}

void ActionStartProgramScanLine::finish()
{
	state = STATE_READY;
	phase = PHASE_STOP;
}
