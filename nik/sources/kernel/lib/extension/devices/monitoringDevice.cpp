#include "monitoringDevice.h"
#include "../ExtensionSystem.h"
#include "../subsystems/io/ioSubsystem.h"
#include "../../config/ConfigData.h"
#include "../../config/Config.h"
#include "../../Local.h"
#include "../../clock/clock.h"
#include "../../log/Log.h"
#include "../../DEBUG/serialDebug.h"
#include "../../controls/UI.h"
#include "../../controls/UsoModeControl.h"
#include "../../controls/UI.h"

bool MonitoringDevice::registered = false;

#pragma region message texts

char* MonitoringDevice::MESSAGE_TEXT_SVAZ_S_PLATOY_PRIVODA_NET = LOCAL_MONITORING_DEVICE_SVAZ_S_PLATOY_PRIVODA_NET_TEXT;
char* MonitoringDevice::MESSAGE_TEXT_SVAZ_S_PLATOY_PRIVODA_EST = LOCAL_MONITORING_DEVICE_SVAZ_S_PLATOY_PRIVODA_EST_TEXT;
char* MonitoringDevice::MESSAGE_TEXT_PONIZHENNOE_NAPRAZHENIE_NA_KONDENSATORAH = LOCAL_MESSAGE_TEXT_PONIZHENNOE_NAPRAZHENIE_NA_KONDENSATORAH;
char* MonitoringDevice::MESSAGE_TEXT_NAPRAZHENIE_NA_KONDENSATORAH_V_NORME = LOCAL_MESSAGE_TEXT_NAPRAZHENIE_NA_KONDENSATORAH_V_NORME;
char* MonitoringDevice::MESSAGE_TEXT_NEISPRAVNOST_SILIVOGO_MOSTA = LOCAL_MESSAGE_TEXT_NEISPRAVNOST_SILIVOGO_MOSTA;
char* MonitoringDevice::MESSAGE_TEXT_VOSSTANOVLENIE_SILOVOGO_MOSTA = LOCAL_MESSAGE_TEXT_VOSSTANOVLENIE_SILOVOGO_MOSTA;
char* MonitoringDevice::MESSAGE_TEXT_NIZKOE_NAPRAZHENIE_PITANIA_CP = LOCAL_MESSAGE_TEXT_NIZKOE_NAPRAZHENIE_PITANIA_CP;
char* MonitoringDevice::MESSAGE_TEXT_NAPRAZHENIE_PITANIA_CP_V_NORME = LOCAL_MESSAGE_TEXT_NAPRAZHENIE_PITANIA_CP_V_NORME;
char* MonitoringDevice::MESSAGE_TEXT_NIZKOE_NAPRAZHENIE_PITANIA_DRAIVERA = LOCAL_MESSAGE_TEXT_NIZKOE_NAPRAZHENIE_PITANIA_DRAIVERA;
char* MonitoringDevice::MESSAGE_TEXT_NIZKOE_NAPRAZHENIE_PITANIA_DRAIVERA_V_NORME = LOCAL_MESSAGE_TEXT_NIZKOE_NAPRAZHENIE_PITANIA_DRAIVERA_V_NORME;
char* MonitoringDevice::MESSAGE_TEXT_VISOKOE_NAPRAZHENIE_PITANIA_DRAIVERA = LOCAL_MESSAGE_TEXT_VISOKOE_NAPRAZHENIE_PITANIA_DRAIVERA;
char* MonitoringDevice::MESSAGE_TEXT_VISOKOE_NAPRAZHENIE_PITANIA_DRAIVERA_V_NORME = LOCAL_MESSAGE_TEXT_VISOKOE_NAPRAZHENIE_PITANIA_DRAIVERA_V_NORME;
char* MonitoringDevice::MESSAGE_TEXT_NIZKOE_NAPRAZHENIE_PITANIA_PLATI = LOCAL_MESSAGE_TEXT_NIZKOE_NAPRAZHENIE_PITANIA_PLATI;
char* MonitoringDevice::MESSAGE_TEXT_NIZKOE_NAPRAZHENIE_PITANIA_PLATI_V_NORME = LOCAL_MESSAGE_TEXT_NIZKOE_NAPRAZHENIE_PITANIA_PLATI_V_NORME;
char* MonitoringDevice::MESSAGE_TEXT_VISOKOE_NAPRAZHENIE_PITANIA_PLATI = LOCAL_MESSAGE_TEXT_VISOKOE_NAPRAZHENIE_PITANIA_PLATI;
char* MonitoringDevice::MESSAGE_TEXT_VISOKOE_NAPRAZHENIE_PITANIA_PLATI_V_NORME = LOCAL_MESSAGE_TEXT_VISOKOE_NAPRAZHENIE_PITANIA_PLATI_V_NORME;
char* MonitoringDevice::MESSAGE_TEXT_OSHIBKA_TESTA_FLESH_PAMATI = LOCAL_MESSAGE_TEXT_OSHIBKA_TESTA_FLESH_PAMATI;
char* MonitoringDevice::MESSAGE_TEXT_TEST_FLASH_PAMATI_VIPOLNEN_USPESHNO = LOCAL_MESSAGE_TEXT_TEST_FLASH_PAMATI_VIPOLNEN_USPESHNO;
char* MonitoringDevice::MESSAGE_TEXT_UROVNI_SIGNALOV_ENKODERA_NE_V_NORME = LOCAL_MESSAGE_TEXT_UROVNI_SIGNALOV_ENKODERA_NE_V_NORME;
char* MonitoringDevice::MESSAGE_TEXT_UROVNI_SIGNALOV_ENKODERA_V_NORME = LOCAL_MESSAGE_TEXT_UROVNI_SIGNALOV_ENKODERA_V_NORME;
char* MonitoringDevice::MESSAGE_TEXT_NEISPRAVNOST_KANALA_ENKODERA = LOCAL_MESSAGE_TEXT_NEISPRAVNOST_KANALA_ENKODERA;
char* MonitoringDevice::MESSAGE_TEXT_VOSSTANOVLENIE_KANALA_ENKODERA = LOCAL_MESSAGE_TEXT_VOSSTANOVLENIE_KANALA_ENKODERA;
char* MonitoringDevice::MESSAGE_TEXT_OSHIBKA_OPREDELENIA_NAPRAVLENIA = LOCAL_MESSAGE_TEXT_OSHIBKA_OPREDELENIA_NAPRAVLENIA;
char* MonitoringDevice::MESSAGE_TEXT_VERNOE_OPREDELENIE_NAPRAVLENIA = LOCAL_MESSAGE_TEXT_VERNOE_OPREDELENIE_NAPRAVLENIA;
char* MonitoringDevice::MESSAGE_TEXT_PRI_DVIZHENII_NE_MENAETSA_KOORDINATA = LOCAL_MESSAGE_TEXT_PRI_DVIZHENII_NE_MENAETSA_KOORDINATA;
char* MonitoringDevice::MESSAGE_TEXT_PRI_DVIZHENII_MENAETSA_KOORDINATA = LOCAL_MESSAGE_TEXT_PRI_DVIZHENII_MENAETSA_KOORDINATA;
char* MonitoringDevice::MESSAGE_TEXT_OTSUTSTVIE_TOKA_MOTORA = LOCAL_MESSAGE_TEXT_OTSUTSTVIE_TOKA_MOTORA;
char* MonitoringDevice::MESSAGE_TEXT_TOK_MOTORA_V_PREDELAH_NORMI = LOCAL_MESSAGE_TEXT_TOK_MOTORA_V_PREDELAH_NORMI;
char* MonitoringDevice::MESSAGE_TEXT_AVARIYNAYA_AMPLITUDE_TOKA_MOTORA = LOCAL_MESSAGE_TEXT_AVARIYNAYA_AMPLITUDE_TOKA_MOTORA;
char* MonitoringDevice::MESSAGE_TEXT_AMPLITUDA_TOKA_MOTORA_V_NORME = LOCAL_MESSAGE_TEXT_AMPLITUDA_TOKA_MOTORA_V_NORME;
char* MonitoringDevice::MESSAGE_TEXT_NAPRAZHENIE_PITANIA_NIZHE_NORMI = LOCAL_MESSAGE_TEXT_NAPRAZHENIE_PITANIA_NIZHE_NORMI;
char* MonitoringDevice::MESSAGE_TEXT_NAPRAZHENIE_PITANIA_NIZHE_NORMI_V_NORME = LOCAL_MESSAGE_TEXT_NAPRAZHENIE_PITANIA_NIZHE_NORMI_V_NORME;
char* MonitoringDevice::MESSAGE_TEXT_NAPRAZHENIE_PITANIA_VISHE_NORMI = LOCAL_MESSAGE_TEXT_NAPRAZHENIE_PITANIA_VISHE_NORMI;
char* MonitoringDevice::MESSAGE_TEXT_NAPRAZHENIE_PITANIA_VISHE_NORMI_V_NORME = LOCAL_MESSAGE_TEXT_NAPRAZHENIE_PITANIA_VISHE_NORMI_V_NORME;
char* MonitoringDevice::MESSAGE_TEXT_PR_OSHIBKA_TESTA_FLESH_PAMATI = LOCAL_MESSAGE_TEXT_PR_OSHIBKA_TESTA_FLESH_PAMATI;
char* MonitoringDevice::MESSAGE_TEXT_PR_TEST_FLASH_PAMATI_VIPOLNEN_USPESHNO = LOCAL_MESSAGE_TEXT_PR_TEST_FLASH_PAMATI_VIPOLNEN_USPESHNO;
char* MonitoringDevice::MESSAGE_TEXT_PR_NIZROE_NAPRAZHENIE_PITENIA_PROCESSORA = LOCAL_MESSAGE_TEXT_PR_NIZROE_NAPRAZHENIE_PITENIA_PROCESSORA;
char* MonitoringDevice::MESSAGE_TEXT_PR_NIZROE_NAPRAZHENIE_PITENIA_PROCESSORA_V_NORME = LOCAL_MESSAGE_TEXT_PR_NIZROE_NAPRAZHENIE_PITENIA_PROCESSORA_V_NORME;
char* MonitoringDevice::MESSAGE_TEXT_OBRIV_DATCHIKA_DAVLENIA = LOCAL_MESSAGE_TEXT_OBRIV_DATCHIKA_DAVLENIA;
char* MonitoringDevice::MESSAGE_TEXT_OBRIV_DATCHIKA_DAVLENIA_V_NORME = LOCAL_MESSAGE_TEXT_OBRIV_DATCHIKA_DAVLENIA_V_NORME;
char* MonitoringDevice::MESSAGE_TEXT_KZ_DATCHIKA_DAVLENIA = LOCAL_MESSAGE_TEXT_KZ_DATCHIKA_DAVLENIA;
char* MonitoringDevice::MESSAGE_TEXT_KZ_DATCHIKA_DAVLENIA_V_NORME = LOCAL_MESSAGE_TEXT_KZ_DATCHIKA_DAVLENIA_V_NORME;
char* MonitoringDevice::MESSAGE_TEXT_PEREZAGRUZKA_CP = LOCAL_MESSAGE_TEXT_PEREZAGRUZKA_CP;
char* MonitoringDevice::MESSAGE_TEXT_SVAZ_S_PR_VOSSTANOVLENA = LOCAL_MESSAGE_TEXT_SVAZ_S_PR_VOSSTANOVLENA;
char* MonitoringDevice::MESSAGE_TEXT_OTSUTSTVUET_SVAZ_S_PR = LOCAL_MESSAGE_TEXT_OTSUTSTVUET_SVAZ_S_PR;
char* MonitoringDevice::MESSAGE_TEXT_OBRIV_MAGISTRALI_RPK = LOCAL_MESSAGE_TEXT_OBRIV_MAGISTRALI_RPK;
char* MonitoringDevice::MESSAGE_TEXT_MAGISTRAL_RPK_VOSSTANOVLENA = LOCAL_MESSAGE_TEXT_MAGISTRAL_RPK_VOSSTANOVLENA;
char* MonitoringDevice::MESSAGE_TEXT_OTSUTSTVUET_SVAZ_S_SK = LOCAL_MESSAGE_TEXT_OTSUTSTVUET_SVAZ_S_SK;
char* MonitoringDevice::MESSAGE_TEXT_SVAZ_S_SK_VOSSTANOVLENA = LOCAL_MESSAGE_TEXT_SVAZ_S_SK_VOSSTANOVLENA;
char* MonitoringDevice::MESSAGE_TEXT_OBRIV_SVAZI_S_ZATVOROM = LOCAL_MESSAGE_TEXT_OBRIV_SVAZI_S_ZATVOROM;
char* MonitoringDevice::MESSAGE_TEXT_SVAZ_S_ZATVOROM_VOSSTANOVLENA = LOCAL_MESSAGE_TEXT_SVAZ_S_ZATVOROM_VOSSTANOVLENA;
char* MonitoringDevice::MESSAGE_TEXT_OBRIV_DATCHIKA_TEMPERATURI = LOCAL_MESSAGE_TEXT_OBRIV_DATCHIKA_TEMPERATURI;
char* MonitoringDevice::MESSAGE_TEXT_OBRIV_DATCHIKA_TEMPERATURI_ISPRAVEN = LOCAL_MESSAGE_TEXT_OBRIV_DATCHIKA_TEMPERATURI_ISPRAVEN;
char* MonitoringDevice::MESSAGE_TEXT_KZ_DATCHIKA_TEMPERATURI = LOCAL_MESSAGE_TEXT_KZ_DATCHIKA_TEMPERATURI;
char* MonitoringDevice::MESSAGE_TEXT_KZ_DATCHIKA_TEMPERATURI_ISPRAVEN = LOCAL_MESSAGE_TEXT_KZ_DATCHIKA_TEMPERATURI_ISPRAVEN;
char* MonitoringDevice::MESSAGE_TEXT_OBRIV_DATCHIKA_VLAGNOSTI = LOCAL_MESSAGE_TEXT_OBRIV_DATCHIKA_VLAGNOSTI;
char* MonitoringDevice::MESSAGE_TEXT_OBRIV_DATCHIKA_VLAGNOSTI_ISPRAVEN = LOCAL_MESSAGE_TEXT_OBRIV_DATCHIKA_VLAGNOSTI_ISPRAVEN;
char* MonitoringDevice::MESSAGE_TEXT_KZ_KNOPOK_VNESHNEGO_POSTA = LOCAL_MESSAGE_TEXT_KZ_KNOPOK_VNESHNEGO_POSTA;
char* MonitoringDevice::MESSAGE_TEXT_KZ_KNOPOK_VNESHNEGO_POSTA_VOSST = LOCAL_MESSAGE_TEXT_KZ_KNOPOK_VNESHNEGO_POSTA_VOSST;
char* MonitoringDevice::MESSAGE_TEXT_OBRIV_KNOPOK_VNESHNEGO_POSTA = LOCAL_MESSAGE_TEXT_OBRIV_KNOPOK_VNESHNEGO_POSTA;
char* MonitoringDevice::MESSAGE_TEXT_OBRIV_KNOPOK_VNESHNEGO_POSTA_VOSST = LOCAL_MESSAGE_TEXT_OBRIV_KNOPOK_VNESHNEGO_POSTA_VOSST;
char* MonitoringDevice::MESSAGE_TEXT_VOSSTANOVLENIE_SOEDINITELNIH_LINIY = LOCAL_MESSAGE_TEXT_VOSSTANOVLENIE_SOEDINITELNIH_LINIY;
char* MonitoringDevice::MESSAGE_TEXT_KZ_SOEDINITELNOY_LINII = LOCAL_MESSAGE_TEXT_KZ_SOEDINITELNOY_LINII;
char* MonitoringDevice::MESSAGE_TEXT_OBRIV_SOEDINITELNOY_LINII = LOCAL_MESSAGE_TEXT_OBRIV_SOEDINITELNOY_LINII;
char* MonitoringDevice::MESSAGE_TEXT_OSHIBKA_OPREDELENIA_SOSTOYANIA_SL = LOCAL_MESSAGE_TEXT_OSHIBKA_OPREDELENIA_SOSTOYANIA_SL;
char* MonitoringDevice::MESSAGE_TEXT_SVAZ_VOSSTANOVLENA = LOCAL_MESSAGE_TEXT_SVAZ_VOSSTANOVLENA;
char* MonitoringDevice::MESSAGE_TEXT_OTSUTSTVUET_SVAZ = LOCAL_MESSAGE_TEXT_OTSUTSTVUET_SVAZ;
char* MonitoringDevice::MESSAGE_TEXT_IZVECHATEL_V_NORME = LOCAL_MESSAGE_TEXT_IZVECHATEL_V_NORME;
char* MonitoringDevice::MESSAGE_TEXT_NET_SVAZI_S_IZVECHATELEM = LOCAL_MESSAGE_TEXT_NET_SVAZI_S_IZVECHATELEM;
char* MonitoringDevice::MESSAGE_TEXT_VNUTRENNAYA_OSHIBKA_IZVECHATELA = LOCAL_MESSAGE_TEXT_VNUTRENNAYA_OSHIBKA_IZVECHATELA;
char* MonitoringDevice::MESSAGE_TEXT_ZATVOR_OSHIBKA = LOCAL_MESSAGE_TEXT_ZATVOR_OSHIBKA;
char* MonitoringDevice::MESSAGE_TEXT_ZATVOR_OTKRIT = LOCAL_MESSAGE_TEXT_ZATVOR_OTKRIT;
char* MonitoringDevice::MESSAGE_TEXT_ZATVOR_ZAKRIT = LOCAL_MESSAGE_TEXT_ZATVOR_ZAKRIT;
char* MonitoringDevice::MESSAGE_TEXT_TEMPERATURA_SHU_VISHE_NORMI = LOCAL_MESSAGE_TEXT_TEMPERATURA_SHU_VISHE_NORMI;
char* MonitoringDevice::MESSAGE_TEXT_TEMPERATURA_SHU_VISHE_NORMI_V_NORME = LOCAL_MESSAGE_TEXT_TEMPERATURA_SHU_VISHE_NORMI_V_NORME;
char* MonitoringDevice::MESSAGE_TEXT_TEMPERATURA_SHU_NIZHE_NORMI = LOCAL_MESSAGE_TEXT_TEMPERATURA_SHU_NIZHE_NORMI;
char* MonitoringDevice::MESSAGE_TEXT_TEMPERATURA_SHU_NIZHE_NORMI_V_NORME = LOCAL_MESSAGE_TEXT_TEMPERATURA_SHU_NIZHE_NORMI_V_NORME;
char* MonitoringDevice::MESSAGE_TEXT_VISOKAJA_VLAZHNOST_VNUTRI_SHU = LOCAL_MESSAGE_TEXT_VISOKAJA_VLAZHNOST_VNUTRI_SHU;
char* MonitoringDevice::MESSAGE_TEXT_VISOKAJA_VLAZHNOST_VNUTRI_SHU_V_NORME = LOCAL_MESSAGE_TEXT_VISOKAJA_VLAZHNOST_VNUTRI_SHU_V_NORME;
char* MonitoringDevice::MESSAGE_TEXT_SREDNIY_TOK_MOTORA_VISHE_NORMI = LOCAL_MESSAGE_TEXT_SREDNIY_TOK_MOTORA_VISHE_NORMI;
char* MonitoringDevice::MESSAGE_TEXT_SREDNIY_TOK_MOTORA_VISHE_NORMI_V_NORME = LOCAL_MESSAGE_TEXT_SREDNIY_TOK_MOTORA_VISHE_NORMI_V_NORME;
char* MonitoringDevice::MESSAGE_TEXT_NEISPRAVNOST_DATCHIKA_TEMPERATURI_NA_PLATE = LOCAL_MESSAGE_TEXT_NEISPRAVNOST_DATCHIKA_TEMPERATURI_NA_PLATE;
char* MonitoringDevice::MESSAGE_TEXT_NEISPRAVNOST_DATCHIKA_TEMPERATURI_NA_PLATE_V_NORME = LOCAL_MESSAGE_TEXT_NEISPRAVNOST_DATCHIKA_TEMPERATURI_NA_PLATE_V_NORME;
char* MonitoringDevice::MESSAGE_TEXT_NEISPRAVNOST_DATCHIKA_TEMPERATURI_PRIVODA = LOCAL_MESSAGE_TEXT_NEISPRAVNOST_DATCHIKA_TEMPERATURI_PRIVODA;
char* MonitoringDevice::MESSAGE_TEXT_NEISPRAVNOST_DATCHIKA_TEMPERATURI_PRIVODA_V_NORME = LOCAL_MESSAGE_TEXT_NEISPRAVNOST_DATCHIKA_TEMPERATURI_PRIVODA_V_NORME;
char* MonitoringDevice::MESSAGE_TEXT_PEREZAPUSK_CP = LOCAL_MESSAGE_TEXT_PEREZAPUSK_CP;
char* MonitoringDevice::MESSAGE_TEXT_ZATVOR_NEISPRAVNOST = LOCAL_MESSAGE_TEXT_ZATVOR_NEISPRAVNOST;
char* MonitoringDevice::MESSAGE_TEXT_PR_DAVLENIE_EST = LOCAL_MESSAGE_TEXT_PR_DAVLENIE_EST;
char* MonitoringDevice::MESSAGE_TEXT_PR_DAVLENIE_NET = LOCAL_MESSAGE_TEXT_PR_DAVLENIE_NET;
char* MonitoringDevice::MESSAGE_TEXT_VOSSTANOVLENIE_DATCHIKOV_ZATVORA = LOCAL_MESSAGE_TEXT_VOSSTANOVLENIE_DATCHIKOV_ZATVORA;
char* MonitoringDevice::MESSAGE_TEXT_ZATVOR_V_NORME = LOCAL_MESSAGE_TEXT_ZATVOR_V_NORME;
char* MonitoringDevice::MESSAGE_TEXT_START_SEARCH = LOCAL_MESSAGE_TEXT_START_SEARCH;
char* MonitoringDevice::MESSAGE_TEXT_START_OROSHENIA = LOCAL_MESSAGE_TEXT_START_OROSHENIA;
char* MonitoringDevice::MESSAGE_TEXT_STOP_POISKA_OROSHENIA = LOCAL_MESSAGE_TEXT_STOP_POISKA_OROSHENIA;
char* MonitoringDevice::MESSAGE_TEXT_SIGNAL_O_VOZGORANII = LOCAL_MESSAGE_TEXT_SIGNAL_O_VOZGORANII;
char* MonitoringDevice::MESSAGE_TEXT_OTMENA_SIGNALA_O_VOZGORANII = LOCAL_MESSAGE_TEXT_OTMENA_SIGNALA_O_VOZGORANII;
char* MonitoringDevice::MESSAGE_TEXT_PEREGORELA_LAMPA = LOCAL_MESSAGE_TEXT_PEREGORELA_LAMPA;
char* MonitoringDevice::MESSAGE_TEXT_KZ_V_CEPI_LAMPI = LOCAL_MESSAGE_TEXT_KZ_V_CEPI_LAMPI;
char* MonitoringDevice::MESSAGE_TEXT_OTKAZ_POZICIONIROVANIJA = LOCAL_MESSAGE_TEXT_OTKAZ_POZICIONIROVANIJA;
char* MonitoringDevice::MESSAGE_TEXT_OTKAZ_OTKRITIJA = LOCAL_MESSAGE_TEXT_OTKAZ_OTKRITIJA;
char* MonitoringDevice::MESSAGE_TEXT_TEMPERATURA_V_NORME = LOCAL_MESSAGE_TEXT_TEMPERATURA_V_NORME;
char* MonitoringDevice::MESSAGE_TEXT_TEMPERATURA_NIZHE_NORMI = LOCAL_MESSAGE_TEXT_TEMPERATURA_NIZHE_NORMI;
char* MonitoringDevice::MESSAGE_TEXT_TEMPERATURA_VISHE_NORMI = LOCAL_MESSAGE_TEXT_TEMPERATURA_VISHE_NORMI;

