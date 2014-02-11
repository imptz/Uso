#pragma once

#include "../subsystems/io/ioSubsystem.h"
#include "../subsystems/io/iIoDevice.h"

class Bk16Device : public IIODevice, public MessageReceiver, public MessageSender
{
	public:
		static const unsigned int ID = 1;

#pragma region static methods
	public:
		static bool registered;
		static void registerDevice()
		{
			if (!registered)
				IOSubsystem::getSingleton().regDevice(ID, createDevice);

			registered = true;
		}

		static IIODevice* createDevice(unsigned char address, unsigned int _type)
		{
			return new Bk16Device(address, _type);
		}
#pragma endregion

		Bk16Device(unsigned char _address, unsigned int _type);
		~Bk16Device();

		virtual void onMessage(Message message);

		virtual void action();
		virtual bool putFrame(unsigned char* _pArea, bool isNotTransfer = false);
		virtual void init();
		virtual bool isReady();
		virtual void timerHandler();
		virtual unsigned int getId();

	private:
		void createInitFrame();
		void createGetInitializeFrame();
		void createSetOutputFrame(unsigned int number, OUTPUT_STATE value);
		void createSetOutputsFrame(unsigned int* number, unsigned int count, OUTPUT_STATE value);
		void createGetInputsFrame();
		void analizeInputs(unsigned char* resizableArea);

	public:
		virtual bool getInput(unsigned int number, INPUT_STATE* value);
		virtual bool setOutput(unsigned int number, OUTPUT_STATE value);
		virtual void setOutputs(unsigned int* numbers, unsigned int count, OUTPUT_STATE value);
	private:
		bool isOutputExists(unsigned int number);
};
