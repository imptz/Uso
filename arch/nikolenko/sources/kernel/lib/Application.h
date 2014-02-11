#pragma once

#include "logic\LogicCooling.h"
#include "logic\LogicSearching.h"

//! Класс приложения.
/*!
	Создает интерфейс пользователя. Содержит бесконечный цикл обработки сообщений.
*/
class Application : public ITimer, public Singleton<Application>, public MessageReceiver, public MessageSender
{
private:
        static const unsigned int TIMER_PERIOD = 10000; /*!< Период вызова метода обработчика таймера */
        virtual void timerHandler(); /*!< Обработчик таймера */
		Logic* logic; /*!< Указатель на объект текущей логики УСО*/
		
		//! Создает объект логики УСО.
		/*!
			Исходя из конфигурации создает требуемую логику работы УСО.
		*/
		void createLogic(); 

public:
		//! Конструктор
        Application();

		//! Деструктор
		~Application();

		//! Основной метод приложения
		/*!
			Ожидает готовности подсистемы расширения УСО. Вызывает метод создания логики УСО. Содержит бесконечный цикл обработки сообщений.
		*/
        void start();

		//! Обработчик сообщений Application
		virtual void onMessage(Message message);
};

