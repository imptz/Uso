#include "MainInfo.h"
#include "../log/Log.h"
#include "../low.h"

#include "../extension/ExtensionSystem.h"
#include "../extension/subsystems/io/ioSubsystem.h"
#include "../extension/subsystems/detection/DetectionSubsystem.h"
#include "../extension/subsystems/monitoring/MonitoringSubsystem.h"
#include "../extension/subsystems/rpk/RpkSubsystem.h"
#include "../extension/devices/scannerDevice.h"
#include "../Config/Config.h"
#include "Button.h"
#include "../DEBUG/serialDebug.h"

//Button* b0;
//Button* b2;
//Button* b3;
//Button* b4;
//Button* b5;
//Button* b6;
//Button* b7;
//Button* b8;
//Button* b9;
//Button* b10;
//Button* b11;
//Button* b12;
//Button* b13;

MainInfo::MainInfo(unsigned int _positionX, unsigned int _positionY, MessageReceiver* _messageReceiver)
	:	Control(_positionX, _positionY, WIDTH, HEIGHT, _messageReceiver)
{ 
//b0 = new Button(_positionX + 1, _positionY + 2, 10, 3, "Ust", Window::BORDER_STYLE_SINGLE, this);
//b2 = new Button(_positionX + 21, _positionY + 2, 10, 3, "DI", Window::BORDER_STYLE_SINGLE, this);
//b3 = new Button(_positionX + 31, _positionY + 2, 10, 3, "ExtBus", Window::BORDER_STYLE_SINGLE, this);
//
//b4 = new Button(_positionX + 1, _positionY + 6, 10, 3, "SetOutput", Window::BORDER_STYLE_SINGLE, this);
//b5 = new Button(_positionX + 11, _positionY + 6, 10, 3, "GetInput", Window::BORDER_STYLE_SINGLE, this);
//b6 = new Button(_positionX + 22, _positionY + 6, 10, 3, "rpk write", Window::BORDER_STYLE_SINGLE, this);
//b7 = new Button(_positionX + 33, _positionY + 6, 10, 3, "rpk read", Window::BORDER_STYLE_SINGLE, this);
//b8 = new Button(_positionX + 44, _positionY + 6, 10, 3, "start ps", Window::BORDER_STYLE_SINGLE, this);
//b9 = new Button(_positionX + 55, _positionY + 6, 10, 3, "stop ps", Window::BORDER_STYLE_SINGLE, this);
//b10 = new Button(_positionX + 66, _positionY + 6, 10, 3, "uso info", Window::BORDER_STYLE_SINGLE, this);
//
//b11 = new Button(_positionX + 41, _positionY + 2, 10, 3, "zt open", Window::BORDER_STYLE_SINGLE, this);
//b12 = new Button(_positionX + 51, _positionY + 2, 10, 3, "zt close", Window::BORDER_STYLE_SINGLE, this);
//b13 = new Button(_positionX + 61, _positionY + 2, 10, 3, "zt info", Window::BORDER_STYLE_SINGLE, this);
//
//addChildControl(b0);
//addChildControl(b2);
//addChildControl(b3);
//addChildControl(b4);
//addChildControl(b5);
//addChildControl(b6);
//addChildControl(b7);
//addChildControl(b8);
//addChildControl(b9);
//addChildControl(b10);
//addChildControl(b11);
//addChildControl(b12);
//addChildControl(b13);
draw();
}

MainInfo::~MainInfo()
{

}

void MainInfo::draw()
{
	drawChildControls();
}

