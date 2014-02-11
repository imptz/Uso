#pragma once

#include "../subsystems/monitoring/monitoringSubsystem.h"
#include "../subsystems/monitoring/iMonitoringDevice.h"

class MonitoringDevice : public IMonitoringDevice, public MessageReceiver, public MessageSender
{
	public:
		static const unsigned int ID = 1;

#pragma region static methods
	public:
		static bool registered;
		static void registerDevice()
		{
			if (!registered)
				MonitoringSubsystem::getSingleton().regDevice(ID, createDevice);

			registered = true;
		}

		static IMonitoringDevice* createDevice(unsigned char address, unsigned int _type)
		{
			return new MonitoringDevice(address, _type);
		}
#pragma endregion

		MonitoringDevice(unsigned char _address, unsigned int _type);
		~MonitoringDevice();

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
		void createSetTimeFrame();
		void createGetEventsFrame();

		static const unsigned int SET_TIME_TIME_OUT = 20 * 60;
		unsigned int setTimeTimeOut;
		bool isSetTimeTimeOut;

		unsigned short eventId;

		void commandGetEvent(unsigned char* _pArea);
		void setFault(unsigned char value);
		void redirectToPc(unsigned char* _pArea);
		void toLog(unsigned char* pMsg);
		void setOutputs(unsigned char* pMsg);
		unsigned int firstMessage(unsigned char** _ppArea);
		void nextMessage(unsigned char** _ppArea);
		bool isEventMessage(unsigned char* _pArea);
		unsigned int getMessageType(unsigned char* _pArea);
		bool isLogingMessage(unsigned char* pMsg);
		void getMessageInfo(unsigned char* pMsg, char** text, unsigned char* parameter1, unsigned char*parameter2);
		bool isCommandMessage(unsigned char* _pArea);

