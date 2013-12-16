#include "ioSubsystem.h"
#include "../../../config/Config.h"
#include "../../../Local.h"

const char* IOSubsystem::MISSING_DEVICE_LOG_TEXT = LOCAL_IOSYSTEM_MISSING_DEVICE_TEXT;

bool IOSubsystem::addDevice(unsigned char id, unsigned char address)
{
	for (unsigned int i = 0; i < MAX_DEVICE_COUNT; i++)
	{
		if (devices[i].id == id)
		{
			for (unsigned int j = 0; j < MAX_DEVICE_COUNT; j++)
				if (pDevices[j] == nullptr)
				{
					pDevices[j] = devices[i].pCreateDevice(address, SUBSYSTEM_TYPE);
					return true;
				}

			return false;
		}
	}

	return false;
}

void IOSubsystem::testMissingDevices()
{
	for (unsigned int i = 0; i < MAX_DEVICE_COUNT; i++)
	{
		if (devices[i].pCreateDevice != nullptr)
		{
			bool res = false;
			for (unsigned int j = 0; j < MAX_DEVICE_COUNT; j++)
				if (pDevices[j] != nullptr)
					if (pDevices[j]->getId() == devices[i].id)
					{
						res = true;
						break;
					}
			
			if (!res)
				Log::getSingleton().add(LOG_MESSAGE_FROM_EXTENSION_SYSTEM, LOG_MESSAGE_TYPE_ERROR, LOCAL_IOSYSTEM_MISSING_DEVICE_TEXT, SUBSYSTEM_TYPE, devices[i].id);
		}
	}
}

IOSubsystem::IOSubsystem()
{
	for (unsigned int i = 0; i < MAX_DEVICE_COUNT; i++)
		pDevices[i] = nullptr;

	ExtensionSystem::getSingleton().regSubsystem(SUBSYSTEM_TYPE, this);

	pTimer->start();
}

IOSubsystem::~IOSubsystem()
{
	for (unsigned int i = 0; i < MAX_DEVICE_COUNT; i++)
		if (pDevices[i] != nullptr)
			SAFE_DELETE(pDevices[i])
}

void IOSubsystem::onMessage(Message message)
{

}

void IOSubsystem::timerHandler()
{
	for (unsigned int i = 0; i < MAX_DEVICE_COUNT; i++)
		if (pDevices[i] != nullptr)
			pDevices[i]->timerHandler();
}

void IOSubsystem::action()
{
	bool initResult = true;

	if (pDevices[deviceActionIndex] != nullptr)
		pDevices[deviceActionIndex++]->action();

	if ((deviceActionIndex == MAX_DEVICE_COUNT) || (pDevices[deviceActionIndex] == nullptr))
		deviceActionIndex = 0;

	for (unsigned int i = 0; i < MAX_DEVICE_COUNT; i++)
		if (pDevices[i] != nullptr)
		{
			if (phase == PHASE_INIT)
				if (!pDevices[i]->isReady())
					initResult = false;
		}
		
	if (phase == PHASE_INIT)
		if (initResult)
			phase = PHASE_START;
}

bool IOSubsystem::putFrame(unsigned char* _pArea, bool isNotTransfer)
{
	for (unsigned int i = 0; i < MAX_DEVICE_COUNT; i++)
		if (pDevices[i] != nullptr)
			if (pDevices[i]->putFrame(_pArea, isNotTransfer))
				return true;

	return false;
}

void IOSubsystem::init()
{
	bool isInit = false;
	for (unsigned int i = 0; i < MAX_DEVICE_COUNT; i++)
		if (pDevices[i] != nullptr)
		{
			isInit = true;
			phase = PHASE_INIT;
			pDevices[i]->init();
		}

	if (!isInit)
		phase = PHASE_START;

//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("IOSubsystem::init\n");	
}

bool IOSubsystem::isReady()
{
	return (phase == PHASE_START);
}

#pragma region ioSpecific

IIODevice::INPUT_STATE IOSubsystem::getInput(unsigned int number)
{
	IIODevice::INPUT_STATE value = IIODevice::INPUT_STATE_UNDEFINED;
	for (unsigned int i = 0; i < MAX_DEVICE_COUNT; i++)
		if (pDevices[i] != nullptr)
			if (pDevices[i]->getInput(number, &value))
				break;

	return value;
}

void IOSubsystem::setOutput(unsigned int number, IIODevice::OUTPUT_STATE value)
{
	for (unsigned int i = 0; i < MAX_DEVICE_COUNT; i++)
		if (pDevices[i] != nullptr)
			pDevices[i]->setOutput(number, value);
}