void MainInfo::onMessage(Message message)
{
//if ((message.from == MESSAGE_FROM_OFFSET_CONTROLS + b0->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
//{
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("___ScannerDevice count: ", DetectionSubsystem::getSingleton().pDevices[0]->devicesCount);
//	for (int i = 0; i < DetectionSubsystem::getSingleton().pDevices[0]->devicesCount; i++)
//	{
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("______ScannerDevice: ", DetectionSubsystem::getSingleton().pDevices[0]->pDevices[i].address);
//	}
//}
//
//if ((message.from == MESSAGE_FROM_OFFSET_CONTROLS + b2->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
//{
//	SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("ConfigDataStructConst\n");
//	SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("  year: ", Config::getSingleton().getConfigData()->getConfigDataStructConst()->year);
//	SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("  maxPR: ", Config::getSingleton().getConfigData()->getConfigDataStructConst()->maxPR);
//	SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("  timeOutBeforeStart: ", Config::getSingleton().getConfigData()->getConfigDataStructConst()->timeOutBeforeStart);
//	SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("  timeOutBeforeFinish: ", Config::getSingleton().getConfigData()->getConfigDataStructConst()->timeOutBeforeFinish);
//	SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("  numberFireToAnalize: ", Config::getSingleton().getConfigData()->getConfigDataStructConst()->numberFireToAnalize);
//	SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("  minimumDistanceForCompactJet: ", Config::getSingleton().getConfigData()->getConfigDataStructConst()->minimumDistanceForCompactJet);
//	SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("  permissionTesting: ", Config::getSingleton().getConfigData()->getConfigDataStructConst()->permissionTesting);
//	SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("  testingHour: ", Config::getSingleton().getConfigData()->getConfigDataStructConst()->testingHour);
//	SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("  testingMinute: ", Config::getSingleton().getConfigData()->getConfigDataStructConst()->testingMinute);
//	SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("  permissionTestingInfo: ", Config::getSingleton().getConfigData()->getConfigDataStructConst()->permissionTestingInfo);
//	SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING3("  protectedZone: ", Config::getSingleton().getConfigData()->getConfigDataStructConst()->protectedZone.x,
//		Config::getSingleton().getConfigData()->getConfigDataStructConst()->protectedZone.y,
//		Config::getSingleton().getConfigData()->getConfigDataStructConst()->protectedZone.z);
//	if (Config::getSingleton().getConfigData()->getConfigDataStructConst()->tv)
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("  tv: TRUE\n");
//	else
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("  tv: FALSE\n");
//
//	SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("ConfigDataStructPRPosition\n");
//	SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("  Count: ", Config::getSingleton().getConfigData()->getConfigDataStructPRPositionCount());
//	for (unsigned int i = 0; i < Config::getSingleton().getConfigData()->getConfigDataStructPRPositionCount(); i++)
//	{
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("    number: ", i);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("      projectNumber: ", Config::getSingleton().getConfigData()->getConfigDataStructPRPositions()[i]->projectNumber);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("      address: ", Config::getSingleton().getConfigData()->getConfigDataStructPRPositions()[i]->address);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING3("      position: ", Config::getSingleton().getConfigData()->getConfigDataStructPRPositions()[i]->position.x,
//			Config::getSingleton().getConfigData()->getConfigDataStructPRPositions()[i]->position.y,
//			Config::getSingleton().getConfigData()->getConfigDataStructPRPositions()[i]->position.z);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING2("      orientation: ", Config::getSingleton().getConfigData()->getConfigDataStructPRPositions()[i]->orientation.x,
//			Config::getSingleton().getConfigData()->getConfigDataStructPRPositions()[i]->orientation.y);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("	  networkIndexNumber: ", Config::getSingleton().getConfigData()->getConfigDataStructPRPositions()[i]->networkIndexNumber);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING3("      axis: ", Config::getSingleton().getConfigData()->getConfigDataStructPRPositions()[i]->axis.x,
//			Config::getSingleton().getConfigData()->getConfigDataStructPRPositions()[i]->axis.y,
//			Config::getSingleton().getConfigData()->getConfigDataStructPRPositions()[i]->axis.z);
//	}
//
//	SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("ConfigDataStructIOBk16\n");
//	SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("  Count: ", Config::getSingleton().getConfigData()->getConfigDataStructIOBk16Count());
//	for (unsigned int i = 0; i < Config::getSingleton().getConfigData()->getConfigDataStructIOBk16Count(); i++)
//	{
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("    number: ", i);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("      bkAddress: ", Config::getSingleton().getConfigData()->getConfigDataStructIOBk16()[i]->bkAddress);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("      numberOnDevice: ", Config::getSingleton().getConfigData()->getConfigDataStructIOBk16()[i]->numberOnDevice);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("      outputFunctionGroup: ", Config::getSingleton().getConfigData()->getConfigDataStructIOBk16()[i]->outputFunctionGroup);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("      prGateNumber: ", Config::getSingleton().getConfigData()->getConfigDataStructIOBk16()[i]->prGateNumber);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("      inputFunctionGroup: ", Config::getSingleton().getConfigData()->getConfigDataStructIOBk16()[i]->inputFunctionGroup);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("      projectNumber: ", Config::getSingleton().getConfigData()->getConfigDataStructIOBk16()[i]->projectNumber);
//	}
//
//	SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("ConfigDataStructIOSerial\n");
//	SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("  Count: ", Config::getSingleton().getConfigData()->getConfigDataStructIOSerialCount());
//	for (unsigned int i = 0; i < Config::getSingleton().getConfigData()->getConfigDataStructIOSerialCount(); i++)
//	{
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("    number: ", i);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("      address: ", Config::getSingleton().getConfigData()->getConfigDataStructIOSerial()[i]->address);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("      normalState: ", Config::getSingleton().getConfigData()->getConfigDataStructIOSerial()[i]->normalState);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("      projectNumber: ", Config::getSingleton().getConfigData()->getConfigDataStructIOSerial()[i]->projectNumber);
//	}
//
//	SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("ConfigDataStructInitSignal\n");
//	SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("  Count: ", Config::getSingleton().getConfigData()->getConfigDataStructInitSignalsCount());
//	for (unsigned int i = 0; i < Config::getSingleton().getConfigData()->getConfigDataStructInitSignalsCount(); i++)
//	{
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("    number: ", i);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("      function: ", Config::getSingleton().getConfigData()->getConfigDataStructInitSignals()[i]->function);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("      number: ", Config::getSingleton().getConfigData()->getConfigDataStructInitSignals()[i]->number);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("      firstInputNumber: ", Config::getSingleton().getConfigData()->getConfigDataStructInitSignals()[i]->firstInputNumber);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("      secondInputNumber: ", Config::getSingleton().getConfigData()->getConfigDataStructInitSignals()[i]->secondInputNumber);
//	}
//
//	SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("ConfigDataStructProgram\n");
//	SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("  Count: ", Config::getSingleton().getConfigData()->getConfigDataStructProgramCount());
//	for (unsigned int i = 0; i < Config::getSingleton().getConfigData()->getConfigDataStructProgramCount(); i++)
//	{
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("    number: ", i);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("      initSignalNumber: ", Config::getSingleton().getConfigData()->getConfigDataStructPrograms()[i]->initSignalNumber);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("      prNumber: ", Config::getSingleton().getConfigData()->getConfigDataStructPrograms()[i]->prNumber);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("      function: ", Config::getSingleton().getConfigData()->getConfigDataStructPrograms()[i]->function);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING2("      point1: ", Config::getSingleton().getConfigData()->getConfigDataStructPrograms()[i]->point1.x,
//			Config::getSingleton().getConfigData()->getConfigDataStructPrograms()[i]->point1.y);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING2("      point2: ", Config::getSingleton().getConfigData()->getConfigDataStructPrograms()[i]->point2.x,
//			Config::getSingleton().getConfigData()->getConfigDataStructPrograms()[i]->point2.y);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("      nPointProgram: ", Config::getSingleton().getConfigData()->getConfigDataStructPrograms()[i]->nPointProgram);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("      nasadok: ", Config::getSingleton().getConfigData()->getConfigDataStructPrograms()[i]->nasadok);
//	}
//
//	SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("ConfigDataStructFv300\n");
//	SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("  Count: ", Config::getSingleton().getConfigData()->getConfigDataStructFv300Count());
//	for (unsigned int i = 0; i < Config::getSingleton().getConfigData()->getConfigDataStructFv300Count(); i++)
//	{
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("    number: ", i);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("      address: ", Config::getSingleton().getConfigData()->getConfigDataStructFv300()[i]->address);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("      prNumber: ", Config::getSingleton().getConfigData()->getConfigDataStructFv300()[i]->prNumber);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("      projectNumber: ", Config::getSingleton().getConfigData()->getConfigDataStructFv300()[i]->projectNumber);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING3("      position: ", Config::getSingleton().getConfigData()->getConfigDataStructFv300()[i]->position.x,
//			Config::getSingleton().getConfigData()->getConfigDataStructFv300()[i]->position.y, Config::getSingleton().getConfigData()->getConfigDataStructFv300()[i]->position.z);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING2("      orientation: ", Config::getSingleton().getConfigData()->getConfigDataStructFv300()[i]->orientation.x,
//			Config::getSingleton().getConfigData()->getConfigDataStructFv300()[i]->orientation.y);
//	}
//
//	SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("ConfigDataStructTrajectory\n");
//	SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("  Count: ", Config::getSingleton().getConfigData()->getConfigDataStructTrajectoryCount());
//	for (unsigned int i = 0; i < Config::getSingleton().getConfigData()->getConfigDataStructTrajectoryCount(); i++)
//	{
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("    number: ", i);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("      prNumber: ", Config::getSingleton().getConfigData()->getConfigDataStructTrajectory()[i]->prNumber);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("      trajectoryNumber: ", Config::getSingleton().getConfigData()->getConfigDataStructTrajectory()[i]->trajectoryNumber);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("      pointNumber: ", Config::getSingleton().getConfigData()->getConfigDataStructTrajectory()[i]->pointNumber);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING2("      position: ", Config::getSingleton().getConfigData()->getConfigDataStructTrajectory()[i]->position.x,
//			Config::getSingleton().getConfigData()->getConfigDataStructTrajectory()[i]->position.y);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("      nasadok: ", Config::getSingleton().getConfigData()->getConfigDataStructTrajectory()[i]->nasadok);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("      pressureNumber: ", Config::getSingleton().getConfigData()->getConfigDataStructTrajectory()[i]->pressureNumber);
//	}
//
//	SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("ConfigDataStructPressure\n");
//	SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("  Count: ", Config::getSingleton().getConfigData()->getConfigDataStructPressureCount());
//	for (unsigned int i = 0; i < Config::getSingleton().getConfigData()->getConfigDataStructPressureCount(); i++)
//	{
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("    number: ", i);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("      prNumber: ", Config::getSingleton().getConfigData()->getConfigDataStructPressure()[i]->prNumber);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("      arNumber: ", Config::getSingleton().getConfigData()->getConfigDataStructPressure()[i]->arNumber);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("      pressure: ", Config::getSingleton().getConfigData()->getConfigDataStructPressure()[i]->pressure);
//		SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("      delta: ", Config::getSingleton().getConfigData()->getConfigDataStructPressure()[i]->delta);
//	}
//
//}
//if ((message.from == MESSAGE_FROM_OFFSET_CONTROLS + b3->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
//{
//	ExtensionSystem::getSingleton().init();
//	//IOSubsystem::getSingleton().printRegDev();
//	//DetectionSubsystem::getSingleton().printRegDev();
//	//MonitoringSubsystem::getSingleton().printRegDev();
//	ExtensionSystem::getSingleton().printRegSubsystem();
//}
//if ((message.from == MESSAGE_FROM_OFFSET_CONTROLS + b4->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
//{
//	unsigned int opt[4] = {1,8,9,21};
//	IOSubsystem::getSingleton().setOutputs(opt, 4, IIODevice::OUTPUT_STATE_ON);
//}
//if ((message.from == MESSAGE_FROM_OFFSET_CONTROLS + b5->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
//{
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING2("Input: ", 1, IOSubsystem::getSingleton().getInput(1));
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING2("Input: ", 9, IOSubsystem::getSingleton().getInput(9));
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING2("Input: ", 23, IOSubsystem::getSingleton().getInput(23));
//}
//
//if ((message.from == MESSAGE_FROM_OFFSET_CONTROLS + b6->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
//{
//	unsigned char frame[5] = {2,31,19,0,12};
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("RPK Button write: ", RpkSubsystem::getSingleton().write(frame));
//}
//
//if ((message.from == MESSAGE_FROM_OFFSET_CONTROLS + b7->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
//{
//	unsigned char* fr = nullptr;
//	SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("RPK Button read: ", RpkSubsystem::getSingleton().read(2, &fr));
//	SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("  ");
//	SERIAL_DEBUG_ADD_DEBUG_MESSAGE_CHAR_ARRAY(fr, 20);
//	SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING("\n");
//	delete[] fr;
//}
//
//if ((message.from == MESSAGE_FROM_OFFSET_CONTROLS + b8->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
//{
//	unsigned char frame[5] = {1,39,20,0,12};
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("RPK Button start ps: ", RpkSubsystem::getSingleton().write(frame));
//}
//
//if ((message.from == MESSAGE_FROM_OFFSET_CONTROLS + b9->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
//{
//	unsigned char frame[5] = {1,39,22,0,12};
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("RPK Button stop ps: ", RpkSubsystem::getSingleton().write(frame));
//}
//
//if ((message.from == MESSAGE_FROM_OFFSET_CONTROLS + b10->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
//{
//	unsigned char frame[5] = {1,39,21,0,12};
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("RPK Button get uso info: ", RpkSubsystem::getSingleton().write(frame));
//}
//
//if ((message.from == MESSAGE_FROM_OFFSET_CONTROLS + b11->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
//{
//	unsigned char frame[5] = {1,39,9,0,12};
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("RPK Button zatvor open: ", RpkSubsystem::getSingleton().write(frame));
//}
//
//if ((message.from == MESSAGE_FROM_OFFSET_CONTROLS + b12->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
//{
//	unsigned char frame[5] = {1,39,10,0,12};
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("RPK Button zatvor cloae: ", RpkSubsystem::getSingleton().write(frame));
//}
//
//if ((message.from == MESSAGE_FROM_OFFSET_CONTROLS + b13->getId()) && (message.msg == Button::BUTTON_MESSAGE_UP))
//{
//	unsigned char frame[5] = {1,39,158,0,12};
//SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("RPK Button zatvor get info: ", RpkSubsystem::getSingleton().write(frame));
//}
//
}