	private:
#pragma region message texts
		static char* MESSAGE_TEXT_SVAZ_S_PLATOY_PRIVODA_NET;
		static char* MESSAGE_TEXT_SVAZ_S_PLATOY_PRIVODA_EST;
		static char* MESSAGE_TEXT_PONIZHENNOE_NAPRAZHENIE_NA_KONDENSATORAH;
		static char* MESSAGE_TEXT_NAPRAZHENIE_NA_KONDENSATORAH_V_NORME;
		static char* MESSAGE_TEXT_NEISPRAVNOST_SILIVOGO_MOSTA;
		static char* MESSAGE_TEXT_VOSSTANOVLENIE_SILOVOGO_MOSTA;
		static char* MESSAGE_TEXT_NIZKOE_NAPRAZHENIE_PITANIA_CP;
		static char* MESSAGE_TEXT_NAPRAZHENIE_PITANIA_CP_V_NORME;
		static char* MESSAGE_TEXT_NIZKOE_NAPRAZHENIE_PITANIA_DRAIVERA;
		static char* MESSAGE_TEXT_NIZKOE_NAPRAZHENIE_PITANIA_DRAIVERA_V_NORME;
		static char* MESSAGE_TEXT_VISOKOE_NAPRAZHENIE_PITANIA_DRAIVERA;
		static char* MESSAGE_TEXT_VISOKOE_NAPRAZHENIE_PITANIA_DRAIVERA_V_NORME;
		static char* MESSAGE_TEXT_NIZKOE_NAPRAZHENIE_PITANIA_PLATI;
		static char* MESSAGE_TEXT_NIZKOE_NAPRAZHENIE_PITANIA_PLATI_V_NORME;
		static char* MESSAGE_TEXT_VISOKOE_NAPRAZHENIE_PITANIA_PLATI;
		static char* MESSAGE_TEXT_VISOKOE_NAPRAZHENIE_PITANIA_PLATI_V_NORME;
		static char* MESSAGE_TEXT_OSHIBKA_TESTA_FLESH_PAMATI;
		static char* MESSAGE_TEXT_TEST_FLASH_PAMATI_VIPOLNEN_USPESHNO;
		static char* MESSAGE_TEXT_UROVNI_SIGNALOV_ENKODERA_NE_V_NORME;
		static char* MESSAGE_TEXT_UROVNI_SIGNALOV_ENKODERA_V_NORME;
		static char* MESSAGE_TEXT_NEISPRAVNOST_KANALA_ENKODERA;
		static char* MESSAGE_TEXT_VOSSTANOVLENIE_KANALA_ENKODERA;
		static char* MESSAGE_TEXT_OSHIBKA_OPREDELENIA_NAPRAVLENIA;
		static char* MESSAGE_TEXT_VERNOE_OPREDELENIE_NAPRAVLENIA;
		static char* MESSAGE_TEXT_PRI_DVIZHENII_NE_MENAETSA_KOORDINATA;
		static char* MESSAGE_TEXT_PRI_DVIZHENII_MENAETSA_KOORDINATA;
		static char* MESSAGE_TEXT_OTSUTSTVIE_TOKA_MOTORA;
		static char* MESSAGE_TEXT_TOK_MOTORA_V_PREDELAH_NORMI;
		static char* MESSAGE_TEXT_AVARIYNAYA_AMPLITUDE_TOKA_MOTORA;
		static char* MESSAGE_TEXT_AMPLITUDA_TOKA_MOTORA_V_NORME;
		static char* MESSAGE_TEXT_NAPRAZHENIE_PITANIA_NIZHE_NORMI;
		static char* MESSAGE_TEXT_NAPRAZHENIE_PITANIA_NIZHE_NORMI_V_NORME;
		static char* MESSAGE_TEXT_NAPRAZHENIE_PITANIA_VISHE_NORMI;
		static char* MESSAGE_TEXT_NAPRAZHENIE_PITANIA_VISHE_NORMI_V_NORME;
		static char* MESSAGE_TEXT_PR_OSHIBKA_TESTA_FLESH_PAMATI;
		static char* MESSAGE_TEXT_PR_TEST_FLASH_PAMATI_VIPOLNEN_USPESHNO;
		static char* MESSAGE_TEXT_PR_NIZROE_NAPRAZHENIE_PITENIA_PROCESSORA;
		static char* MESSAGE_TEXT_PR_NIZROE_NAPRAZHENIE_PITENIA_PROCESSORA_V_NORME;
		static char* MESSAGE_TEXT_OBRIV_DATCHIKA_DAVLENIA;
		static char* MESSAGE_TEXT_OBRIV_DATCHIKA_DAVLENIA_V_NORME;
		static char* MESSAGE_TEXT_KZ_DATCHIKA_DAVLENIA;
		static char* MESSAGE_TEXT_KZ_DATCHIKA_DAVLENIA_V_NORME;
		static char* MESSAGE_TEXT_PEREZAGRUZKA_CP;
		static char* MESSAGE_TEXT_SVAZ_S_PR_VOSSTANOVLENA;
		static char* MESSAGE_TEXT_OTSUTSTVUET_SVAZ_S_PR;
		static char* MESSAGE_TEXT_OBRIV_MAGISTRALI_RPK;
		static char* MESSAGE_TEXT_MAGISTRAL_RPK_VOSSTANOVLENA;
		static char* MESSAGE_TEXT_OTSUTSTVUET_SVAZ_S_SK;
		static char* MESSAGE_TEXT_SVAZ_S_SK_VOSSTANOVLENA;
		static char* MESSAGE_TEXT_OBRIV_SVAZI_S_ZATVOROM;
		static char* MESSAGE_TEXT_SVAZ_S_ZATVOROM_VOSSTANOVLENA;
		static char* MESSAGE_TEXT_OBRIV_DATCHIKA_TEMPERATURI;
		static char* MESSAGE_TEXT_OBRIV_DATCHIKA_TEMPERATURI_ISPRAVEN;
		static char* MESSAGE_TEXT_KZ_DATCHIKA_TEMPERATURI;
		static char* MESSAGE_TEXT_KZ_DATCHIKA_TEMPERATURI_ISPRAVEN;
		static char* MESSAGE_TEXT_OBRIV_DATCHIKA_VLAGNOSTI;
		static char* MESSAGE_TEXT_OBRIV_DATCHIKA_VLAGNOSTI_ISPRAVEN;
		static char* MESSAGE_TEXT_KZ_KNOPOK_VNESHNEGO_POSTA;
		static char* MESSAGE_TEXT_KZ_KNOPOK_VNESHNEGO_POSTA_VOSST;
		static char* MESSAGE_TEXT_OBRIV_KNOPOK_VNESHNEGO_POSTA;
		static char* MESSAGE_TEXT_OBRIV_KNOPOK_VNESHNEGO_POSTA_VOSST;
		static char* MESSAGE_TEXT_VOSSTANOVLENIE_SOEDINITELNIH_LINIY;
		static char* MESSAGE_TEXT_KZ_SOEDINITELNOY_LINII;
		static char* MESSAGE_TEXT_OBRIV_SOEDINITELNOY_LINII;
		static char* MESSAGE_TEXT_OSHIBKA_OPREDELENIA_SOSTOYANIA_SL;
		static char* MESSAGE_TEXT_SVAZ_VOSSTANOVLENA;
		static char* MESSAGE_TEXT_OTSUTSTVUET_SVAZ;
		static char* MESSAGE_TEXT_IZVECHATEL_V_NORME;
		static char* MESSAGE_TEXT_NET_SVAZI_S_IZVECHATELEM;
		static char* MESSAGE_TEXT_VNUTRENNAYA_OSHIBKA_IZVECHATELA;
		static char* MESSAGE_TEXT_ZATVOR_OSHIBKA;
		static char* MESSAGE_TEXT_ZATVOR_OTKRIT;
		static char* MESSAGE_TEXT_ZATVOR_ZAKRIT;
		static char* MESSAGE_TEXT_TEMPERATURA_SHU_VISHE_NORMI;
		static char* MESSAGE_TEXT_TEMPERATURA_SHU_VISHE_NORMI_V_NORME;
		static char* MESSAGE_TEXT_TEMPERATURA_SHU_NIZHE_NORMI;
		static char* MESSAGE_TEXT_TEMPERATURA_SHU_NIZHE_NORMI_V_NORME;
		static char* MESSAGE_TEXT_VISOKAJA_VLAZHNOST_VNUTRI_SHU;
		static char* MESSAGE_TEXT_VISOKAJA_VLAZHNOST_VNUTRI_SHU_V_NORME;
		static char* MESSAGE_TEXT_SREDNIY_TOK_MOTORA_VISHE_NORMI;
		static char* MESSAGE_TEXT_SREDNIY_TOK_MOTORA_VISHE_NORMI_V_NORME;
		static char* MESSAGE_TEXT_NEISPRAVNOST_DATCHIKA_TEMPERATURI_NA_PLATE;
		static char* MESSAGE_TEXT_NEISPRAVNOST_DATCHIKA_TEMPERATURI_NA_PLATE_V_NORME;
		static char* MESSAGE_TEXT_NEISPRAVNOST_DATCHIKA_TEMPERATURI_PRIVODA;
		static char* MESSAGE_TEXT_NEISPRAVNOST_DATCHIKA_TEMPERATURI_PRIVODA_V_NORME;
		static char* MESSAGE_TEXT_PEREZAPUSK_CP;
		static char* MESSAGE_TEXT_ZATVOR_NEISPRAVNOST;
		static char* MESSAGE_TEXT_PR_DAVLENIE_EST;
		static char* MESSAGE_TEXT_PR_DAVLENIE_NET;
		static char* MESSAGE_TEXT_VOSSTANOVLENIE_DATCHIKOV_ZATVORA;
		static char* MESSAGE_TEXT_ZATVOR_V_NORME;
		static char* MESSAGE_TEXT_START_SEARCH;
		static char* MESSAGE_TEXT_START_OROSHENIA;
		static char* MESSAGE_TEXT_STOP_POISKA_OROSHENIA;
		static char* MESSAGE_TEXT_SIGNAL_O_VOZGORANII;
		static char* MESSAGE_TEXT_OTMENA_SIGNALA_O_VOZGORANII;
		static char* MESSAGE_TEXT_PEREGORELA_LAMPA;
		static char* MESSAGE_TEXT_KZ_V_CEPI_LAMPI;
		static char* MESSAGE_TEXT_OTKAZ_POZICIONIROVANIJA;
		static char* MESSAGE_TEXT_OTKAZ_OTKRITIJA;
		static char* MESSAGE_TEXT_TEMPERATURA_V_NORME;
		static char* MESSAGE_TEXT_TEMPERATURA_NIZHE_NORMI;
		static char* MESSAGE_TEXT_TEMPERATURA_VISHE_NORMI;
		static char* MESSAGE_TEXT_PENABAK_SVAZ_OFF;
		static char* MESSAGE_TEXT_PENABAK_SVAZ_ON;
		static char* MESSAGE_TEXT_PENABAK_PERELIV;
		static char* MESSAGE_TEXT_PENABAK_NORMA;
		static char* MESSAGE_TEXT_PENABAK_UTECHKA;
		static char* MESSAGE_TEXT_PENABAK_50_IZRASHODOVANO;
		static char* MESSAGE_TEXT_PENABAK_NAPOLOVINU_ZAPOLNEN;
		static char* MESSAGE_TEXT_PENABAK_MIN_UROVEN;
		static char* MESSAGE_TEXT_PENABAK_DATCHIK_NEISPRAVEN;
		static char* MESSAGE_TEXT_PENABAK_DATCHIK_ISPRAVEN;
		static char* MESSAGE_TEXT_SCANER_MEHANIZM_GUT;
		static char* MESSAGE_TEXT_ZATVOR_VREMA_OTKRITIA_V_NORME;
		static char* MESSAGE_TEXT_BLOKIROVKA_ZATVORA_ON;
		static char* MESSAGE_TEXT_BLOKIROVKA_ZATVORA_OFF;

#pragma endregion

	public:
		virtual void createAndSendMessage(MESSAGE_NUMBER messageNumber, unsigned char parameter1 = 0, unsigned char parameter2 = 0, unsigned char parameter3 = 0, unsigned char parameter4 = 0);
};
