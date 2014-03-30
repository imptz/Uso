#include "scannerDevice.h"
#include "../ExtensionSystem.h"
#include "../../config/ConfigData.h"
#include "../../config/Config.h"
#include "../../Local.h"

bool ScannerDevice::registered = false;

ScannerDevice::ScannerDevice(unsigned char _address, unsigned int _type)
	:	IDetectionDevice(_address, _type), actionSearch(nullptr), actionCount(0), pChannelInfo(nullptr), fireFrame(nullptr)
{
}

ScannerDevice::~ScannerDevice()
{
	if (actionCount != 0)
	{
		for (unsigned int i = 0; i < actionCount; i++)
			delete actionSearch[i];

		delete[] actionSearch;
	}
}

void ScannerDevice::init()
{
	phase = PHASE_START;
}

bool ScannerDevice::isReady()
{
	return (phase == PHASE_START);
}

void ScannerDevice::timerHandler()
{
	if (actionTimeOut++ == ACTION_TIME_OUT)
	{
		isActionTimeOut = true;
		actionTimeOut = 0;
	}

	if (fireTimeout > 0)
		fireTimeout--;
}

void ScannerDevice::action()
{
	for (unsigned int i = 0; i < actionCount; i++)
		if (actionSearch[i] != nullptr)
			actionSearch[i]->step();

	bool zaversheno;
	if (!disabled)
	{
		switch (phase)
		{
			case PHASE_SET_GET_FIRE:
				actionCount = updateChannelsCount;
				actionSearch = new ActionSearch*[actionCount];
				for (unsigned int i = 0; i < actionCount; i++)
				{
					DEBUG_PUT_METHOD("address = %i\n", address)
					actionSearch[i] = new ActionSearch(pChannelInfo[i].address, pChannelInfo[i].point1, pChannelInfo[i].point2, pChannelInfo[i].mode);
				}

				fireTimeout = TIME_OUT;
				phase = PHASE_SET_GET_FIRE_WAIT;
				break;
			case PHASE_SET_GET_FIRE_WAIT:
				zaversheno = true;
				for (unsigned int i = 0; i < actionCount; i++)
					if (actionSearch[i]->getState() == Action::STATE_UNDEFINED)
						zaversheno = false;

				if (zaversheno)
				{
					DEBUG_PUT_METHOD("zaversheno actionCount = %i\n", actionCount)
					Point2<int> _point1;
					Point2<int> _point2;
					if (fireFrame != nullptr)
						SAFE_DELETE_ARRAY(fireFrame)

					unsigned int fireCount = 0;
					unsigned int notFireCount = 0;
					for (unsigned int i = 0; i < actionCount; i++)
						if (actionSearch[i]->getResult(_point1, _point2))
							fireCount++;
						else
							notFireCount++;

					const unsigned int NOT_FIRE_SIZE = 2;
					const unsigned int FIRE_SIZE = 10;
					fireFrame = new unsigned char[7 + notFireCount * NOT_FIRE_SIZE + fireCount * FIRE_SIZE];

					fireFrame[0] = 1;
					fireFrame[1] = 1;
					fireFrame[2] = 5;
					fireFrame[3] = notFireCount * NOT_FIRE_SIZE + fireCount * FIRE_SIZE;
					fireFrame[4] = (notFireCount * NOT_FIRE_SIZE + fireCount * FIRE_SIZE) >> 8;

					int frameIndex = 5;

					ConfigDataStructPRPosition** prPosition = Config::getSingleton().getConfigData()->getConfigDataStructPRPositions();

					for (unsigned int i = 0; i < actionCount; i++)
						if (actionSearch[i]->getResult(_point1, _point2))
						{
							fireFrame[frameIndex++] = actionSearch[i]->getDeviceAddress();
							fireFrame[frameIndex++] = 1;

							unsigned char index = Config::getSingleton().getConfigData()->getPRIndexByAddress(actionSearch[i]->getDeviceAddress());

							// координаты центра зоны поиска
							float c1x = static_cast<float>(actionSearch[i]->point1.x);
							float c2x = static_cast<float>(actionSearch[i]->point2.x);
							float c1y = static_cast<float>(actionSearch[i]->point1.y);
							float c2y = static_cast<float>(actionSearch[i]->point2.y);
							
							if (c1x > 180)
								c1x -= 360; 
							if (c2x > 180)
								c2x -= 360; 
							if (c1y > 180)
								c1y -= 360; 
							if (c2y > 180)
								c2y -= 360; 

							float cx = c2x + (c1x - c2x) / 2;
							float cy = c2y + (c1y - c2y) / 2;

							if (_point1.x > 180)
								_point1.x -= 360; 
							if (_point2.x > 180)
								_point2.x -= 360; 
							if (_point1.y > 180)
								_point1.y -= 360; 
							if (_point2.y > 180)
								_point2.y -= 360; 

							// для сканеров координаты очага в СО ПР, а не в СО зоны поиска
							//_point1.x -= cx; 
							//_point2.x -= cx; 
							//_point1.y -= cy; 
							//_point2.y -= cy; 

							cx = static_cast<float>(_point2.x + (_point1.x - _point2.x) / 2);
							cy = static_cast<float>(_point2.y + (_point1.y - _point2.y) / 2);

							if (cx < 0)
								cx = 32000 + abs(cx);
							if (cy < 0)
								cy = 32000 + abs(cy);

							fireFrame[frameIndex++] = static_cast<int>(cx) % 256;
							fireFrame[frameIndex++] = static_cast<int>(cx) / 256;
							fireFrame[frameIndex++] = static_cast<int>(cy) % 256;
							fireFrame[frameIndex++] = static_cast<int>(cy) / 256;

							fireFrame[frameIndex++] = static_cast<int>(abs(static_cast<float>(_point1.y - _point2.y))) % 256;
							fireFrame[frameIndex++] = static_cast<int>(abs(static_cast<float>(_point1.y - _point2.y))) / 256;
							fireFrame[frameIndex++] = static_cast<int>(abs(static_cast<float>(_point1.x - _point2.x))) % 256;
							fireFrame[frameIndex++] = static_cast<int>(abs(static_cast<float>(_point1.x - _point2.x))) / 256;
						}
						else
						{
							fireFrame[frameIndex++] = actionSearch[i]->getDeviceAddress();
							fireFrame[frameIndex++] = 0;
						}

					for (unsigned int i = 0; i < actionCount; i++)
						delete actionSearch[i];

					actionCount = 0;

					SAFE_DELETE_ARRAY(actionSearch)
						
					phase = PHASE_START;
				}
/* 
QWERT:

сравнение на fireTime == 0 не должно производиться если обработан очаг, потому что тогда будет происходить вторичное
удаление  delete actionSearch[i]
*/
				if (fireTimeout == 0)
				{
					for (unsigned int i = 0; i < actionCount; i++)
						delete actionSearch[i];

					actionCount = 0;

					SAFE_DELETE_ARRAY(actionSearch)
					phase = PHASE_START;
				}
				break;
		}
	}
}

