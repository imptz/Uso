#include "detectionSubsystem.h"
#include "../../../config/Config.h"
#include "../../../Local.h"
#include "../../devices/fv300Device.h"
#include "../../devices/TvDevice.h"

const char* DetectionSubsystem::MISSING_DEVICE_LOG_TEXT = LOCAL_DETECTIONSYSTEM_MISSING_DEVICE_TEXT;

bool DetectionSubsystem::addDevice(unsigned char id, unsigned char address)
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

void DetectionSubsystem::testMissingDevices()
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
			{
				Log::getSingleton().add(LOG_MESSAGE_FROM_EXTENSION_SYSTEM, LOG_MESSAGE_TYPE_ERROR, LOCAL_DETECTIONSYSTEM_MISSING_DEVICE_TEXT, SUBSYSTEM_TYPE, devices[i].id);
			}	
		}
	}
}

DetectionSubsystem::DetectionSubsystem()
	: detectionPhase(DETECTION_PHASE_STOP), pAddress(nullptr), pPoints(nullptr), addressPointsCount(0), actionMoveToPoint(nullptr), actionMoveToPointCount(0), detection(DETECTION_PROCCESS),
	hddPhase(HDD_PHASE_READ_1), pChannelInfo(nullptr)
{
	for (unsigned int i = 0; i < MAX_DEVICE_COUNT; i++)
		pDevices[i] = nullptr;

	ExtensionSystem::getSingleton().regSubsystem(SUBSYSTEM_TYPE, this);

	pTimer->start();
}

DetectionSubsystem::~DetectionSubsystem()
{
	for (unsigned int i = 0; i < MAX_DEVICE_COUNT; i++)
		if (pDevices[i] != nullptr)
			SAFE_DELETE(pDevices[i])
}

void DetectionSubsystem::onMessage(Message message)
{

}

void DetectionSubsystem::timerHandler()
{
	for (unsigned int i = 0; i < MAX_DEVICE_COUNT; i++)
		if (pDevices[i] != nullptr)
			pDevices[i]->timerHandler();
}

void DetectionSubsystem::hddAction()
{
	switch (hddPhase)
	{
		case HDD_PHASE_WRITE_1:
			memcpy(buffer, coordOffset, sizeof(Point3<float>) * COORD_OFFSET_COUNT);
			hddTaskId = HddManager::getSingleton().write(buffer, START_SECTOR, BUFFER_SIZE / 512);
			if (hddTaskId != HddManager::UNDEFINED_ID) 
				hddPhase = HDD_PHASE_WRITE_2;
			break;
		case HDD_PHASE_WRITE_2:
			if (!HddManager::getSingleton().isTaskExecute(hddTaskId))
				hddPhase = HDD_PHASE_STOP;
			break;
		case HDD_PHASE_READ_1:
			hddTaskId = HddManager::getSingleton().read(buffer, START_SECTOR, BUFFER_SIZE / 512);
			if (hddTaskId != HddManager::UNDEFINED_ID) 
				hddPhase = HDD_PHASE_READ_2;

			break;
		case HDD_PHASE_READ_2:
			if (!HddManager::getSingleton().isTaskExecute(hddTaskId))
			{
				hddPhase = HDD_PHASE_STOP;

				memcpy(coordOffset, buffer, sizeof(Point3<float>) * COORD_OFFSET_COUNT);
			}
			break;
	}
}

