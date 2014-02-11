#pragma once

#include "logic\LogicCooling.h"
#include "logic\LogicSearching.h"

//! ����� ����������.
/*!
	������� ��������� ������������. �������� ����������� ���� ��������� ���������.
*/
class Application : public ITimer, public Singleton<Application>, public MessageReceiver, public MessageSender
{
private:
        static const unsigned int TIMER_PERIOD = 10000; /*!< ������ ������ ������ ����������� ������� */
        virtual void timerHandler(); /*!< ���������� ������� */
		Logic* logic; /*!< ��������� �� ������ ������� ������ ���*/
		
		//! ������� ������ ������ ���.
		/*!
			������ �� ������������ ������� ��������� ������ ������ ���.
		*/
		void createLogic(); 

public:
		//! �����������
        Application();

		//! ����������
		~Application();

		//! �������� ����� ����������
		/*!
			������� ���������� ���������� ���������� ���. �������� ����� �������� ������ ���. �������� ����������� ���� ��������� ���������.
		*/
        void start();

		//! ���������� ��������� Application
		virtual void onMessage(Message message);
};