bool ScannerDevice::putFrame(unsigned char* _pArea, bool isNotTransfer)
{
	return false;
}

void ScannerDevice::onMessage(Message message)
{
}

unsigned int ScannerDevice::getId()
{
	return ID;
}

void ScannerDevice::updateFire(unsigned char* pAddress, unsigned int count, ActionMoveToPoint** actionMoveToPoint)
{
}

void ScannerDevice::updateFire(ChannelInfo* _pChannelInfo, unsigned int count)
{
	if (pChannelInfo != nullptr)
		delete[] pChannelInfo;

	SAFE_DELETE_ARRAY(fireFrame);
	updateChannelsCount = count;
	
	DEBUG_PUT_METHOD("updateChannelsCount = %i\n", updateChannelsCount)

	pChannelInfo = new ChannelInfo[updateChannelsCount];
	for (unsigned int i = 0; i < updateChannelsCount; i++)
	{
		pChannelInfo[i].address = _pChannelInfo[i].address;
		pChannelInfo[i].point1 = _pChannelInfo[i].point1;
		pChannelInfo[i].point2 = _pChannelInfo[i].point2;
		pChannelInfo[i].mode = _pChannelInfo[i].mode;
	}
	
	phase = PHASE_SET_GET_FIRE;
}

unsigned char* ScannerDevice::getFire()
{
	return fireFrame;
}

void ScannerDevice::reset()
{
}