void IOSubsystem::setOutputs(unsigned int* numbers, unsigned int count, IIODevice::OUTPUT_STATE value)
{
	for (unsigned int i = 0; i < MAX_DEVICE_COUNT; i++)
		if (pDevices[i] != nullptr)
			pDevices[i]->setOutputs(numbers, count, value);
}

#pragma region customOutputSet
void IOSubsystem::enableAllFireAlarmOutputs()
{
	ConfigDataStructIOBk16** outputs = Config::getSingleton().getConfigData()->getConfigDataStructIOBk16();
	for (unsigned int i = 0; i < Config::getSingleton().getConfigData()->getConfigDataStructIOBk16Count(); i++)
		if (outputs[i]->outputFunctionGroup == ConfigDataStructIOBk16::OUTPUT_FUNCTION_GROUP_FIRE_ALARM)
		{
			setOutput((outputs[i]->bkAddress - 1) * 8 + outputs[i]->numberOnDevice - 1, IIODevice::OUTPUT_STATE_ON);
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING2("enableAllFireAlarmOutputs: ", i, outputs[i]->projectNumber); 
		}
}

void IOSubsystem::disableAllFireAlarmOutputs()
{
	ConfigDataStructIOBk16** outputs = Config::getSingleton().getConfigData()->getConfigDataStructIOBk16();
	for (unsigned int i = 0; i < Config::getSingleton().getConfigData()->getConfigDataStructIOBk16Count(); i++)
		if (outputs[i]->outputFunctionGroup == ConfigDataStructIOBk16::OUTPUT_FUNCTION_GROUP_FIRE_ALARM)
		{
			setOutput((outputs[i]->bkAddress - 1) * 8 + outputs[i]->numberOnDevice - 1, IIODevice::OUTPUT_STATE_OFF);
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING2("disableAllFireAlarmOutputs: ", i, outputs[i]->projectNumber); 
		}
}

void IOSubsystem::enableAllHardwareOutputs()
{
	ConfigDataStructIOBk16** outputs = Config::getSingleton().getConfigData()->getConfigDataStructIOBk16();
	for (unsigned int i = 0; i < Config::getSingleton().getConfigData()->getConfigDataStructIOBk16Count(); i++)
		if (outputs[i]->outputFunctionGroup == ConfigDataStructIOBk16::OUTPUT_FUNCTION_GROUP_HARDWARE)
		{
			setOutput((outputs[i]->bkAddress - 1) * 8 + outputs[i]->numberOnDevice - 1, IIODevice::OUTPUT_STATE_ON);
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING2("enableAllHardwareOutputs: ", i, outputs[i]->projectNumber); 
		}
}

void IOSubsystem::disableAllHardwareOutputs()
{
	ConfigDataStructIOBk16** outputs = Config::getSingleton().getConfigData()->getConfigDataStructIOBk16();
	for (unsigned int i = 0; i < Config::getSingleton().getConfigData()->getConfigDataStructIOBk16Count(); i++)
		if (outputs[i]->outputFunctionGroup == ConfigDataStructIOBk16::OUTPUT_FUNCTION_GROUP_HARDWARE)
		{
			setOutput((outputs[i]->bkAddress - 1) * 8 + outputs[i]->numberOnDevice - 1, IIODevice::OUTPUT_STATE_OFF);
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING2("disableAllHardwareOutputs: ", i, outputs[i]->projectNumber); 
		}
}

void IOSubsystem::enableAllPumpStationOutputs()
{
	ConfigDataStructIOBk16** outputs = Config::getSingleton().getConfigData()->getConfigDataStructIOBk16();
	for (unsigned int i = 0; i < Config::getSingleton().getConfigData()->getConfigDataStructIOBk16Count(); i++)
		if (outputs[i]->outputFunctionGroup == ConfigDataStructIOBk16::OUTPUT_FUNCTION_GROUP_PUMPING_STATION)
		{
			setOutput((outputs[i]->bkAddress - 1) * 8 + outputs[i]->numberOnDevice - 1, IIODevice::OUTPUT_STATE_ON);
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING2("enableAllPumpStationOutputs: ", i, outputs[i]->projectNumber); 
		}
}

void IOSubsystem::disableAllPumpStationOutputs()
{
	ConfigDataStructIOBk16** outputs = Config::getSingleton().getConfigData()->getConfigDataStructIOBk16();
	for (unsigned int i = 0; i < Config::getSingleton().getConfigData()->getConfigDataStructIOBk16Count(); i++)
		if (outputs[i]->outputFunctionGroup == ConfigDataStructIOBk16::OUTPUT_FUNCTION_GROUP_PUMPING_STATION)
		{
			setOutput((outputs[i]->bkAddress - 1) * 8 + outputs[i]->numberOnDevice - 1, IIODevice::OUTPUT_STATE_OFF);
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING2("disableAllPumpStationOutputs: ", i, outputs[i]->projectNumber); 
		}
}

