#include "Application.h"

#include "touchpad\touchpad.h"
#include "message\Messages.h"
#include "controls\UI.h"
#include "extension\ExtensionSystem.h"
#include "extension\subsystems\io\ioSubsystem.h"
#include "extension\subsystems\monitoring\monitoringSubsystem.h"
#include "extension\subsystems\detection\detectionSubsystem.h"
#include "extension\subsystems\rpk\rpkSubsystem.h"
#include "DEBUG\serialDebug.h"
#include "extension\devices\bk16Device.h"
#include "extension\devices\fv300Device.h"
#include "extension\devices\monitoringDevice.h"
#include "extension\devices\rpkDevice.h"
#include "Local.h"
#include "logic\povorotniy.h"


Application::Application()
	:	ITimer(TIMER_PERIOD)
{
	//switch (Config::getSingleton().getConfigData()->getConfigDataStructConst()->logic)
	//{
	//	case ConfigDataStructConst::LOGIC_COOLING:
	//		logic = new LogicCooling(this);
	//		break;
	//	case ConfigDataStructConst::LOGIC_SEARCH:
	//		logic = new LogicSearching(this);
	//		break;
	//}

	addReceiver(UI::getSingleton().getMainTabControl());
	UI::getSingleton().getMainTabControl()->addReceiver(this);

	pTimer->start();
}

Application::~Application()
{
	delete logic;
}

void Application::createLogic()
{
	unsigned int count = Config::getSingleton().getConfigData()->getConfigDataStructProgramCount();
	ConfigDataStructProgram** programs = Config::getSingleton().getConfigData()->getConfigDataStructPrograms();

	bool searching = false;
	for (unsigned int i = 0; i < count; i++)
		if ((programs[i]->function == LOGIC_FUNCTION_SEARCHING) || (programs[i]->function == LOGIC_FUNCTION_SEARCHING_PENA) || (programs[i]->function == LOGIC_FUNCTION_SEARCHING_PENA_ZATVOR))
			searching = true;

	if (searching)
	{
		logic = new LogicSearching(this);
	}
	else
	{
		logic = new LogicCooling(this);
	}
	
	addReceiver(logic);
}

void Application::start()
{
	Display::getSingleton().print(LOCAL_MESSAGE_TEXT_INITIALIZATION, 33, 7, true);

	while (!ExtensionSystem::getSingleton().isReady())
	{
		ExtensionSystem::getSingleton().action();
		MessageReceiver::messagesProccess();
	}

	createLogic();

	Display::getSingleton().print("                     ", 33, 7, false);
	UI::getSingleton().getUsoModeControl()->setEnabled(true);

	for (;;)
	{
		ExtensionSystem::getSingleton().action();
		logic->action();
		Config::getSingleton().action();
		Povorotniy::getSingleton().threadAction();
		MessageReceiver::messagesProccess();
	}
}

void Application::onMessage(Message message)
{
	if ((message.from == MESSAGE_FROM_OFFSET_LOGIC) && (message.msg == Logic::LOGIC_MESSAGE_GET_CONFIRMATION))
	{
		UI::getSingleton().getMainTabControl()->setOwner(static_cast<MainConfirmation::CONFIRMATION_OWNER>(message.par2));
		UI::getSingleton().getMainTabControl()->setConfirmationText(reinterpret_cast<char*>(message.par1));
		UI::getSingleton().getMainTabControl()->activateConfirmationTab();
	}

	if ((message.from == MESSAGE_FROM_OFFSET_CONTROLS + UI::getSingleton().getMainTabControl()->getId()) && (message.msg == MainConfirmation::CONFIRMATION_MESSAGE_RESULT))
	{
		sendMessage(Message(MESSAGE_FROM_OFFSET_APPLICATION, MainConfirmation::CONFIRMATION_MESSAGE_RESULT, message.par1, message.par2));
	}

	if ((message.from == MESSAGE_FROM_OFFSET_LOGIC) && (message.msg == Logic::LOGIC_MESSAGE_GET_FINISH))
	{
		UI::getSingleton().getMainTabControl()->activateFinishTab();
	}

	if ((message.from == MESSAGE_FROM_OFFSET_CONTROLS + UI::getSingleton().getMainTabControl()->getId()) && (message.msg == MainFinish::FINISH_MESSAGE_RESULT))
	{
		sendMessage(Message(MESSAGE_FROM_OFFSET_APPLICATION, MainFinish::FINISH_MESSAGE_RESULT, 0, 0));
	}

	if ((message.from == MESSAGE_FROM_OFFSET_LOGIC) && (message.msg == MainTabControl::MAIN_TAB_MESSAGE_SET_MAIN_TAB))
	{
		UI::getSingleton().getMainTabControl()->activateMainTab();
	}

	if ((message.from == MESSAGE_FROM_OFFSET_LOGIC) && (message.msg == MainFinish::FINISH_MESSAGE_LABEL))
	{
		sendMessage(Message(MESSAGE_FROM_OFFSET_APPLICATION, MainFinish::FINISH_MESSAGE_LABEL, message.par1, 0));
	}
}

void Application::timerHandler()
{
	MonitoringSubsystem::getSingleton().createAndSendMessage(IMonitoringDevice::MESSAGE_NUMBER_DUMMY, 0, 0, 0, 0);
}