char* MonitoringDevice::MESSAGE_TEXT_PENABAK_PERELIV = LOCAL_MESSAGE_TEXT_PENABAK_PERELIV;
char* MonitoringDevice::MESSAGE_TEXT_PENABAK_NORMA = LOCAL_MESSAGE_TEXT_PENABAK_NORMA;
char* MonitoringDevice::MESSAGE_TEXT_PENABAK_UTECHKA = LOCAL_MESSAGE_TEXT_PENABAK_UTECHKA;
char* MonitoringDevice::MESSAGE_TEXT_PENABAK_50_IZRASHODOVANO = LOCAL_MESSAGE_TEXT_PENABAK_50_IZRASHODOVANO;
char* MonitoringDevice::MESSAGE_TEXT_PENABAK_NAPOLOVINU_ZAPOLNEN = LOCAL_MESSAGE_TEXT_PENABAK_NAPOLOVINU_ZAPOLNEN;
char* MonitoringDevice::MESSAGE_TEXT_PENABAK_MIN_UROVEN = LOCAL_MESSAGE_TEXT_PENABAK_MIN_UROVEN;
char* MonitoringDevice::MESSAGE_TEXT_PENABAK_DATCHIK_NEISPRAVEN = LOCAL_MESSAGE_TEXT_PENABAK_DATCHIK_NEISPRAVEN;
char* MonitoringDevice::MESSAGE_TEXT_PENABAK_DATCHIK_ISPRAVEN = LOCAL_MESSAGE_TEXT_PENABAK_DATCHIK_ISPRAVEN;
char* MonitoringDevice::MESSAGE_TEXT_PENABAK_SVAZ_OFF = LOCAL_MESSAGE_TEXT_PENABAK_SVAZ_OFF;
char* MonitoringDevice::MESSAGE_TEXT_PENABAK_SVAZ_ON = LOCAL_MESSAGE_TEXT_PENABAK_SVAZ_ON;