void DetectionSubsystem::action()
{
	hddAction();

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

	if (actionMoveToPoint != nullptr)
		for (unsigned int i = 0; i < actionMoveToPointCount; i++)
			if (actionMoveToPoint[i] != nullptr)
				actionMoveToPoint[i]->step();

	bool result = true;

	switch (detectionPhase)
	{
		case DETECTION_PHASE_MOVE_PR:
			result = true;
			for (unsigned int i = 0; i < actionMoveToPointCount; i++)
				if (actionMoveToPoint[i]->getState() == Action::STATE_UNDEFINED)
					result = false;
			
			if (result)
			{
				unsigned int _count = actionMoveToPointCount;
				for (unsigned int i = 0; i < actionMoveToPointCount; i++)
					switch (actionMoveToPoint[i]->getState())
					{
						case Action::STATE_ERROR:
							SAFE_DELETE(actionMoveToPoint[i])
							_count--;
							break;
						case Action::STATE_READY:
							break;
					}	

				if (_count == 0)
				{
					detection = DETECTION_FAULT;
					detectionPhase = DETECTION_PHASE_STOP;
					sendMessage(Message(MESSAGE_FROM_OFFSET_DETECTION_MANAGER, DETECTION_MANAGER_MESSAGE_CHANNELS_UPDATED, DETECTION_FAULT, 0));
				}
				else
				{
					if (pDevices[0] == nullptr)
					{
						detection = DETECTION_FAULT;
						detectionPhase = DETECTION_PHASE_STOP;
						sendMessage(Message(MESSAGE_FROM_OFFSET_DETECTION_MANAGER, DETECTION_MANAGER_MESSAGE_CHANNELS_UPDATED, DETECTION_FAULT, 0));
					}
					else
					{
						unsigned char* _channelsList = new unsigned char[actionMoveToPointCount];
						unsigned int _channelsCount = 0;
						for (unsigned int i = 0; i < actionMoveToPointCount; i++)
							if (actionMoveToPoint[i] != nullptr)
								_channelsList[_channelsCount++] = actionMoveToPoint[i]->getDeviceAddress();

						pDevices[0]->updateFire(_channelsList, _channelsCount, actionMoveToPoint);
						detectionPhase = DETECTION_PHASE_GET_FIRE;
					}

					for (unsigned int i = 0; i < actionMoveToPointCount; i++)
						SAFE_DELETE(actionMoveToPoint[i])
				}
			}
			break;
		case DETECTION_PHASE_GET_FIRE:
			if (pDevices[0]->isReady())
			{
				unsigned char* fire = pDevices[0]->getFire();

				if (fire == nullptr)
				{
					detection = DETECTION_FAULT;
					detectionPhase = DETECTION_PHASE_STOP;
					sendMessage(Message(MESSAGE_FROM_OFFSET_DETECTION_MANAGER, DETECTION_MANAGER_MESSAGE_CHANNELS_UPDATED, DETECTION_FAULT, 0));
				}
				else
				{
					if (!isFireExists(fire))
					{
						detection = DETECTION_FAULT;
						detectionPhase = DETECTION_PHASE_STOP;
						sendMessage(Message(MESSAGE_FROM_OFFSET_DETECTION_MANAGER, DETECTION_MANAGER_MESSAGE_CHANNELS_UPDATED, DETECTION_FAULT, 0));
					}
					else
					{
						detection = DETECTION_READY;
						detectionPhase = DETECTION_PHASE_STOP;
						sendMessage(Message(MESSAGE_FROM_OFFSET_DETECTION_MANAGER, DETECTION_MANAGER_MESSAGE_CHANNELS_UPDATED, DETECTION_READY, 0));
					}
				}
			}
			break;
		case DETECTION_PHASE_SCANNER_WAIT:
			if (pDevices[0]->isReady())
			{
				unsigned char* fire = pDevices[0]->getFire();

				if (fire == nullptr)
				{
					detection = DETECTION_FAULT;
					detectionPhase = DETECTION_PHASE_STOP;
					sendMessage(Message(MESSAGE_FROM_OFFSET_DETECTION_MANAGER, DETECTION_MANAGER_MESSAGE_CHANNELS_UPDATED, DETECTION_FAULT, 0));
				}
				else
				{
					if (!isFireExists(fire))
					{
						detection = DETECTION_FAULT;
						detectionPhase = DETECTION_PHASE_STOP;
						sendMessage(Message(MESSAGE_FROM_OFFSET_DETECTION_MANAGER, DETECTION_MANAGER_MESSAGE_CHANNELS_UPDATED, DETECTION_FAULT, 0));
					}
					else
					{
						detection = DETECTION_READY;
						detectionPhase = DETECTION_PHASE_STOP;
						sendMessage(Message(MESSAGE_FROM_OFFSET_DETECTION_MANAGER, DETECTION_MANAGER_MESSAGE_CHANNELS_UPDATED, DETECTION_READY, 0));
					}
				}
			}
			break;
	}
}

