#pragma once

#include "../Singleton.h"
#include "../timer/Timer.h"
#include "../display/display.h"
#include "../message/Messages.h"
#include "../serialport/serialport.h"
#include "ConfigData.h"

class Config : public Singleton<Config>, public ITimer, public MessageSender, public MessageReceiver
{
	private:
		static const unsigned int TIMER_PERIOD = 50;
		virtual void timerHandler();

		ConfigData* pConfigData;
		static const unsigned int MAX_PR_DATA_SIZE = 0;

		virtual void onMessage(Message message);

	public:
		Config();
		~Config();

		ConfigData* getConfigData();

	private:
		int progress;
		int progressFR;
		int progressFRStep;
	public:
		int getUpdateProgress();
		int getUpdateProgressFR();

		void startUpdate();
		void stopUpdate();
		void startUpdateFR();
		void stopUpdateFR();

		enum CONFIG_MESSAGE
		{
			CONFIG_MESSAGE_START_DEVICES_UPDATE = 4,
			CONFIG_MESSAGE_UPDATE_FINISH = 5,
			CONFIG_MESSAGE_UPDATE_ERROR = 6,
			CONFIG_MESSAGE_START_DEVICES_UPDATE_FR = 46,
			CONFIG_MESSAGE_UPDATE_FINISH_FR = 47,
			CONFIG_MESSAGE_UPDATE_ERROR_FR = 48
		};

	private:
		SerialPort* serialPort;
		SERIAL_PORT serialPortName;
		SERIAL_PORT_SPEED serialPortSpeed;

		static const unsigned int CONNECT_CODE = 0xfa12c64b;
		static const unsigned int CONNECT_ANSWER_CODE = 0x45dc69a3;
		static const unsigned int DISCONNECT_CODE = 0x8e94ac13;
		static const unsigned int DOWNLOAD_RESULT_OK = 0x00000000;
		static const unsigned int DOWNLOAD_RESULT_FAULT = 0xffffffff;

	public:
		enum CONFIG_PHASE
		{
			CONFIG_PHASE_STOP = 0,
			CONFIG_PHASE_CONNECTION = 1,
			CONFIG_PHASE_LENGTH = 2,
			CONFIG_PHASE_DATA = 3,
			CONFIG_PHASE_DISCONNECT = 4,
			CONFIG_PHASE_DISCONNECT_AFTER_ERROR = 5
		};
	private:
		CONFIG_PHASE phase;

		enum CONFIG_PHASE_FR
		{
			CONFIG_PHASE_FR_STOP,
			CONFIG_PHASE_FR_CONNECTION,
			CONFIG_PHASE_FR_CONNECTION_WAIT,
			CONFIG_PHASE_FR_TRAJECTORY,
			CONFIG_PHASE_FR_TRAJECTORY_WAIT,
			CONFIG_PHASE_FR_PRESSURE,
			CONFIG_PHASE_FR_PRESSURE_WAIT,
			CONFIG_PHASE_FR_DISCONNECTION,
			CONFIG_PHASE_FR_DISCONNECTION_WAIT,
		};
	private:
		CONFIG_PHASE_FR phaseFR;

		void configPhaseFrConnection();
		void configPhaseFrConnectionWait();
		void configPhaseFrTrajectory();
		void configPhaseFrTrajectoryWait();
		void configPhaseFrPressure();
		void configPhaseFrPressureWait();
		void configPhaseFrDisconnection();
		void configPhaseFrDisconnectionWait();

		unsigned int dataSize;
		int loadDataSize;
		static const unsigned int MAX_DATA_SIZE = 1024 * 256;
		unsigned char buffer[MAX_DATA_SIZE];
		static const unsigned int LENGTH_AREA_SIZE = 4;
		static const unsigned int CRC_AREA_SIZE = 4;

		void pcTimerHandler();
		void frTimerHandler();

		unsigned char* prNumbers;
		unsigned int prNumbersCount;
		unsigned int prNumbersIndex;

		unsigned char* trajectoryNumbers;
		unsigned int trajectoryNumbersCount;
		unsigned int trajectoryNumbersIndex;

		unsigned char* pressureTableNumbers;
		unsigned int pressureTableNumbersCount;
		unsigned int pressureTableNumbersIndex;

		unsigned int trajectoryPointCount;
		unsigned int trajectoryPointIndex;

		unsigned int pressureCount;
		unsigned int pressureIndex;

		unsigned int createPrNumbers();
		unsigned int createTrajectoryNumbers(unsigned char prNumber);
		unsigned int createPointsNumbers(unsigned char prNumber);
		unsigned int createPressureTableNumbers(unsigned char prNumber);
		unsigned int getTrajectoryPointsCount(unsigned char prNumber, unsigned char trajectoryNumber = 255);
		unsigned int getPressureCount(unsigned char prNumber);

		int getNextTrajectoryPointsIndex(unsigned int firstIndex, unsigned char prNumber);
		bool getPressureTableItemsIndex(unsigned char pressureTableNumber, unsigned int** pIndex);

		enum COMMAND
		{
			COMMAND_SET_TRAJECTORY = 130
		};

		enum SUBCOMMAND
		{
			SUBCOMMAND_START = 1,
			SUBCOMMAND_POINT = 2,
			SUBCOMMAND_PRESSURE = 3,
			SUBCOMMAND_FINISH = 4
		};

		unsigned int frameId;
		unsigned int frameId1;
		unsigned int frameIdRes;
		unsigned int frameId1Res;

	public:
		void action();
	private:
		void pcAction();
		void frAction();

		void frActionError(CONFIG_PHASE_FR phase, unsigned char prNumber);
		void frActionFinish();

	public:
		void sendMessageToPort(unsigned char* pData, unsigned int size);
};
