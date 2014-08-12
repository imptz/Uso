#pragma once

#include "../../message/Messages.h"
#include "../../fifo.h"
#include "../../log/Log.h"

class IDevice
{
	public:
		enum COMMAND
		{
			COMMAND_GET_ID = 1
		};

	protected:

		static const unsigned int FIFO_FRAME_SIZE = 10000;
		Fifo<unsigned char*>* fifoFrame;

		unsigned char address;

	public:
		IDevice(unsigned char _address, unsigned int _type)
			:	fifoFrame(new Fifo<unsigned char*>(FIFO_FRAME_SIZE)), address(_address), type(_type), actionTimeOut(0), isActionTimeOut(false), disabled(false), obrivTest(false), obriv(false)
		{}

		virtual void action() = 0;
		virtual bool putFrame(unsigned char* _pArea, bool isNotTransfer = false) = 0;
		virtual void init() = 0;
		virtual bool isReady() = 0;
		virtual void timerHandler() = 0;
		virtual unsigned int getId() = 0;

	protected:
		unsigned int type;
		static const unsigned int INITIALIZE_RESULT_OK = 1;
		static const unsigned int ACTION_TIME_OUT = 200;
		unsigned int actionTimeOut;
		bool isActionTimeOut;

		bool disabled;
		virtual void disablingDevice() = 0;

		bool obrivTest;
		bool obriv;
		bool obrivOffTest(char* msg, int par1, int par2){
			if (obrivTest & !obriv){
				Log::getSingleton().add(LOG_MESSAGE_FROM_EXTENSION_SYSTEM, LOG_MESSAGE_TYPE_MESSAGE, msg, par1, par2);

				obriv = true;
				obrivTest = true;
				return true;
			}
			obrivTest = true;
			return false;
		}

		void obrivOnTest(char* msg, int par1, int par2){
			if (obriv){
				Log::getSingleton().add(LOG_MESSAGE_FROM_EXTENSION_SYSTEM, LOG_MESSAGE_TYPE_MESSAGE, msg, par1, par2);
				obriv = false;
			}
			obrivTest = false;
		}
};
