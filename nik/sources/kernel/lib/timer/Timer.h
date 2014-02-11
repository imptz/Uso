/*! \file Timer.h
    \brief Таймер    
*/

#pragma once

#include "../Singleton.h"

class ITimer;
class TimerManager;

//! Класс таймера.
class Timer
{
	friend class TimerManager;
	private:
		unsigned int index; /*!< Индекс таймера в массиве */
		long period; /*!< Период таймера в милисек */
		long counter; /*!< Текущий счетчик таймера */
		bool enable; /*!< Признак влючения таймера: true - включен, false - выключен */
		ITimer *client; /*!< Указатель на объект содержащий метод обработчик таймера */

		Timer();
		Timer(unsigned int _index, long _period, ITimer *_client);
		Timer(const Timer& copy);
		Timer& operator =(const Timer &a);

	public:
		~Timer();
		//! Включение таймера
		void start();
		//! Выключение таймера
		void stop();

		//! Проверка включен ли таймер
		/*!
			@return true - включен, false - выключен
		*/
		bool isStart();
		//! Получить индекс таймера в массиве
		/*!
			@return Индекс таймера в массиве
		*/
		unsigned int getIndex();
};

//! Класс менеджера таймеров.
/*!
	Синглетон класс.
	Настраивает системные часы, содержит, создает и удаляет таймеры, изменяет значения счетчика таймеров.
*/
class TimerManager : public Singleton<TimerManager>
{
	private:
		static const unsigned short PIT_CONTROL_WORD; /*!<  */
		static const unsigned short PIT_COUNTER_0; /*!<  */
		static const unsigned short PIT_DIVIDER; /*!<  */

		static const unsigned int MAX_TIMERS_COUNT = 300; /*!< Максимальное число таймеров в системе */
		static Timer* timers[MAX_TIMERS_COUNT]; /*!< Массив таймеров */

		//! Статический обработчик прерывания системного таймера
		/*!
			Обрабатывает прерывание системного таймера.
			Увеличивает счетчик у всех включенных таймеров. При достижении каким-нибудь таймером своего периода его счетчик обнуляется и вызывается метод-обработчик у клиента.
		*/
		static void staticIrqHandler();

	public:
		TimerManager();
		//! Создает таймер
		/*!
			Если в массиве таймеров есть место, то создает новый таймер и помещает указатель на него в массив

			@param period  - период таймера в милисек
			@param _client - указатель на объект содержащий обработчик таймера
			@return Указатель на созданный таймер или nullptr, если тацмер создать не удалось.
		*/
		Timer* createTimer(long period, ITimer *_client);
		//! Удаляет таймер
		/*!
			Удаляет таймер и удаляет указатель на него из массива. 

			@param index  - индекс таймера. Индекс не проверяется на выход за границы массива.
		*/
		void deleteTimer(unsigned int index);

	private:
		int timersCount; /*!< Число таймеров в массиве. */
	public:
		//! Получить число таймеров в массиве
		/*!
			@return число таймеров в массиве
		*/
		int getTimersCount();
};

//! Интерфейс клиента таймера
/*!
	От этого класса наследуются классы, которые должны выполнять периодические действия.
*/
class ITimer
{
	public:
		Timer *pTimer; /*!< Указатель на таймер. */
		//! Конструктор
		/*!
			Получает таймер от TimerManager и сохраняет его в pTimer

			@param period - период таймера в милисек
		*/
		ITimer(long period);
		virtual ~ITimer();
		//! Чисто виртуальный метод обработки события таймера о завершении периода.
		/*!
			Метод вызывает TimerManager когда счетчик таймера достиг установленного периода.
		*/
		virtual void timerHandler() = 0;
};