char* MonitoringDevice::MESSAGE_TEXT_SCANER_MEHANIZM_GUT = LOCAL_MESSAGE_TEXT_SCANER_MEHANIZM_GUT;
char* MonitoringDevice::MESSAGE_TEXT_ZATVOR_VREMA_OTKRITIA_V_NORME = LOCAL_MESSAGE_TEXT_ZATVOR_VREMA_OTKRITIA_V_NORME;

char* MonitoringDevice::MESSAGE_TEXT_BLOKIROVKA_ZATVORA_ON = LOCAL_MESSAGE_TEXT_BLOKIROVKA_ZATVORA_ON;
char* MonitoringDevice::MESSAGE_TEXT_BLOKIROVKA_ZATVORA_OFF = LOCAL_MESSAGE_TEXT_BLOKIROVKA_ZATVORA_OFF;

#pragma endregion

MonitoringDevice::MonitoringDevice(unsigned char _address, unsigned int _type)
	:	IMonitoringDevice(_address, _type), setTimeTimeOut(0), isSetTimeTimeOut(false), eventId(0)
{
	ExtensionSystem::getSingleton().addReceiver(this);
	addReceiver(ExtensionSystem::getSingletonPtr());
}

MonitoringDevice::~MonitoringDevice()
{

}

void MonitoringDevice::init()
{
	phase = PHASE_CONFIG;
}

