#pragma once

#include "../../devices/IDevice.h"
#include "../../../DEBUG/serialDebug.h"

class IMonitoringDevice : public IDevice
{
	protected:
		enum PHASE
		{
			PHASE_STOP,
			PHASE_CONFIG,
//11062013
			PHASE_CONFIG_0,
			PHASE_CONFIG_1,
//11062013_
			PHASE_CONFIG_WAIT,
			PHASE_INIT,
			PHASE_INIT_WAIT,
			PHASE_START,
		};
		PHASE phase;
	public:
		enum COMMAND
		{
			COMMAND_GET_ID = 1,
			COMMAND_INIT = 2, 
			COMMAND_GET_INITIALIZE_RESULT = 3,
			COMMAND_SET_TIME = 4,
			COMMAND_GET_EVENTS = 5,
			COMMAND_SEND_EVENT = 6
		};

		IMonitoringDevice(unsigned char _address, unsigned int _type)
			:	IDevice(_address, _type), phase(PHASE_STOP)
		{}

	protected:
		virtual void disablingDevice()
		{
			disabled = true;
			phase = PHASE_START;
		}

#pragma region monitoringSpecific
#pragma region message numbers
	public:
		enum MESSAGE_NUMBER
			{
				MESSAGE_NUMBER_SVAZ_S_PLATOY_PRIVODA_NET = 1,
				MESSAGE_NUMBER_SVAZ_S_PLATOY_PRIVODA_EST = 2,
				MESSAGE_NUMBER_PONIZHENNOE_NAPRAZHENIE_NA_KONDENSATORAH = 5,
				MESSAGE_NUMBER_NAPRAZHENIE_NA_KONDENSATORAH_V_NORME = 6,
				MESSAGE_NUMBER_NEISPRAVNOST_SILIVOGO_MOSTA = 9,
				MESSAGE_NUMBER_VOSSTANOVLENIE_SILOVOGO_MOSTA = 10,
				MESSAGE_NUMBER_NIZKOE_NAPRAZHENIE_PITANIA_CP = 11,
				MESSAGE_NUMBER_NAPRAZHENIE_PITANIA_CP_V_NORME = 12,
				MESSAGE_NUMBER_NIZKOE_NAPRAZHENIE_PITANIA_DRAIVERA = 13,
				MESSAGE_NUMBER_VISOKOE_NAPRAZHENIE_PITANIA_DRAIVERA = 15,
				MESSAGE_NUMBER_NAPRAZHENIE_PITANIA_DRAIVERA_V_NORME = 16,
				MESSAGE_NUMBER_NIZKOE_NAPRAZHENIE_PITANIA_PLATI = 17,
				MESSAGE_NUMBER_VISOKOE_NAPRAZHENIE_PITANIA_PLATI = 19,
				MESSAGE_NUMBER_NAPRAZHENIE_PITANIA_PLATI_V_NORME = 20,
				MESSAGE_NUMBER_OSHIBKA_TESTA_FLESH_PAMATI = 21,
				MESSAGE_NUMBER_TEST_FLASH_PAMATI_VIPOLNEN_USPESHNO = 22,
				MESSAGE_NUMBER_UROVNI_SIGNALOV_ENKODERA_NE_V_NORME = 23,
				MESSAGE_NUMBER_UROVNI_SIGNALOV_ENKODERA_V_NORME = 24,
				MESSAGE_NUMBER_NEISPRAVNOST_KANALA_ENKODERA = 25,
				MESSAGE_NUMBER_VOSSTANOVLENIE_KANALA_ENKODERA = 26,
				MESSAGE_NUMBER_OSHIBKA_OPREDELENIA_NAPRAVLENIA = 27,
				MESSAGE_NUMBER_VERNOE_OPREDELENIE_NAPRAVLENIA = 28,
				MESSAGE_NUMBER_PRI_DVIZHENII_NE_MENAETSA_KOORDINATA = 29,
				MESSAGE_NUMBER_PRI_DVIZHENII_MENAETSA_KOORDINATA = 30,
				MESSAGE_NUMBER_OTSUTSTVIE_TOKA_MOTORA = 31,
				MESSAGE_NUMBER_TOK_MOTORA_V_PREDELAH_NORMI = 32,
				MESSAGE_NUMBER_AVARIYNAYA_AMPLITUDE_TOKA_MOTORA = 33,
				MESSAGE_NUMBER_AMPLITUDA_TOKA_MOTORA_V_NORME = 34,
				MESSAGE_NUMBER_OBRIV_DATCHIKA_DAVLENIA = 43,
				MESSAGE_NUMBER_KZ_DATCHIKA_DAVLENIA = 45,
				MESSAGE_NUMBER_DATCHIK_DAVLENIA_V_NORME = 46,
				MESSAGE_NUMBER_PEREZAGRUZKA_CP = 47,
				MESSAGE_NUMBER_SVAZ_S_PR_VOSSTANOVLENA = 49,
				MESSAGE_NUMBER_OTSUTSTVUET_SVAZ_S_PR = 50,
				MESSAGE_NUMBER_OBRIV_MAGISTRALI_RPK = 51,
				MESSAGE_NUMBER_MAGISTRAL_RPK_VOSSTANOVLENA = 52,
				MESSAGE_NUMBER_OTSUTSTVUET_SVAZ_S_SK = 53,
				MESSAGE_NUMBER_SVAZ_S_SK_VOSSTANOVLENA = 54,
				MESSAGE_NUMBER_OBRIV_SVAZI_S_ZATVOROM = 64,
				MESSAGE_NUMBER_SVAZ_S_ZATVOROM_VOSSTANOVLENA = 65,
				MESSAGE_NUMBER_OBRIV_DATCHIKA_TEMPERATURI = 66,
				MESSAGE_NUMBER_KZ_DATCHIKA_TEMPERATURI = 68,
				MESSAGE_NUMBER_DATCHIK_TEMPERATURI_ISPRAVEN = 69,
				MESSAGE_NUMBER_OBRIV_DATCHIKA_VLAGNOSTI = 70,
				MESSAGE_NUMBER_OBRIV_DATCHIKA_VLAGNOSTI_ISPRAVEN = 71,
				MESSAGE_NUMBER_KZ_KNOPOK_VNESHNEGO_POSTA = 72,
				MESSAGE_NUMBER_KZ_KNOPOK_VNESHNEGO_POSTA_VOSST = 73,
				MESSAGE_NUMBER_OBRIV_KNOPOK_VNESHNEGO_POSTA = 74,
				MESSAGE_NUMBER_OBRIV_KNOPOK_VNESHNEGO_POSTA_VOSST = 75,
				MESSAGE_NUMBER_VOSSTANOVLENIE_SOEDINITELNIH_LINIY = 77,
				MESSAGE_NUMBER_KZ_SOEDINITELNOY_LINII = 79,
				MESSAGE_NUMBER_OBRIV_SOEDINITELNOY_LINII = 80,
				MESSAGE_NUMBER_OSHIBKA_OPREDELENIA_SOSTOYANIA_SL = 81,
				MESSAGE_NUMBER_SVAZ_VOSSTANOVLENA = 82,
				MESSAGE_NUMBER_OTSUTSTVUET_SVAZ = 83,
				MESSAGE_NUMBER_IZVECHATEL_V_NORME = 84,
				MESSAGE_NUMBER_NET_SVAZI_S_IZVECHATELEM = 85,
				MESSAGE_NUMBER_VNUTRENNAYA_OSHIBKA_IZVECHATELA = 86,
				MESSAGE_NUMBER_BLOKIROVKA_ZATVORA_ON = 90,
				MESSAGE_NUMBER_BLOKIROVKA_ZATVORA_OFF = 91,
				MESSAGE_NUMBER_ZATVOR_OSHIBKA = 92,
				MESSAGE_NUMBER_ZATVOR_OTKRIT = 93,
				MESSAGE_NUMBER_ZATVOR_ZAKRIT = 94,
				MESSAGE_NUMBER_TEMPERATURA_SHU_VISHE_NORMI = 96,
				MESSAGE_NUMBER_TEMPERATURA_SHU_NIZHE_NORMI = 98,
				MESSAGE_NUMBER_TEMPERATURA_SHU_V_NORME = 99,
				MESSAGE_NUMBER_VISOKAJA_VLAZHNOST_VNUTRI_SHU = 100,
				MESSAGE_NUMBER_VISOKAJA_VLAZHNOST_VNUTRI_SHU_V_NORME = 101,
				MESSAGE_NUMBER_SREDNIY_TOK_MOTORA_VISHE_NORMI = 112,
				MESSAGE_NUMBER_SREDNIY_TOK_MOTORA_VISHE_NORMI_V_NORME = 113,
				MESSAGE_NUMBER_NEISPRAVNOST_DATCHIKA_TEMPERATURI_NA_PLATE = 114,
				MESSAGE_NUMBER_NEISPRAVNOST_DATCHIKA_TEMPERATURI_NA_PLATE_V_NORME = 115,
				MESSAGE_NUMBER_NEISPRAVNOST_DATCHIKA_TEMPERATURI_PRIVODA = 116,
				MESSAGE_NUMBER_NEISPRAVNOST_DATCHIKA_TEMPERATURI_PRIVODA_V_NORME = 117,
				MESSAGE_NUMBER_ZATVOR_BK16_ZAKRIT = 121,
				MESSAGE_NUMBER_ZATVOR_BK16_OTKRIT = 122,
				MESSAGE_NUMBER_ZATVOR_NEISPRAVNOST = 123,
				MESSAGE_NUMBER_PR_DAVLENIE_EST = 124,
				MESSAGE_NUMBER_PR_DAVLENIE_NET = 125,
				MESSAGE_NUMBER_VOSSTANOVLENIE_DATCHIKOV_ZATVORA = 126,
				MESSAGE_NUMBER_LAMPA_NORMA = 128,
				MESSAGE_NUMBER_PEREGORELA_LAMPA = 129,
				MESSAGE_NUMBER_KZ_V_CEPI_LAMPI = 130,
				MESSAGE_NUMBER_POZICIONIROVANIE_VIPOLNENO = 131,
				MESSAGE_NUMBER_OTKAZ_POZICIONIROVANIJA = 132,
				MESSAGE_NUMBER_KORPUS_ZAKRIT = 133,
				MESSAGE_NUMBER_KORPUS_OTKRIT = 134,
				MESSAGE_NUMBER_OTKAZ_OTKRITIJA = 135,
				MESSAGE_NUMBER_TEMPERATURA_V_NORME = 136,
				MESSAGE_NUMBER_TEMPERATURA_NIZHE_NORMI = 137,
				MESSAGE_NUMBER_TEMPERATURA_VISHE_NORMI = 138,
				