bool DetectionSubsystem::putFrame(unsigned char* _pArea, bool isNotTransfer)
{
	for (unsigned int i = 0; i < MAX_DEVICE_COUNT; i++)
		if (pDevices[i] != nullptr)
			if (pDevices[i]->putFrame(_pArea, isNotTransfer))
				return true;

	return false;
}

void DetectionSubsystem::init()
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
}

bool DetectionSubsystem::isReady()
{
	return true;
}

//#pragma region detectionSpecific

void DetectionSubsystem::movePR()
{
	actionMoveToPointCount = addressPointsCount;
	actionMoveToPoint = new ActionMoveToPoint*[actionMoveToPointCount];
	for (unsigned int i = 0; i < actionMoveToPointCount; i++)
	{
		actionMoveToPoint[i] = new ActionMoveToPoint(pAddress[i], pPoints[i]);
	}
}

void DetectionSubsystem::createPointsList(unsigned int* listProgramIndex, unsigned int count)
{
	SAFE_DELETE_ARRAY(pAddress)
	SAFE_DELETE_ARRAY(pPoints)

	addressPointsCount = count;
	pAddress = new unsigned char[addressPointsCount];
	pPoints = new Point2<unsigned int>[addressPointsCount];

	ConfigDataStructProgram** programs = Config::getSingleton().getConfigData()->getConfigDataStructPrograms();

	for (unsigned int i = 0; i < addressPointsCount; i++)
	{
		pAddress[i] = Config::getSingleton().getConfigData()->getPRAddressByNumber(programs[listProgramIndex[i]]->prNumber);
		int x1 = programs[listProgramIndex[i]]->point1.x;
		int x2 = programs[listProgramIndex[i]]->point2.x;
		int y1 = programs[listProgramIndex[i]]->point1.y;
		int y2 = programs[listProgramIndex[i]]->point2.y;

		ConfigDataStructPRPosition** pr = Config::getSingleton().getConfigData()->getConfigDataStructPRPositions();

		unsigned int index = Config::getSingleton().getConfigData()->getPRIndexByAddress(Config::getSingleton().getConfigData()->getPRAddressByNumber(programs[listProgramIndex[i]]->prNumber));
		
		//x1 -= pr[index]->orientation.x; херня полная
		//x2 -= pr[index]->orientation.x;
		//y1 -= pr[index]->orientation.y;
		//y2 -= pr[index]->orientation.y;

		if (x1 > 180)
			x1 = (360 - x1) * -1;
		if (x2 > 180)
			x2 = (360 - x2) * -1;
		if (y1 > 180)
			y1 = (360 - y1) * -1;
		if (y2 > 180)
			y2 = (360 - y2) * -1;

		x1 += ((x2 - x1) / 2);
		y1 += ((y2 - y1) / 2);

		if (x1 < 0)
			x1 += 360;
		if (y1 < 0)
			y1 += 360;

		if (x1 == 360)
			x1 = 0;
		if (y1 == 360)
			y1 = 0;
		
		pPoints[i].x = x1;
		pPoints[i].y = y1;
	}
}