bool MonitoringDevice::isReady()
{
	return (phase == PHASE_START);
}

void MonitoringDevice::timerHandler()
{
	if (actionTimeOut++ == ACTION_TIME_OUT)
	{
		isActionTimeOut = true;
		actionTimeOut = 0;
	}

	if (setTimeTimeOut++ == SET_TIME_TIME_OUT)
	{
		isSetTimeTimeOut = true;
		setTimeTimeOut = 0;
	}
}

void MonitoringDevice::action()
{
	if (!disabled)
	{
		switch (phase)
		{
//11062013
			//case PHASE_CONFIG:
			//	DEBUG_PUT_METHOD("PHASE_CONFIG\n");
			//	createGetInitializeFrame();
			//	sendMessage(Message(MESSAGE_FROM_OFFSET_EXTENSION_SYSTEM, ExtensionSystem::EXTENSION_SYSTEM_MESSAGE_NEW_SEND_DATA, reinterpret_cast<unsigned int>(fifoFrame), ExtensionSystem::PACKING_OFF));
			//	isActionTimeOut = false;
			//	actionTimeOut = 0;
			//	phase = PHASE_CONFIG_0;
			//	break;
			//case PHASE_CONFIG_0:
			//	if (isActionTimeOut)
			//	{
			//		isActionTimeOut = false;
			//		phase = PHASE_CONFIG;
			//	}
			//	break;
			//case PHASE_CONFIG_1:
//11062013_	
			case PHASE_CONFIG:
				createInitFrame();
				sendMessage(Message(MESSAGE_FROM_OFFSET_EXTENSION_SYSTEM, ExtensionSystem::EXTENSION_SYSTEM_MESSAGE_NEW_SEND_DATA, reinterpret_cast<unsigned int>(fifoFrame), ExtensionSystem::PACKING_OFF));
				phase = PHASE_CONFIG_WAIT;
				break;
			case PHASE_INIT_WAIT:
				if (isActionTimeOut)
				{
					isActionTimeOut = false;
					createGetInitializeFrame();
					sendMessage(Message(MESSAGE_FROM_OFFSET_EXTENSION_SYSTEM, ExtensionSystem::EXTENSION_SYSTEM_MESSAGE_NEW_SEND_DATA, reinterpret_cast<unsigned int>(fifoFrame), ExtensionSystem::PACKING_OFF));
				}
				break;
			case PHASE_START:
				if (isActionTimeOut){
//11062013
//					if (obrivOffTest(LOCAL_MESSAGE_TEXT_EXT_SVAZ_OFF_MONITORING, address, 0)){
					obrivOffTest(LOCAL_MESSAGE_TEXT_EXT_SVAZ_OFF_MONITORING, address, 0);
					//DEBUG_PUT_METHOD("PHASE_START  obrivOffTest - true\n");
						//phase = PHASE_CONFIG;
						//return;
//					}
//11062013_	
					isActionTimeOut = false;
					if (fifoFrame->isEmpty())
						createGetEventsFrame();

					sendMessage(Message(MESSAGE_FROM_OFFSET_EXTENSION_SYSTEM, ExtensionSystem::EXTENSION_SYSTEM_MESSAGE_NEW_SEND_DATA, reinterpret_cast<unsigned int>(fifoFrame), ExtensionSystem::PACKING_OFF));
				}

				if (isSetTimeTimeOut)
				{
					isSetTimeTimeOut = false;
					createSetTimeFrame();
					sendMessage(Message(MESSAGE_FROM_OFFSET_EXTENSION_SYSTEM, ExtensionSystem::EXTENSION_SYSTEM_MESSAGE_NEW_SEND_DATA, reinterpret_cast<unsigned int>(fifoFrame), ExtensionSystem::PACKING_OFF));
				}
				break;
		}
	}
}

bool MonitoringDevice::putFrame(unsigned char* _pArea, bool isNotTransfer)
{
	if (!disabled)
	{
		if (isNotTransfer)
		{
			if (_pArea[0] == address)
			{
				switch (phase)
				{
					case PHASE_CONFIG_WAIT:
						break;
					case PHASE_INIT_WAIT:
						phase = PHASE_START;
						break;
				}
					
				return true;
			}
		}
		else
		{
			if (_pArea[1] == address)
			{
				switch (phase)
				{
//11062013
					case PHASE_CONFIG_0:
						DEBUG_PUT_METHOD("PHASE_CONFIG_0   _pArea[2] == COMMAND_GET_INITIALIZE_RESULT  _pArea[5] = %u   (INITIALIZE_RESULT_OK == 1)\n", _pArea[5]);
						if (_pArea[2] == COMMAND_GET_INITIALIZE_RESULT){
							if (_pArea[5] == INITIALIZE_RESULT_OK){
								phase = PHASE_START;
							}
							else
								phase = PHASE_CONFIG_1;
						}
						break;
//11062013_
					case PHASE_CONFIG_WAIT:
						if (_pArea[2] == COMMAND_INIT)
							phase = PHASE_INIT_WAIT;
						break;
					case PHASE_INIT_WAIT:
						if (_pArea[2] == COMMAND_GET_INITIALIZE_RESULT)
						{
							if (_pArea[5] == INITIALIZE_RESULT_OK)
								phase = PHASE_START;
						}
						break;
					case PHASE_START:
						obrivOnTest(LOCAL_MESSAGE_TEXT_EXT_SVAZ_ON_MONITORING, address, 0);
						if (_pArea[2] == COMMAND_GET_EVENTS)
							commandGetEvent(_pArea);
						break;
				}

				return true;
			}
		}
	}

	return false;
}

void MonitoringDevice::onMessage(Message message)
{
}

#pragma region createFrames

void MonitoringDevice::createInitFrame()
{
	ConfigData* pConfigData = Config::getSingleton().getConfigData();

	unsigned int prCount = pConfigData->getConfigDataStructPRPositionCount();
	ConfigDataStructPRPosition** prPositionStruct = pConfigData->getConfigDataStructPRPositions();

	unsigned int penaBakCount = pConfigData->getConfigDataStructPenaBakCount();
	ConfigDataStructPenaBak** penaBakStruct = pConfigData->getConfigDataStructPenaBak();
	
//	unsigned int dataLength = 10 + prCount * 4 + 1 + penaBakCount * 4; 
	unsigned int dataLength = 10 + 1 + prCount * 5 + 1 + penaBakCount * 4;
	unsigned char* initData = new unsigned char[dataLength + 7];

	ConfigDataStructConst* constStruct = pConfigData->getConfigDataStructConst();
	
	initData[0] = address;
	initData[1] = 0;
	initData[2] = COMMAND_INIT;
	initData[3] = dataLength;
	initData[4] = dataLength >> 8;

	const unsigned int INIT_YEAR_OFFSET = 5;
	const unsigned int INIT_MONTH_OFFSET = 6;
	const unsigned int INIT_DAY_OFFSET = 7;
	const unsigned int INIT_HOUR_OFFSET = 8;
	const unsigned int INIT_MINUTE_OFFSET = 9;
	const unsigned int INIT_SECOND_OFFSET = 10;
	const unsigned int HOUR_OFFSET = 11;
	const unsigned int MINUTE_OFFSET = 12;
	const unsigned int TESTING_INFO_OFFSET = 13;
	const unsigned int BK_ZATVOR_OFFSET = 14;
	const unsigned int PR_COUNT_OFFSET = 15;
	const unsigned int FIRST_PR_INFO_OFFSET = 16;
	
	Clock::DateTime dt = Clock::getSingleton().getClock2();
	
	initData[INIT_YEAR_OFFSET] = dt.year;
	initData[INIT_MONTH_OFFSET] = dt.month;
	initData[INIT_DAY_OFFSET] = dt.day;
	initData[INIT_HOUR_OFFSET] = dt.hour;
	initData[INIT_MINUTE_OFFSET] = dt.minute;
	initData[INIT_SECOND_OFFSET] = dt.second;

	initData[HOUR_OFFSET] = constStruct->testingHour;
	initData[MINUTE_OFFSET] = constStruct->testingMinute;
	initData[TESTING_INFO_OFFSET] = constStruct->permissionTestingInfo;
	initData[BK_ZATVOR_OFFSET] = 0;

	initData[PR_COUNT_OFFSET] = prCount;

	for (unsigned int i = 0; i < prCount; i++)
	{
		initData[FIRST_PR_INFO_OFFSET + i * 5] = prPositionStruct[i]->projectNumber;
		initData[FIRST_PR_INFO_OFFSET + i * 5 + 1] = prPositionStruct[i]->address;
		initData[FIRST_PR_INFO_OFFSET + i * 5 + 2] = prPositionStruct[i]->networkIndexNumber;
		initData[FIRST_PR_INFO_OFFSET + i * 5 + 3] = 0;
		initData[FIRST_PR_INFO_OFFSET + i * 5 + 4] = 1;
	}

	initData[FIRST_PR_INFO_OFFSET + prCount * 5] = penaBakCount;

	for (unsigned int i = 0; i < penaBakCount; ++i){
		initData[FIRST_PR_INFO_OFFSET + prCount * 5 + i * 4 + 1] = penaBakStruct[i]->number;
		initData[FIRST_PR_INFO_OFFSET + prCount * 5 + i * 4 + 1 + 1] = penaBakStruct[i]->level;
		initData[FIRST_PR_INFO_OFFSET + prCount * 5 + i * 4 + 2 + 1] = penaBakStruct[i]->address;
		initData[FIRST_PR_INFO_OFFSET + prCount * 5 + i * 4 + 3 + 1] = penaBakStruct[i]->numberOnDevice;
	}
	fifoFrame->put(&initData);
}

