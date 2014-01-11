#pragma once

#include "timer\Timer.h"
#include "message\Messages.h"

class Application : public ITimer, public Singleton<Application>, public MessageReceiver, public MessageSender
{
private:
        static const unsigned int TIMER_PERIOD = 10000; /*!< ������ ������ ������ ����������� ������� */
        virtual void timerHandler(); /*!< ���������� ������� */
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