void IOSubsystem::enablePrPressureOutputs(unsigned char number)
{
	ConfigDataStructIOBk16** outputs = Config::getSingleton().getConfigData()->getConfigDataStructIOBk16();
	for (unsigned int i = 0; i < Config::getSingleton().getConfigData()->getConfigDataStructIOBk16Count(); i++)
		if ((outputs[i]->outputFunctionGroup == ConfigDataStructIOBk16::OUTPUT_FUNCTION_GROUP_PR_PRESSURE_UP) && (outputs[i]->prGateNumber == number))
		{
			setOutput((outputs[i]->bkAddress - 1) * 8 + outputs[i]->numberOnDevice - 1, IIODevice::OUTPUT_STATE_ON);
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING2("enablePrPressureOutputs: ", i, outputs[i]->numberOnDevice); 
		}
}

void IOSubsystem::disablePrPressureOutputs(unsigned char number)
{
	ConfigDataStructIOBk16** outputs = Config::getSingleton().getConfigData()->getConfigDataStructIOBk16();
	for (unsigned int i = 0; i < Config::getSingleton().getConfigData()->getConfigDataStructIOBk16Count(); i++)
		if ((outputs[i]->outputFunctionGroup == ConfigDataStructIOBk16::OUTPUT_FUNCTION_GROUP_PR_PRESSURE_UP) && (outputs[i]->prGateNumber == number))
		{
			setOutput((outputs[i]->bkAddress - 1) * 8 + outputs[i]->numberOnDevice - 1, IIODevice::OUTPUT_STATE_OFF);
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING2("disablePrPressureOutputs: ", i, outputs[i]->numberOnDevice); 
		}
}

void IOSubsystem::enableGateOutputs(unsigned char number)
{
	ConfigDataStructIOBk16** outputs = Config::getSingleton().getConfigData()->getConfigDataStructIOBk16();
	for (unsigned int i = 0; i < Config::getSingleton().getConfigData()->getConfigDataStructIOBk16Count(); i++)
		if ((outputs[i]->outputFunctionGroup == ConfigDataStructIOBk16::OUTPUT_FUNCTION_GROUP_GATE_OPEN) && (outputs[i]->prGateNumber == number))
		{
			setOutput((outputs[i]->bkAddress - 1) * 8 + outputs[i]->numberOnDevice - 1, IIODevice::OUTPUT_STATE_ON);
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING2("enableGateOutputs: ", i, outputs[i]->numberOnDevice); 
		}
}

void IOSubsystem::disableGateOutputs(unsigned char number)
{
	ConfigDataStructIOBk16** outputs = Config::getSingleton().getConfigData()->getConfigDataStructIOBk16();
	for (unsigned int i = 0; i < Config::getSingleton().getConfigData()->getConfigDataStructIOBk16Count(); i++)
		if ((outputs[i]->outputFunctionGroup == ConfigDataStructIOBk16::OUTPUT_FUNCTION_GROUP_GATE_OPEN) && (outputs[i]->prGateNumber == number))
		{
			setOutput((outputs[i]->bkAddress - 1) * 8 + outputs[i]->numberOnDevice - 1, IIODevice::OUTPUT_STATE_OFF);
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING2("disableGateOutputs: ", i, outputs[i]->numberOnDevice); 
		}
}

void IOSubsystem::enableSystemFaultOutputs()
{
	ConfigDataStructIOBk16** outputs = Config::getSingleton().getConfigData()->getConfigDataStructIOBk16();
	for (unsigned int i = 0; i < Config::getSingleton().getConfigData()->getConfigDataStructIOBk16Count(); i++)
		if (outputs[i]->outputFunctionGroup == ConfigDataStructIOBk16::OUTPUT_FUNCTION_GROUP_SYSTEM_FAULT)
		{
			setOutput((outputs[i]->bkAddress - 1) * 8 + outputs[i]->numberOnDevice - 1, IIODevice::OUTPUT_STATE_ON);
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING2("enableSystemFaultOutputs: ", i, outputs[i]->numberOnDevice); 
		}
}