				MESSAGE_NUMBER_PENABAK_SVAZ_OFF = 150,
				MESSAGE_NUMBER_PENABAK_SVAZ_ON = 151,
				MESSAGE_NUMBER_PENABAK_PERELIV = 152,
				MESSAGE_NUMBER_PENABAK_NORMA = 153,
				MESSAGE_NUMBER_PENABAK_UTECHKA = 154,
				MESSAGE_NUMBER_PENABAK_50_IZRASHODOVANO = 155,
				MESSAGE_NUMBER_PENABAK_NAPOLOVINU_ZAPOLNEN = 156,
				MESSAGE_NUMBER_PENABAK_MIN_UROVEN = 157,
				MESSAGE_NUMBER_PENABAK_UROVEN_VISHE_MIN = 158,

				MESSAGE_NUMBER_PENABAK_DATCHIK_NEISPRAVEN = 159,
				MESSAGE_NUMBER_PENABAK_DATCHIK_ISPRAVEN = 160,

				MESSAGE_NUMBER_START_SEARCH = 200,
				MESSAGE_NUMBER_START_OROSHENIA = 201,
				MESSAGE_NUMBER_STOP_POISKA_OROSHENIA = 202,
				MESSAGE_NUMBER_SIGNAL_O_VOZGORANII = 203,
				MESSAGE_NUMBER_OTMENA_SIGNALA_O_VOZGORANII = 204,
				MESSAGE_NUMBER_SCANER_MEHANIZM_GUT = 205,
				MESSAGE_NUMBER_ZATVOR_VREMA_OTKRITIA_V_NORME = 206,

				MESSAGE_NUMBER_START_PROGRAMMI_SCANIROVANIJA = 250,

				MESSAGE_NUMBER_DUMMY = 251
			};

		virtual void createAndSendMessage(MESSAGE_NUMBER messageNumber, unsigned char parameter1 = 0, unsigned char parameter2 = 0, unsigned char parameter3 = 0, unsigned char parameter4 = 0) = 0;

#pragma endregion
#pragma endregion
};
