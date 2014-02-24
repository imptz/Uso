#include "actionSendPressureTable.h"
#include "../extension/subsystems/rpk/rpkSubsystem.h"

ActionSendPressureTable::ActionSendPressureTable()
{
}

ActionSendPressureTable::ActionSendPressureTable(Fire::FireScanProgram* _program)
	:	Action(_program->prNumber), program(_program), phase(PHASE_COMMAND)
{
}

ActionSendPressureTable::~ActionSendPressureTable()
{
}

void ActionSendPressureTable::timerHandler()
{
}

void ActionSendPressureTable::step()
{
	unsigned char* pFrame = nullptr;
	unsigned char frame[20];

	IRpkDevice::FRAME_RESULT result;

	switch (phase)
	{
		case PHASE_COMMAND:
			frame[0] = deviceAddress;
			frame[1] = 0;
			frame[2] = RPK_COMMANDS_SET_PRESSURE_TABLE;
			frame[3] = 10;
			frame[4] = program->pressureTable0;
			frame[5] = program->pressureTable1;
			frame[6] = program->pressureTable2;
			frame[7] = program->pressureTable3;
			frame[8] = program->pressureTable4;
			frame[9] = program->pressureTable5;
			frame[10] = program->pressureTable6;
			frame[11] = program->pressureTable7;
			frame[12] = program->pressureTable8;
			frame[13] = program->pressureTable9;

DEBUG_PUT_METHOD(": ");
for(unsigned int i = 0; i < 10; ++i){
	DEBUG_PUT("%i ", frame[4 + i]);
}
DEBUG_PUT("\n");

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

void ActionSendPressureTable::onMessage(Message message)
{
}

void ActionSendPressureTable::error()
{
	state = STATE_ERROR;
	phase = PHASE_STOP;
}

void ActionSendPressureTable::finish()
{
	state = STATE_READY;
	phase = PHASE_STOP;
}

Fire::FireScanProgram* ActionSendPressureTable::getProgram()
{
	return program;
}
