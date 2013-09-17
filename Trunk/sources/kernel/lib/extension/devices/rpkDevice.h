#pragma once
#include "../subsystems/rpk/rpkSubsystem.h"
#include "../subsystems/rpk/iRpkDevice.h"
#include "../ExtensionSystem.h"

class RpkDevice : public IRpkDevice, public MessageReceiver, public MessageSender
{
	public:
		static const unsigned int ID = 1;

#pragma region static methods
	public:
		static bool registered;
		static void registerDevice()
		{
			if (!registered)
				RpkSubsystem::getSingleton().regDevice(ID, createDevice);

			registered = true;
		}

		static IRpkDevice* createDevice(unsigned char address, unsigned int _type)
		{
			return new RpkDevice(address, _type);
		}
#pragma endregion
		RpkDevice(unsigned char _address, unsigned int _type);
		~RpkDevice();

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

	private:
		struct Item
		{
			unsigned int id;
			unsigned char* frame;
			FRAME_RESULT resultCode;
			static const unsigned int TIME_LIVE = 10;
			unsigned int timeLive;

			Item(unsigned int _id)
				:	id(_id), frame(nullptr), resultCode(FRAME_RESULT_PROCCESS), timeLive(TIME_LIVE)
			{}

			~Item()
			{
				SAFE_DELETE_ARRAY(frame);
			}
		};

		static const unsigned int MAX_ITEMS_NUMBER = 1000;
		Item** items;
		
		bool addItem(Item* pItem);
		bool removeItem(unsigned int index);
		int findItemById(unsigned int id);

		unsigned char calcRpcKS(unsigned char* pFrame);
	public:
		virtual unsigned int write(unsigned char* pFrame);
		virtual FRAME_RESULT read(unsigned int id, unsigned char** pFrame);
	private:
		void createGetAnyReadyFrame();
		void handlerGetAnyReadyFrame(unsigned char* _pArea);

		static const unsigned int INTERNAL_FRAME_DATA_OFFSET = 5;

		enum INTERNAL_FRAME_DATA_POSITION
		{
			INTERNAL_FRAME_DATA_POSITION_TARGET_ADDRESS = 0,
			INTERNAL_FRAME_DATA_POSITION_SOURCE_ADDRESS = 1,
			INTERNAL_FRAME_DATA_POSITION_COMMAND = 2,
			INTERNAL_FRAME_DATA_POSITION_LENGTH_LOW = 3,
			INTERNAL_FRAME_DATA_POSITION_LENGTH_HIGH = 4
		};

		static const unsigned int RPK_FRAME_DATA_OFFSET = 4;

		enum RPK_FRAME_DATA_POSITION
		{
			RPK_FRAME_DATA_POSITION_TARGET_ADDRESS = 0,
			RPK_FRAME_DATA_POSITION_SOURCE_ADDRESS = 1,
			RPK_FRAME_DATA_POSITION_COMMAND = 2,
			RPK_FRAME_DATA_POSITION_LENGTH = 3
		};
};