void DetectionSubsystem::createPointsListForScanner(unsigned int* listProgramIndex, unsigned int count)
{
	SAFE_DELETE_ARRAY(pChannelInfo)

	addressPointsCount = count;
	pChannelInfo = new IDetectionDevice::ChannelInfo[addressPointsCount];

	ConfigDataStructProgram** programs = Config::getSingleton().getConfigData()->getConfigDataStructPrograms();

	for (unsigned int i = 0; i < addressPointsCount; i++)
	{
		pChannelInfo[i].address = Config::getSingleton().getConfigData()->getPRAddressByNumber(programs[listProgramIndex[i]]->prNumber);
		pChannelInfo[i].point1.x = programs[listProgramIndex[i]]->point1.x;
		pChannelInfo[i].point1.y = programs[listProgramIndex[i]]->point1.y;
		pChannelInfo[i].point2.x = programs[listProgramIndex[i]]->point2.x;
		pChannelInfo[i].point2.y = programs[listProgramIndex[i]]->point2.y;
		pChannelInfo[i].mode = Config::getSingleton().getConfigData()->getConfigDataStructConst()->numberFireToAnalize;

		ConfigDataStructPRPosition** pr = Config::getSingleton().getConfigData()->getConfigDataStructPRPositions();

		unsigned int index = Config::getSingleton().getConfigData()->getPRIndexByAddress(Config::getSingleton().getConfigData()->getPRAddressByNumber(programs[listProgramIndex[i]]->prNumber));
		
		pChannelInfo[i].point1.x += 360;
		pChannelInfo[i].point2.x += 360;
		pChannelInfo[i].point1.y += 360;
		pChannelInfo[i].point2.y += 360;

		//pChannelInfo[i].point1.x -= pr[index]->orientation.x;
		//pChannelInfo[i].point2.x -= pr[index]->orientation.x;
		//pChannelInfo[i].point1.y -= pr[index]->orientation.y;
		//pChannelInfo[i].point2.y -= pr[index]->orientation.y;


		if (pChannelInfo[i].point1.x > 180)
			pChannelInfo[i].point1.x = (360 - pChannelInfo[i].point1.x) * -1;
		if (pChannelInfo[i].point2.x > 180)
			pChannelInfo[i].point2.x = (360 - pChannelInfo[i].point2.x) * -1;
		if (pChannelInfo[i].point1.y > 180)
			pChannelInfo[i].point1.y = (360 - pChannelInfo[i].point1.y) * -1;
		if (pChannelInfo[i].point2.y > 180)
			pChannelInfo[i].point2.y = (360 - pChannelInfo[i].point2.y) * -1;

		if (pChannelInfo[i].point1.x < 0)
			pChannelInfo[i].point1.x += 360;
		if (pChannelInfo[i].point2.x < 0)
			pChannelInfo[i].point2.x += 360;
		if (pChannelInfo[i].point1.y < 0)
			pChannelInfo[i].point1.y += 360;
		if (pChannelInfo[i].point2.y < 0)
			pChannelInfo[i].point2.y += 360;

		if (pChannelInfo[i].point1.x == 360)
			pChannelInfo[i].point1.x = 0;
		if (pChannelInfo[i].point2.x == 360)
			pChannelInfo[i].point2.x = 0;
		if (pChannelInfo[i].point1.y == 360)
			pChannelInfo[i].point1.y = 0;
		if (pChannelInfo[i].point2.y == 360)
			pChannelInfo[i].point2.y = 0;
	}
}

void DetectionSubsystem::searchFire(unsigned int* listProgramIndex, unsigned int count)
{
	if (devices[0].id == 3)//ScannerDevice::ID)
	{
		createPointsListForScanner(listProgramIndex, count);
		pDevices[0]->updateFire(pChannelInfo, addressPointsCount);
		detectionPhase = DETECTION_PHASE_SCANNER_WAIT;
	}
	else
	{
		createPointsList(listProgramIndex, count);
		movePR();
		detectionPhase = DETECTION_PHASE_MOVE_PR;
	}
}

void DetectionSubsystem::searchFireJustirovka(unsigned char address)
{
	static unsigned char deviceAddress = 0;

	if (pDevices[0] != nullptr)
	{
		deviceAddress = address;
		pDevices[0]->updateFire(&deviceAddress, 1);
		detectionPhase = DETECTION_PHASE_GET_FIRE;
	}
}