void MonitoringDevice::createGetInitializeFrame()
{
	unsigned char* initData = new unsigned char[7];
	
	initData[0] = address;
	initData[1] = 0;
	initData[2] = COMMAND_GET_INITIALIZE_RESULT;
	initData[3] = 0;
	initData[4] = 0;

	fifoFrame->put(&initData);
}

void MonitoringDevice::createSetTimeFrame()
{
	unsigned char* initData = new unsigned char[7 + 6];
	
	initData[0] = address;
	initData[1] = 0;
	initData[2] = COMMAND_SET_TIME;
	initData[3] = 6;
	initData[4] = 0;

	Clock::DateTime dt = Clock::getSingleton().getClock2();
	
	initData[5] = dt.year;
	initData[6] = dt.month;
	initData[7] = dt.day;
	initData[8] = dt.hour;
	initData[9] = dt.minute;
	initData[10] = dt.second;

	fifoFrame->put(&initData);
}

void MonitoringDevice::createGetEventsFrame()
{
	unsigned char* initData = new unsigned char[7 + 3];
	
//	DEBUG_PUT_METHOD("id = %u\n", eventId);

	initData[0] = address;
	initData[1] = 0;
	initData[2] = COMMAND_GET_EVENTS;
	initData[3] = 3;
	initData[4] = 0;
	initData[5] = static_cast<unsigned char>(eventId);
	initData[6] = eventId >> 8;
	initData[7] = 0;

	fifoFrame->put(&initData);
}

#pragma endregion

unsigned int MonitoringDevice::getId()
{
	return ID;
}

void MonitoringDevice::commandGetEvent(unsigned char* _pArea)
{
	const unsigned int EVENT_ID_OFFSET = 5;
	const unsigned int EVENT_FAULT_OFFSET = 7;
	const unsigned int EVENT_COUNT_OFFSET = 8;

	setFault(_pArea[EVENT_FAULT_OFFSET]);

	unsigned short _eventId = _pArea[EVENT_ID_OFFSET] + _pArea[EVENT_ID_OFFSET + 1] * 256;

//	DEBUG_PUT_METHOD("id = %u, rId = %u\n", eventId, _eventId);

	if (_eventId != eventId)
	{
		if (_pArea[EVENT_COUNT_OFFSET] != 0)
		{
			if (!SerialDebug::getSingleton().isOn())
				redirectToPc(_pArea);

			unsigned int messageCount = firstMessage(&_pArea);
			for (unsigned int i = 0; i < messageCount; ++i){
				if (isEventMessage(_pArea)){

//DEBUG_PUT_METHOD(" %i, %i, %i, %i, %i\n", _pArea[1], _pArea[9], _pArea[10], _pArea[11], _pArea[12])

					toLog(_pArea);
					setOutputs(_pArea);
				}else{
					if(isControlMessage(_pArea)){
						controlMessage(_pArea);
					}
				}

				nextMessage(&_pArea);
			}
		}
	}

	eventId = _eventId;	
}

void MonitoringDevice::setFault(unsigned char value)
{
	static unsigned char prevValue = 98;

	if (prevValue != value)
	{
		if (value == 0)
			IOSubsystem::getSingleton().disableSystemFaultOutputs();
		else
			IOSubsystem::getSingleton().enableSystemFaultOutputs();

		prevValue = value;
	}
}

void MonitoringDevice::toLog(unsigned char* pMsg)
{
	if (isLogingMessage(pMsg))
	{
		char* messageText = nullptr;
		unsigned char parameter1 = 0;
		unsigned char parameter2 = 0;
		getMessageInfo(pMsg, &messageText, &parameter1, &parameter2);
		Log::getSingleton().add(LOG_MESSAGE_FROM_MONITORING, LOG_MESSAGE_TYPE_MESSAGE, reinterpret_cast<char*>(messageText), parameter1, parameter2);
	}
}

unsigned int MonitoringDevice::firstMessage(unsigned char** _ppArea)
{
	const unsigned int MESSAGE_COUNT_OFFSET = 8;
	const unsigned int FIRST_MESSAGE_OFFSET = 9;

	unsigned int messageCount = (*_ppArea)[MESSAGE_COUNT_OFFSET];
	if (messageCount != 0)
		(*_ppArea) += FIRST_MESSAGE_OFFSET;

	return messageCount;
}

void MonitoringDevice::nextMessage(unsigned char** _ppArea)
{
	const unsigned int MESSAGE_TYPE_1 = 1;
	const unsigned int MESSAGE_TYPE_2 = 2;
	const unsigned int MESSAGE_TYPE_3 = 3;
	const unsigned int MESSAGE_TYPE_4 = 4;

	const unsigned int MESSAGE_SIZE_TYPE_1 = 12;
	const unsigned int MESSAGE_SIZE_TYPE_2 = 13;
	const unsigned int MESSAGE_SIZE_TYPE_3 = 11;
	const unsigned int MESSAGE_SIZE_TYPE_4 = 10;

	if (getMessageType(*_ppArea) == MESSAGE_TYPE_2)
		*_ppArea += MESSAGE_SIZE_TYPE_2;
	else
		if (getMessageType(*_ppArea) == MESSAGE_TYPE_1)
			*_ppArea += MESSAGE_SIZE_TYPE_1;
		else
			if (getMessageType(*_ppArea) == MESSAGE_TYPE_3)
				*_ppArea += MESSAGE_SIZE_TYPE_3;
			else
				*_ppArea += MESSAGE_SIZE_TYPE_4;
}

unsigned int MonitoringDevice::getMessageType(unsigned char* _pArea)
{
	const unsigned int MESSAGE_TYPE_OFFSET = 0;

	return _pArea[MESSAGE_TYPE_OFFSET];
}

bool MonitoringDevice::isEventMessage(unsigned char* _pArea)
{
	const unsigned int MESSAGE_TYPE_2 = 2;

	return (getMessageType(_pArea) == MESSAGE_TYPE_2);
}

bool MonitoringDevice::isControlMessage(unsigned char* _pArea)
{
	const unsigned int MESSAGE_TYPE_1 = 1;

	return (getMessageType(_pArea) == MESSAGE_TYPE_1);
}

void MonitoringDevice::redirectToPc(unsigned char* _pArea)
{
	const unsigned int LENGTH_HIGH_BYTE_OFFSET = 4;
	const unsigned int LENGTH_LOW_BYTE_OFFSET = 3;
	Config::getSingleton().sendMessageToPort(_pArea, _pArea[LENGTH_HIGH_BYTE_OFFSET] * 256 + _pArea[LENGTH_LOW_BYTE_OFFSET] + 7);
}

