#pragma once

#include "singleton.h"
#include "interrupt\interrupt.h"
#include "display\display.h"
#include "serialport\serialport.h"
#include "timer\timer.h"
#include "hdd\hddManager.h"
#include "touchpad\touchpad.h"
#include "application.h"
#include "message\messages.h"
#include "process\process.h"
#include "systems\bk.h"
#include "systems\pws.h"
#include "log\Log.h"
#include "clock\clock.h"
#include "config\Config.h"
#include "controls\UI.h"

class Root : public Singleton<Root>{
private:
	MemoryAllocator memoryAllocator;
	Display* pDisplay;
	TimerManager* pTimerManager;
	SerialPortManager* pSerialPortManager;
	HddManager* pHddManager;
	Touchpad* pTouchpad;
	SerialDebug* pSerialDebug;
	Process* pProcess;
	SystemPanel* pSystemPanel;
	SystemBk* pSystemBk;
	Log* pLog;
	Clock* pClock;
	Config* pConfig;
	UI* pUI;

	Application* pApplication;
		
public:
	Root(){
		Interrupts::initInterrupts();	
		MessageReceiver::init();
		
		if (Display::getSingletonPtr() == nullptr)
			pDisplay = new Display();

		if (TimerManager::getSingletonPtr() == nullptr)
			pTimerManager = new TimerManager();

		if (SerialPortManager::getSingletonPtr() == nullptr)
			pSerialPortManager = new SerialPortManager();

		if (HddManager::getSingletonPtr() == nullptr)
			pHddManager = new HddManager();

		pSerialDebug = new SerialDebug();
		
		if (Touchpad::getSingletonPtr() == nullptr)
			pTouchpad = new Touchpad();

		if (Process::getSingletonPtr() == nullptr)
			pProcess = new Process();

		if (SystemPanel::getSingletonPtr() == nullptr)
			pSystemPanel = new SystemPanel();
		_asm sti

		if (Log::getSingletonPtr() == nullptr)
			pLog = new Log();

		if (Clock::getSingletonPtr() == nullptr)
			pClock = new Clock();

		if (Config::getSingletonPtr() == nullptr)
			pConfig = new Config();

		if (UI::getSingletonPtr() == nullptr)
			pUI = new UI();

		if (Application::getSingletonPtr() == nullptr)
			pApplication = new Application();
	}

	~Root(){
		SAFE_DELETE(pApplication)
		SAFE_DELETE(pSystemBk)
		SAFE_DELETE(pSystemPanel)
		SAFE_DELETE(pProcess)
		delete pSerialDebug;
		SAFE_DELETE(pTouchpad)
		SAFE_DELETE(pHddManager)
		SAFE_DELETE(pSerialPortManager)
		SAFE_DELETE(pTimerManager)
		SAFE_DELETE(pDisplay)
	}
};