bool DetectionSubsystem::isFireExists(unsigned char* _frame)
{
	bool result = false;
	unsigned int offset = 6;
	while (offset < static_cast<unsigned int>((_frame[3] + _frame[4] * 256 + 5)))
	{
		if (_frame[offset] != 0)
		{
			result = true;
			break;
		}

		offset += 2;
	}

	return result;
}

bool DetectionSubsystem::createPreFires(PreFire** preFires, unsigned int* count)
{
	if (pDevices[0] == nullptr)
		return false;

	unsigned char* pFireData = pDevices[0]->getFire();

		unsigned int paramsCount = pFireData[3] + pFireData[4] * 256;
		const unsigned int HEADER_SIZE = 5;
		const unsigned int FIRE_AREA_SIZE = 8;

		unsigned int channelOffset = 6;

		unsigned int validAddressCount = 0;

		while (channelOffset < (paramsCount + HEADER_SIZE))
		{
			unsigned int fireCount = pFireData[channelOffset];
			if (fireCount != 0)
			{
				validAddressCount++;
				channelOffset += (2 + FIRE_AREA_SIZE * fireCount);
			}
			else
				channelOffset += 2;
		}

		*preFires = new PreFire[validAddressCount];
		*count = validAddressCount;

		unsigned int offset = 5;

		unsigned int fireCount = 0;
		unsigned int preFireIndex = 0;

		while (preFireIndex < validAddressCount)
		{
			(*preFires)[preFireIndex].channel = pFireData[offset++];
			unsigned int fireCount = pFireData[offset++];

			if (fireCount != 0)
			{
				unsigned char p0 = pFireData[offset++];
				unsigned int p1 = static_cast<unsigned char>(pFireData[offset++]);
				p1 *= 256;
				p1 += p0;

				unsigned char p2 = pFireData[offset++];
				unsigned int p3 = static_cast<unsigned char>(pFireData[offset++]);
				p3 *= 256;
				p3 += p2;

				unsigned char p4 = pFireData[offset++];
				unsigned int p5 = static_cast<unsigned char>(pFireData[offset++]);
				p5 *= 256;
				p5 += p4;

				unsigned char p6 = pFireData[offset++];
				unsigned int p7 = static_cast<unsigned char>(pFireData[offset++]);
				p7 *= 256;
				p7 += p6;

				float centerX = static_cast<float>(p1);
				if (centerX > 32000)
				{
					centerX -= 32000;
					centerX *= -1;
				}

				float centerY = static_cast<float>(p3);
				if (centerY > 32000)
				{
					centerY -= 32000;
					centerY *= -1;
				}

				float h = static_cast<float>(p5);
				float w = static_cast<float>(p7);
			
				(*preFires)[preFireIndex].center.x = centerX;
				(*preFires)[preFireIndex].center.y = centerY;
				(*preFires)[preFireIndex].leftAngle = centerX - w / 2;
				(*preFires)[preFireIndex].rightAngle = centerX + w / 2;
				(*preFires)[preFireIndex].topAngle = centerY + h / 2;
				(*preFires)[preFireIndex].bottomAngle = centerY - h / 2;
				//(*preFires)[preFireIndex].leftAngle = centerX - w / 3;
				//(*preFires)[preFireIndex].rightAngle = centerX + w / 3;
				//(*preFires)[preFireIndex].topAngle = centerY + h / 3;
				//(*preFires)[preFireIndex].bottomAngle = centerY - h / 3;

				int pointIndex = 0;
				for (unsigned int i = 0; i < addressPointsCount; i++)
					if (pAddress[i] == (*preFires)[preFireIndex].channel)
					{
						pointIndex = i;
						break;
					}

				(*preFires)[preFireIndex].prProg.x = static_cast<float>(pPoints[pointIndex].x);
				(*preFires)[preFireIndex].prProg.y = static_cast<float>(pPoints[pointIndex].y);

				//offset += (FIRE_AREA_SIZE * (fireCount - 1));
				preFireIndex++;
			}
		}

	return true;
}