void MonitoringDevice::setOutputs(unsigned char* pMsg)
{
	const unsigned int MESSAGE_CODE_OFFSET = 1;
	const unsigned int MESSAGE_PAR1_OFFSET = 9;
	const unsigned int MESSAGE_PAR2_OFFSET = 10;
	const unsigned int MESSAGE_PAR3_OFFSET = 11;
	const unsigned int MESSAGE_PAR4_OFFSET = 12;

	switch (pMsg[MESSAGE_CODE_OFFSET])
	{
		case MESSAGE_NUMBER_PR_DAVLENIE_EST:
			IOSubsystem::getSingleton().enablePrPressureOutputs(pMsg[MESSAGE_PAR2_OFFSET]);
			break;
		case MESSAGE_NUMBER_PR_DAVLENIE_NET:
			IOSubsystem::getSingleton().disablePrPressureOutputs(pMsg[MESSAGE_PAR2_OFFSET]);
			break;
		case MESSAGE_NUMBER_ZATVOR_OTKRIT:
			IOSubsystem::getSingleton().enableGateOutputs(pMsg[MESSAGE_PAR2_OFFSET]);
			break;
		case MESSAGE_NUMBER_ZATVOR_ZAKRIT:
			IOSubsystem::getSingleton().disableGateOutputs(pMsg[MESSAGE_PAR2_OFFSET]);
			break;
		case MESSAGE_NUMBER_ZATVOR_OSHIBKA:
			IOSubsystem::getSingleton().enableGateFaultOutputs(pMsg[MESSAGE_PAR2_OFFSET]);
			break;
		case MESSAGE_NUMBER_ZATVOR_NEISPRAVNOST:
			IOSubsystem::getSingleton().enableGateFaultOutputs(pMsg[MESSAGE_PAR2_OFFSET]);
			break;
		case MESSAGE_NUMBER_SVAZ_S_PLATOY_PRIVODA_NET:
		case MESSAGE_NUMBER_PONIZHENNOE_NAPRAZHENIE_NA_KONDENSATORAH:
			IOSubsystem::getSingleton().enablePrFaultOutputs(pMsg[MESSAGE_PAR2_OFFSET]);
			break;
		case MESSAGE_NUMBER_SVAZ_S_PLATOY_PRIVODA_EST:
		case MESSAGE_NUMBER_NAPRAZHENIE_NA_KONDENSATORAH_V_NORME:
			IOSubsystem::getSingleton().disablePrFaultOutputs(pMsg[MESSAGE_PAR2_OFFSET]);
			break;
	}
}

bool MonitoringDevice::isLogingMessage(unsigned char* pMsg)
{
	const unsigned int MESSAGE_CODE_OFFSET = 1;

	bool result = true;

	switch (pMsg[MESSAGE_CODE_OFFSET])
	{
		case MESSAGE_NUMBER_START_SEARCH:
		case MESSAGE_NUMBER_START_OROSHENIA:
		case MESSAGE_NUMBER_STOP_POISKA_OROSHENIA:
		case MESSAGE_NUMBER_SIGNAL_O_VOZGORANII:
		case MESSAGE_NUMBER_OTMENA_SIGNALA_O_VOZGORANII:
		case MESSAGE_NUMBER_LAMPA_NORMA:
		case MESSAGE_NUMBER_POZICIONIROVANIE_VIPOLNENO:
		case MESSAGE_NUMBER_KORPUS_ZAKRIT:
		case MESSAGE_NUMBER_KORPUS_OTKRIT:
		case MESSAGE_NUMBER_DUMMY:
		case MESSAGE_NUMBER_START_PROGRAMMI_SCANIROVANIJA:
		case MESSAGE_NUMBER_PEREZAGRUZKA_CP:
		case MESSAGE_NUMBER_NIZKOE_NAPRAZHENIE_PITANIA_CP:
		case MESSAGE_NUMBER_NAPRAZHENIE_PITANIA_CP_V_NORME:
		case MESSAGE_NUMBER_NIZKOE_NAPRAZHENIE_PITANIA_DRAIVERA:
		case MESSAGE_NUMBER_VISOKOE_NAPRAZHENIE_PITANIA_DRAIVERA:
		case MESSAGE_NUMBER_OSHIBKA_TESTA_FLESH_PAMATI:
		case MESSAGE_NUMBER_TEST_FLASH_PAMATI_VIPOLNEN_USPESHNO:
		case MESSAGE_NUMBER_TEMPERATURA_SHU_VISHE_NORMI:
		case MESSAGE_NUMBER_TEMPERATURA_SHU_NIZHE_NORMI:
		case MESSAGE_NUMBER_TEMPERATURA_SHU_V_NORME:
		case MESSAGE_NUMBER_VISOKAJA_VLAZHNOST_VNUTRI_SHU:
		case MESSAGE_NUMBER_VISOKAJA_VLAZHNOST_VNUTRI_SHU_V_NORME:
		case MESSAGE_NUMBER_TEMPERATURA_V_NORME:
		case MESSAGE_NUMBER_TEMPERATURA_NIZHE_NORMI:
		case MESSAGE_NUMBER_TEMPERATURA_VISHE_NORMI:
		case MESSAGE_NUMBER_PENABAK_UROVEN_VISHE_MIN:
		case MESSAGE_NUMBER_NAPRAZHENIE_PITANIA_DRAIVERA_V_NORME:
		case MESSAGE_NUMBER_OSHIBKA_OPREDELENIA_NAPRAVLENIA:
		case MESSAGE_NUMBER_VERNOE_OPREDELENIE_NAPRAVLENIA:
		case MESSAGE_NUMBER_NIZKOE_NAPRAZHENIE_PITANIA_PLATI:
		case MESSAGE_NUMBER_VISOKOE_NAPRAZHENIE_PITANIA_PLATI:
		case MESSAGE_NUMBER_NAPRAZHENIE_PITANIA_PLATI_V_NORME:
			
			result = false;
			break;
	}

	return result;
}