void IOSubsystem::disableSystemFaultOutputs()
{
	ConfigDataStructIOBk16** outputs = Config::getSingleton().getConfigData()->getConfigDataStructIOBk16();
	for (unsigned int i = 0; i < Config::getSingleton().getConfigData()->getConfigDataStructIOBk16Count(); i++)
		if (outputs[i]->outputFunctionGroup == ConfigDataStructIOBk16::OUTPUT_FUNCTION_GROUP_SYSTEM_FAULT)
		{
			setOutput((outputs[i]->bkAddress - 1) * 8 + outputs[i]->numberOnDevice - 1, IIODevice::OUTPUT_STATE_OFF);
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING2("disableSystemFaultOutputs: ", i, outputs[i]->numberOnDevice); 
		}
}

void IOSubsystem::enableGateFaultOutputs(unsigned char number)
{
	ConfigDataStructIOBk16** outputs = Config::getSingleton().getConfigData()->getConfigDataStructIOBk16();
	for (unsigned int i = 0; i < Config::getSingleton().getConfigData()->getConfigDataStructIOBk16Count(); i++)
		if ((outputs[i]->outputFunctionGroup == ConfigDataStructIOBk16::OUTPUT_FUNCTION_GROUP_GATE_FAULT) && (outputs[i]->prGateNumber == number))
		{
			setOutput((outputs[i]->bkAddress - 1) * 8 + outputs[i]->numberOnDevice - 1, IIODevice::OUTPUT_STATE_ON);
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING2("enableGateFaultOutputs: ", i, outputs[i]->numberOnDevice); 
		}
}

void IOSubsystem::disableGateFaultOutputs(unsigned char number)
{
	ConfigDataStructIOBk16** outputs = Config::getSingleton().getConfigData()->getConfigDataStructIOBk16();
	for (unsigned int i = 0; i < Config::getSingleton().getConfigData()->getConfigDataStructIOBk16Count(); i++)
		if ((outputs[i]->outputFunctionGroup == ConfigDataStructIOBk16::OUTPUT_FUNCTION_GROUP_GATE_FAULT) && (outputs[i]->prGateNumber == number))
		{
			setOutput((outputs[i]->bkAddress - 1) * 8 + outputs[i]->numberOnDevice - 1, IIODevice::OUTPUT_STATE_OFF);
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING2("disableGateFaultOutputs: ", i, outputs[i]->numberOnDevice); 
		}
}

void IOSubsystem::enablePrFaultOutputs(unsigned char number)
{
	ConfigDataStructIOBk16** outputs = Config::getSingleton().getConfigData()->getConfigDataStructIOBk16();
	for (unsigned int i = 0; i < Config::getSingleton().getConfigData()->getConfigDataStructIOBk16Count(); i++)
		if ((outputs[i]->outputFunctionGroup == ConfigDataStructIOBk16::OUTPUT_FUNCTION_GROUP_PR_FAULT) && (outputs[i]->prGateNumber == number))
		{
			setOutput((outputs[i]->bkAddress - 1) * 8 + outputs[i]->numberOnDevice - 1, IIODevice::OUTPUT_STATE_ON);
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING2("enablePrFaultOutputs: ", i, outputs[i]->numberOnDevice); 
		}
}

void IOSubsystem::disablePrFaultOutputs(unsigned char number)
{
	ConfigDataStructIOBk16** outputs = Config::getSingleton().getConfigData()->getConfigDataStructIOBk16();
	for (unsigned int i = 0; i < Config::getSingleton().getConfigData()->getConfigDataStructIOBk16Count(); i++)
		if ((outputs[i]->outputFunctionGroup == ConfigDataStructIOBk16::OUTPUT_FUNCTION_GROUP_PR_FAULT) && (outputs[i]->prGateNumber == number))
		{
			setOutput((outputs[i]->bkAddress - 1) * 8 + outputs[i]->numberOnDevice - 1, IIODevice::OUTPUT_STATE_OFF);
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING2("disablePrFaultOutputs: ", i, outputs[i]->numberOnDevice); 
		}
}

#pragma endregion 
#pragma endregion 

void IOSubsystem::bkZatvor(int number, int action){
	ConfigDataStructIOBk16** outputs = Config::getSingleton().getConfigData()->getConfigDataStructIOBk16();
	for (unsigned int i = 0; i < Config::getSingleton().getConfigData()->getConfigDataStructIOBk16Count(); i++)
		if ((outputs[i]->outputFunctionGroup == ConfigDataStructIOBk16::OUTPUT_FUNCTION_GROUP_BK_ZATVOR) && (outputs[i]->prGateNumber == number)){
			if (action == 0)
				setOutput((outputs[i]->bkAddress - 1) * 8 + outputs[i]->numberOnDevice - 1, IIODevice::OUTPUT_STATE_OFF);
			else
				setOutput((outputs[i]->bkAddress - 1) * 8 + outputs[i]->numberOnDevice - 1, IIODevice::OUTPUT_STATE_ON);
		}
}