void DetectionSubsystem::correctionPreFires(PreFire* preFires, unsigned int count)
{
	for (unsigned int preFireIndex = 0; preFireIndex < count; preFireIndex++)
	{
		Point3<float> offset = getCoordOffset(preFires[preFireIndex].channel);

		preFires[preFireIndex].center.x -= offset.x;
		preFires[preFireIndex].center.y -= offset.y;
		preFires[preFireIndex].leftAngle -= offset.x;
		preFires[preFireIndex].rightAngle -= offset.x;
		preFires[preFireIndex].topAngle -= offset.y;
		preFires[preFireIndex].bottomAngle -= offset.y;
	}
}

void DetectionSubsystem::convertionPreFiresToObjectSpace(PreFire* preFires, unsigned int count)
{
	Point3<float> position;
	Point2<float> angle;

	// изменить для fv300 на стене
	//if (pDevices[0]->getId() == Fv300Device::ID)
	
	ConfigDataStructPRPosition** pr = Config::getSingleton().getConfigData()->getConfigDataStructPRPositions();

	for (unsigned int preFireIndex = 0; preFireIndex < count; preFireIndex++)
	{
		unsigned int index = Config::getSingleton().getConfigData()->getPRIndexByAddress(preFires[preFireIndex].channel);
		preFires[preFireIndex].pivotPoint = pr[index]->position;

		int pointIndex = 0;
		for (unsigned int i = 0; i < addressPointsCount; i++)
			if (pAddress[i] == preFires[preFireIndex].channel)
			{
				pointIndex = i;
				break;
			}


		Point2<float> sensorAngle = pr[index]->orientation;
		//sensorAngle.x += pPoints[pointIndex].x;
//		sensorAngle.y += pPoints[pointIndex].y;

		preFires[preFireIndex].center.x += sensorAngle.x;
		preFires[preFireIndex].center.y += sensorAngle.y;
		preFires[preFireIndex].leftAngle += sensorAngle.x;
		preFires[preFireIndex].rightAngle += sensorAngle.x;
		preFires[preFireIndex].topAngle += sensorAngle.y;
		preFires[preFireIndex].bottomAngle += sensorAngle.y;
	}
}

unsigned int DetectionSubsystem::getFire(PreFire** outFires, Fire::FireObject* pFire)
{
	unsigned int preFiresCount = 0;

	if (*outFires != nullptr)
	{
		delete[] (*outFires);
		*outFires = nullptr;
	}

	if (!createPreFires(outFires, &preFiresCount))
		return 0;

	correctionPreFires(*outFires, preFiresCount);
	convertionPreFiresToObjectSpace(*outFires, preFiresCount);

	Fire::calcFire(*outFires, pFire, preFiresCount);

	return preFiresCount;
}

bool DetectionSubsystem::getFireJustirovka()
{
	PreFire* preFires = nullptr;
	unsigned int preFiresCount = 0;

	if (!createPreFires(&preFires, &preFiresCount))
		return false;

	setCoordOffset(preFires[0].channel, Point3<float>(preFires[0].center.x, preFires[0].center.y, 0));
	
	delete[] preFires;

	return true;
}

void DetectionSubsystem::resetDiveces()
{
	if (pDevices[0] != nullptr)
		pDevices[0]->reset();
}

void DetectionSubsystem::setCoordOffset(unsigned int address, Point3<float> value)
{
	if ((address > 0) && (address < COORD_OFFSET_COUNT - 1))
	{
		coordOffset[address] = value;
		hddPhase = HDD_PHASE_WRITE_1;
	}
}

Point3<float> DetectionSubsystem::getCoordOffset(unsigned int address)
{
return Point3<float>(0,0,0);


	if ((address > 0) && (address < COORD_OFFSET_COUNT - 1))
	{
		return coordOffset[address];
	}
	else
		return Point3<float>(0,0,0);
}

//#pragma endregion