void MonitoringDevice::getMessageInfo(unsigned char* pMsg, char** text, unsigned char* parameter1, unsigned char*parameter2)
{
	const unsigned int MESSAGE_CODE_OFFSET = 1;
	const unsigned int MESSAGE_PAR1_OFFSET = 9;
	const unsigned int MESSAGE_PAR2_OFFSET = 10;
	const unsigned int MESSAGE_PAR3_OFFSET = 11;
	const unsigned int MESSAGE_PAR4_OFFSET = 12;

	switch (pMsg[MESSAGE_CODE_OFFSET])
	{
		case MESSAGE_NUMBER_SVAZ_S_PLATOY_PRIVODA_NET:
			*text = const_cast<char*>(MESSAGE_TEXT_SVAZ_S_PLATOY_PRIVODA_NET);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_SVAZ_S_PLATOY_PRIVODA_EST:
			*text = const_cast<char*>(MESSAGE_TEXT_SVAZ_S_PLATOY_PRIVODA_EST);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_PONIZHENNOE_NAPRAZHENIE_NA_KONDENSATORAH:
			*text = const_cast<char*>(MESSAGE_TEXT_PONIZHENNOE_NAPRAZHENIE_NA_KONDENSATORAH);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_NAPRAZHENIE_NA_KONDENSATORAH_V_NORME:
			*text = const_cast<char*>(MESSAGE_TEXT_NAPRAZHENIE_NA_KONDENSATORAH_V_NORME);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_NEISPRAVNOST_SILIVOGO_MOSTA:
			*text = const_cast<char*>(MESSAGE_TEXT_NEISPRAVNOST_SILIVOGO_MOSTA);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_VOSSTANOVLENIE_SILOVOGO_MOSTA:
			*text = const_cast<char*>(MESSAGE_TEXT_VOSSTANOVLENIE_SILOVOGO_MOSTA);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_NIZKOE_NAPRAZHENIE_PITANIA_CP:
			*text = const_cast<char*>(MESSAGE_TEXT_NIZKOE_NAPRAZHENIE_PITANIA_CP);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_NAPRAZHENIE_PITANIA_CP_V_NORME:
			*text = const_cast<char*>(MESSAGE_TEXT_NAPRAZHENIE_PITANIA_CP_V_NORME);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_NIZKOE_NAPRAZHENIE_PITANIA_DRAIVERA:
			*text = const_cast<char*>(MESSAGE_TEXT_NIZKOE_NAPRAZHENIE_PITANIA_DRAIVERA);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_VISOKOE_NAPRAZHENIE_PITANIA_DRAIVERA:
			*text = const_cast<char*>(MESSAGE_TEXT_VISOKOE_NAPRAZHENIE_PITANIA_DRAIVERA);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_NIZKOE_NAPRAZHENIE_PITANIA_PLATI:
			*text = const_cast<char*>(MESSAGE_TEXT_NIZKOE_NAPRAZHENIE_PITANIA_PLATI);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_VISOKOE_NAPRAZHENIE_PITANIA_PLATI:
			*text = const_cast<char*>(MESSAGE_TEXT_VISOKOE_NAPRAZHENIE_PITANIA_PLATI);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_NAPRAZHENIE_PITANIA_PLATI_V_NORME:
			*text = const_cast<char*>(LOCAL_MESSAGE_TEXT_VISOKOE_NAPRAZHENIE_PITANIA_PLATI_V_NORME);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_OSHIBKA_TESTA_FLESH_PAMATI:
			*text = const_cast<char*>(MESSAGE_TEXT_OSHIBKA_TESTA_FLESH_PAMATI);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_TEST_FLASH_PAMATI_VIPOLNEN_USPESHNO:
			*text = const_cast<char*>(MESSAGE_TEXT_TEST_FLASH_PAMATI_VIPOLNEN_USPESHNO);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_UROVNI_SIGNALOV_ENKODERA_NE_V_NORME:
			*text = const_cast<char*>(MESSAGE_TEXT_UROVNI_SIGNALOV_ENKODERA_NE_V_NORME);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_UROVNI_SIGNALOV_ENKODERA_V_NORME:
			*text = const_cast<char*>(MESSAGE_TEXT_UROVNI_SIGNALOV_ENKODERA_V_NORME);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_NEISPRAVNOST_KANALA_ENKODERA:
			*text = const_cast<char*>(MESSAGE_TEXT_NEISPRAVNOST_KANALA_ENKODERA);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_VOSSTANOVLENIE_KANALA_ENKODERA:
			*text = const_cast<char*>(MESSAGE_TEXT_VOSSTANOVLENIE_KANALA_ENKODERA);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_OSHIBKA_OPREDELENIA_NAPRAVLENIA:
			*text = const_cast<char*>(MESSAGE_TEXT_OSHIBKA_OPREDELENIA_NAPRAVLENIA);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_VERNOE_OPREDELENIE_NAPRAVLENIA:
			*text = const_cast<char*>(MESSAGE_TEXT_VERNOE_OPREDELENIE_NAPRAVLENIA);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_PRI_DVIZHENII_NE_MENAETSA_KOORDINATA:
			*text = const_cast<char*>(MESSAGE_TEXT_PRI_DVIZHENII_NE_MENAETSA_KOORDINATA);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_PRI_DVIZHENII_MENAETSA_KOORDINATA:
			*text = const_cast<char*>(MESSAGE_TEXT_PRI_DVIZHENII_MENAETSA_KOORDINATA);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_OTSUTSTVIE_TOKA_MOTORA:
			*text = const_cast<char*>(MESSAGE_TEXT_OTSUTSTVIE_TOKA_MOTORA);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_TOK_MOTORA_V_PREDELAH_NORMI:
			*text = const_cast<char*>(MESSAGE_TEXT_TOK_MOTORA_V_PREDELAH_NORMI);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_AVARIYNAYA_AMPLITUDE_TOKA_MOTORA:
			*text = const_cast<char*>(MESSAGE_TEXT_AVARIYNAYA_AMPLITUDE_TOKA_MOTORA);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_AMPLITUDA_TOKA_MOTORA_V_NORME:
			*text = const_cast<char*>(MESSAGE_TEXT_AMPLITUDA_TOKA_MOTORA_V_NORME);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_OBRIV_DATCHIKA_DAVLENIA:
			*text = const_cast<char*>(MESSAGE_TEXT_OBRIV_DATCHIKA_DAVLENIA);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_KZ_DATCHIKA_DAVLENIA:
			*text = const_cast<char*>(MESSAGE_TEXT_KZ_DATCHIKA_DAVLENIA);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_PEREZAGRUZKA_CP:
			*text = const_cast<char*>(MESSAGE_TEXT_PEREZAGRUZKA_CP);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_SVAZ_S_PR_VOSSTANOVLENA:
			*text = const_cast<char*>(MESSAGE_TEXT_SVAZ_S_PR_VOSSTANOVLENA);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_OTSUTSTVUET_SVAZ_S_PR:
			*text = const_cast<char*>(MESSAGE_TEXT_OTSUTSTVUET_SVAZ_S_PR);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_OBRIV_MAGISTRALI_RPK:
			*text = const_cast<char*>(MESSAGE_TEXT_OBRIV_MAGISTRALI_RPK);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR4_OFFSET];
			break;
		case MESSAGE_NUMBER_MAGISTRAL_RPK_VOSSTANOVLENA:
			*text = const_cast<char*>(MESSAGE_TEXT_MAGISTRAL_RPK_VOSSTANOVLENA);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_OTSUTSTVUET_SVAZ_S_SK:
			*text = const_cast<char*>(MESSAGE_TEXT_OTSUTSTVUET_SVAZ_S_SK);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_SVAZ_S_SK_VOSSTANOVLENA:
			*text = const_cast<char*>(MESSAGE_TEXT_SVAZ_S_SK_VOSSTANOVLENA);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_OBRIV_SVAZI_S_ZATVOROM:
			*text = const_cast<char*>(MESSAGE_TEXT_OBRIV_SVAZI_S_ZATVOROM);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_SVAZ_S_ZATVOROM_VOSSTANOVLENA:
			*text = const_cast<char*>(MESSAGE_TEXT_SVAZ_S_ZATVOROM_VOSSTANOVLENA);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_OBRIV_DATCHIKA_TEMPERATURI:
			*text = const_cast<char*>(MESSAGE_TEXT_OBRIV_DATCHIKA_TEMPERATURI);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_KZ_DATCHIKA_TEMPERATURI:
			*text = const_cast<char*>(MESSAGE_TEXT_KZ_DATCHIKA_TEMPERATURI);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_OBRIV_DATCHIKA_VLAGNOSTI:
			*text = const_cast<char*>(MESSAGE_TEXT_OBRIV_DATCHIKA_VLAGNOSTI);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_OBRIV_DATCHIKA_VLAGNOSTI_ISPRAVEN:
			*text = const_cast<char*>(MESSAGE_TEXT_OBRIV_DATCHIKA_VLAGNOSTI_ISPRAVEN);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_KZ_KNOPOK_VNESHNEGO_POSTA:
			*text = const_cast<char*>(MESSAGE_TEXT_KZ_KNOPOK_VNESHNEGO_POSTA);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_KZ_KNOPOK_VNESHNEGO_POSTA_VOSST:
			*text = const_cast<char*>(MESSAGE_TEXT_KZ_KNOPOK_VNESHNEGO_POSTA_VOSST);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_OBRIV_KNOPOK_VNESHNEGO_POSTA:
			*text = const_cast<char*>(MESSAGE_TEXT_OBRIV_KNOPOK_VNESHNEGO_POSTA);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_OBRIV_KNOPOK_VNESHNEGO_POSTA_VOSST:
			*text = const_cast<char*>(MESSAGE_TEXT_OBRIV_KNOPOK_VNESHNEGO_POSTA_VOSST);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_VOSSTANOVLENIE_SOEDINITELNIH_LINIY:
			*text = const_cast<char*>(MESSAGE_TEXT_VOSSTANOVLENIE_SOEDINITELNIH_LINIY);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_KZ_SOEDINITELNOY_LINII:
			*text = const_cast<char*>(MESSAGE_TEXT_KZ_SOEDINITELNOY_LINII);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_OBRIV_SOEDINITELNOY_LINII:
			*text = const_cast<char*>(MESSAGE_TEXT_OBRIV_SOEDINITELNOY_LINII);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_OSHIBKA_OPREDELENIA_SOSTOYANIA_SL:
			*text = const_cast<char*>(MESSAGE_TEXT_OSHIBKA_OPREDELENIA_SOSTOYANIA_SL);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_SVAZ_VOSSTANOVLENA:
			*text = const_cast<char*>(MESSAGE_TEXT_SVAZ_VOSSTANOVLENA);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_OTSUTSTVUET_SVAZ:
			*text = const_cast<char*>(MESSAGE_TEXT_OTSUTSTVUET_SVAZ);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_IZVECHATEL_V_NORME:
			*text = const_cast<char*>(MESSAGE_TEXT_IZVECHATEL_V_NORME);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_NET_SVAZI_S_IZVECHATELEM:
			*text = const_cast<char*>(MESSAGE_TEXT_NET_SVAZI_S_IZVECHATELEM);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_VNUTRENNAYA_OSHIBKA_IZVECHATELA:
			*text = const_cast<char*>(MESSAGE_TEXT_VNUTRENNAYA_OSHIBKA_IZVECHATELA);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_ZATVOR_OSHIBKA:
			*text = const_cast<char*>(MESSAGE_TEXT_ZATVOR_OSHIBKA);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_ZATVOR_OTKRIT:
			*text = const_cast<char*>(MESSAGE_TEXT_ZATVOR_OTKRIT);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_ZATVOR_ZAKRIT:
			*text = const_cast<char*>(MESSAGE_TEXT_ZATVOR_ZAKRIT);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_TEMPERATURA_SHU_VISHE_NORMI:
			*text = const_cast<char*>(MESSAGE_TEXT_TEMPERATURA_SHU_VISHE_NORMI);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_TEMPERATURA_SHU_NIZHE_NORMI:
			*text = const_cast<char*>(MESSAGE_TEXT_TEMPERATURA_SHU_NIZHE_NORMI);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_TEMPERATURA_SHU_V_NORME:
			*text = const_cast<char*>(MESSAGE_TEXT_TEMPERATURA_SHU_NIZHE_NORMI_V_NORME);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_VISOKAJA_VLAZHNOST_VNUTRI_SHU:
			*text = const_cast<char*>(MESSAGE_TEXT_VISOKAJA_VLAZHNOST_VNUTRI_SHU);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_VISOKAJA_VLAZHNOST_VNUTRI_SHU_V_NORME:
			*text = const_cast<char*>(MESSAGE_TEXT_VISOKAJA_VLAZHNOST_VNUTRI_SHU_V_NORME);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_SREDNIY_TOK_MOTORA_VISHE_NORMI:
			*text = const_cast<char*>(MESSAGE_TEXT_SREDNIY_TOK_MOTORA_VISHE_NORMI);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_SREDNIY_TOK_MOTORA_VISHE_NORMI_V_NORME:
			*text = const_cast<char*>(MESSAGE_TEXT_SREDNIY_TOK_MOTORA_VISHE_NORMI_V_NORME);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_NEISPRAVNOST_DATCHIKA_TEMPERATURI_NA_PLATE:
			*text = const_cast<char*>(MESSAGE_TEXT_NEISPRAVNOST_DATCHIKA_TEMPERATURI_NA_PLATE);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_NEISPRAVNOST_DATCHIKA_TEMPERATURI_NA_PLATE_V_NORME:
			*text = const_cast<char*>(MESSAGE_TEXT_NEISPRAVNOST_DATCHIKA_TEMPERATURI_NA_PLATE_V_NORME);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_NEISPRAVNOST_DATCHIKA_TEMPERATURI_PRIVODA:
			*text = const_cast<char*>(MESSAGE_TEXT_NEISPRAVNOST_DATCHIKA_TEMPERATURI_PRIVODA);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_NEISPRAVNOST_DATCHIKA_TEMPERATURI_PRIVODA_V_NORME:
			*text = const_cast<char*>(MESSAGE_TEXT_NEISPRAVNOST_DATCHIKA_TEMPERATURI_PRIVODA_V_NORME);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_ZATVOR_NEISPRAVNOST:
			*text = const_cast<char*>(MESSAGE_TEXT_ZATVOR_NEISPRAVNOST);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_PR_DAVLENIE_EST:
			*text = const_cast<char*>(MESSAGE_TEXT_PR_DAVLENIE_EST);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_PR_DAVLENIE_NET:
			*text = const_cast<char*>(MESSAGE_TEXT_PR_DAVLENIE_NET);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_VOSSTANOVLENIE_DATCHIKOV_ZATVORA:
			*text = const_cast<char*>(MESSAGE_TEXT_VOSSTANOVLENIE_DATCHIKOV_ZATVORA);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_PEREGORELA_LAMPA:
			*text = const_cast<char*>(MESSAGE_TEXT_PEREGORELA_LAMPA);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_KZ_V_CEPI_LAMPI:
			*text = const_cast<char*>(MESSAGE_TEXT_KZ_V_CEPI_LAMPI);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_OTKAZ_POZICIONIROVANIJA:
			*text = const_cast<char*>(MESSAGE_TEXT_OTKAZ_POZICIONIROVANIJA);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_OTKAZ_OTKRITIJA:
			*text = const_cast<char*>(MESSAGE_TEXT_OTKAZ_OTKRITIJA);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_TEMPERATURA_NIZHE_NORMI:
			*text = const_cast<char*>(MESSAGE_TEXT_TEMPERATURA_NIZHE_NORMI);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_TEMPERATURA_V_NORME:
			*text = const_cast<char*>(MESSAGE_TEXT_TEMPERATURA_V_NORME);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_TEMPERATURA_VISHE_NORMI:
			*text = const_cast<char*>(MESSAGE_TEXT_TEMPERATURA_VISHE_NORMI);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_PENABAK_SVAZ_OFF:
			*text = const_cast<char*>(MESSAGE_TEXT_PENABAK_SVAZ_OFF);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = 0;//pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_PENABAK_SVAZ_ON:
			*text = const_cast<char*>(MESSAGE_TEXT_PENABAK_SVAZ_ON);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = 0;//pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_PENABAK_PERELIV:
			*text = const_cast<char*>(MESSAGE_TEXT_PENABAK_PERELIV);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_PENABAK_NORMA:
			*text = const_cast<char*>(MESSAGE_TEXT_PENABAK_NORMA);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_PENABAK_UTECHKA:
			*text = const_cast<char*>(MESSAGE_TEXT_PENABAK_UTECHKA);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_PENABAK_50_IZRASHODOVANO:
			*text = const_cast<char*>(MESSAGE_TEXT_PENABAK_50_IZRASHODOVANO);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_PENABAK_NAPOLOVINU_ZAPOLNEN:
			*text = const_cast<char*>(MESSAGE_TEXT_PENABAK_NAPOLOVINU_ZAPOLNEN);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_PENABAK_MIN_UROVEN:
			*text = const_cast<char*>(MESSAGE_TEXT_PENABAK_MIN_UROVEN);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_PENABAK_DATCHIK_NEISPRAVEN:
			*text = const_cast<char*>(MESSAGE_TEXT_PENABAK_DATCHIK_NEISPRAVEN);
			*parameter1 = pMsg[MESSAGE_PAR3_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR4_OFFSET];
			break;
		case MESSAGE_NUMBER_PENABAK_DATCHIK_ISPRAVEN:
			*text = const_cast<char*>(MESSAGE_TEXT_PENABAK_DATCHIK_ISPRAVEN);
			*parameter1 = pMsg[MESSAGE_PAR3_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR4_OFFSET];
			break;
		case MESSAGE_NUMBER_SCANER_MEHANIZM_GUT:
			*text = const_cast<char*>(MESSAGE_TEXT_SCANER_MEHANIZM_GUT);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_ZATVOR_VREMA_OTKRITIA_V_NORME:
			*text = const_cast<char*>(MESSAGE_TEXT_ZATVOR_VREMA_OTKRITIA_V_NORME);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_BLOKIROVKA_ZATVORA_ON:
			*text = const_cast<char*>(MESSAGE_TEXT_BLOKIROVKA_ZATVORA_ON);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		case MESSAGE_NUMBER_BLOKIROVKA_ZATVORA_OFF:
			*text = const_cast<char*>(MESSAGE_TEXT_BLOKIROVKA_ZATVORA_OFF);
			*parameter1 = pMsg[MESSAGE_PAR2_OFFSET];
			*parameter2 = pMsg[MESSAGE_PAR3_OFFSET];
			break;
		default:
//			DEBUG_PUT_METHOD("default: %i %i %i\n", pMsg[MESSAGE_CODE_OFFSET], pMsg[MESSAGE_PAR2_OFFSET], pMsg[MESSAGE_PAR3_OFFSET])
			*text = "...";//const_cast<char*>("FIG ZNAET CHTO");
			*parameter1 = pMsg[MESSAGE_CODE_OFFSET] / 100;
			*parameter2 = pMsg[MESSAGE_CODE_OFFSET] % 100;
			break;
	}
}

void MonitoringDevice::createAndSendMessage(IMonitoringDevice::MESSAGE_NUMBER messageNumber, unsigned char parameter1, unsigned char parameter2, unsigned char parameter3, unsigned char parameter4)
{
	unsigned char* initData = new unsigned char[7 + 6];
	
	initData[0] = address;
	initData[1] = 0;
	initData[2] = COMMAND_SEND_EVENT;
	initData[3] = 6;
	initData[4] = 0;
	initData[5] = messageNumber;
	initData[6] = 4;
	initData[7] = parameter1;
	initData[8] = parameter2;
	initData[9] = parameter3;
	initData[10] = parameter4;

	fifoFrame->put(&initData);
}

void MonitoringDevice::controlMessage(unsigned char* _pArea){
	const unsigned int MESSAGE_TYPE_TO_OFFSET = 7;
	const unsigned int MESSAGE_ADDRESS_TO_OFFSET = 8;
	const unsigned int MESSAGE_TYPE_FROM_OFFSET = 9;
	const unsigned int MESSAGE_ADDRESS_FROM_OFFSET = 10;
	const unsigned int MESSAGE_COMMAND_CODE_OFFSET = 11;

	const unsigned int PDU_ADDRESS_FIRST = 34;
	const unsigned int PDU_ADDRESS_LAST = 37;

	if((_pArea[MESSAGE_ADDRESS_FROM_OFFSET] >= PDU_ADDRESS_FIRST) && (_pArea[MESSAGE_ADDRESS_FROM_OFFSET] <= PDU_ADDRESS_LAST)){
		UI::getSingleton().getUsoModeControl()->change_toRemote();
	}
}
