/*! \file Messages.h
    \brief Обмен сообщениями   
*/

#pragma once

#include "../fifo.h"
#include "../display/display.h"

//! диапазоны подсистем параметра FROM сообщения
enum MESSAGE_FROM_OFFSET
{
	MESSAGE_FROM_OFFSET_CONTROLS = 0,
	MESSAGE_FROM_OFFSET_SYSTEM = 1000,
	MESSAGE_FROM_OFFSET_LOG = 2000,
	MESSAGE_FROM_OFFSET_CLOCK = 3000,
	MESSAGE_FROM_OFFSET_CONFIG = 4000,
	MESSAGE_FROM_OFFSET_EXTENSION_SYSTEM = 5000,
	MESSAGE_FROM_OFFSET_APPLICATION = 6000,
	MESSAGE_FROM_OFFSET_LOGIC = 7000,
	MESSAGE_FROM_OFFSET_MONITOR_MANAGER = 8000,
	MESSAGE_FROM_OFFSET_DETECTION_MANAGER = 9000,
	MESSAGE_FROM_OFFSET_SERIAL_DEBUG = 10000
};

enum MESSAGE{
	MESSAGE_USO_MODE_CONTROL_NEW_MODE = 1000,
	MESSAGE_USO_MODE_CONTROL_TO_TOOLS,
	MESSAGE_USO_MODE_CONTROL_FROM_TOOLS,
	MESSAGE_USO_MODE_CONTROL_STOP_LOGIC
};


class MessageReceiver;

//! Класс сообщения
struct Message
{
	MessageReceiver* to; /*!< указатель на получателя сообщения */
	unsigned int from; /*!< от кого */
	unsigned int msg; /*!< код сообщения */
	unsigned int par1; /*!< первый параметр сообщения */
	unsigned int par2; /*!< второй параметр сообщения */

	Message()
	{
	}

	Message(unsigned int _from, unsigned int _msg, unsigned int _par1, unsigned int _par2)
		:	from(_from), msg(_msg), par1(_par1), par2(_par2)
	{
	}
};

//! Класс получателя сообщений
/*!
	В классе создается статическая очередь сообщений. При разборе сообщений они передаются адресатам через метод onMessage,
	а очередь очищается.
*/
class MessageReceiver
{
	public:
		//! Инициализатор класса
		/*!
			Вызывается один раз при запуске приложения.
			Создает очередь сообщений.
		*/
		static void init();

		//! Обработчик сообщений.
		/*!
			Вызывается при разборе очереди сообщений у объекта, которому адресовано сообщение.
			Метод должен быть переопределен в наследнике класса.

			@param message - сообщение
		*/
		virtual void onMessage(Message message) = 0;
	protected:
		static const int MESSAGES_COUNT_MAX = 150; /*!< максимальное число сообщений в очереди. */
		static Fifo<Message>* messages; /*!< указатель на очередь сообщений */

	public:
		//! Метод производит разбор очереди сообщений.
		/*!
			Метод должен периодически вызываться.
		*/
		static void messagesProccess()
		{
			Message msg;
			if (messages->get(&msg) != 0)
				msg.to->onMessage(msg);
		}

		//! Добавить сообщение в очередь
		static void addMessage(Message message)
		{
			messages->put(message);
		}

		//! Очистить очередь сообщений
		static void clearAllMessages()
		{
			messages->clear();
		}
};

//! Класс отправителя сообщений
/*!
	MessageSender содержит список указателе на MessageReceiver-ы, которым он должен отправлять сообщения.
	Получатели регистрируются в отправителе заранее.

	Например класс кнопки (наследник MessageSender) может отсылать сообщение о нажатии на кнопку. 
	Все заинтересованные получатели регистрируют себя в кнопке и получают от нее сообщения (в том числе и о нажатии)
*/
class MessageSender
{
	private:
		static const int RECEIVER_MAX_COUNT = 150; /*!< максимальное число получателей сообщений. */
		typedef MessageReceiver* PReceiver;
		PReceiver receivers[RECEIVER_MAX_COUNT]; /*!< массив указателей на получателей сообщений */
	public:
		bool messageEnable; /*!< включение механизма передачи сообщений. true - включен, false - выключен */
		MessageSender()
			:	messageEnable(true)
		{
			for (int i = 0; i < RECEIVER_MAX_COUNT; i++)
				receivers[i] = nullptr;
		}

		virtual ~MessageSender()
		{

		}

		//! Добавить получателя сообщений
		/*!
			Метод вызывается из вне.

			@param receiver - указатель на получателя сообщений.
		*/
		void addReceiver(MessageReceiver* receiver)
		{
			for (int i = 0; i < RECEIVER_MAX_COUNT; i++)
				if (receivers[i] == nullptr)
				{
					receivers[i] = receiver;
					break;
				}
		}

		//! Удалить получателя сообщений
		/*!
			Метод вызывается из вне.

			@param receiver - указатель на получателя сообщений.
		*/
		void removeReceiver(MessageReceiver* receiver)
		{
			for (int i = 0; i < RECEIVER_MAX_COUNT; i++)
				if (receivers[i] == receiver)
				{
					receivers[i] = nullptr;
					break;
				}
		}

		//! Отправить сообщение
		/*!
			Метод вызывается наследником MessageSender. Если отправка сообщений включена, то
			всем зарегистрированным получателям отправляется сообщение.

			@param message - сообщение
		*/
		virtual void sendMessage(Message message)
		{
			if (messageEnable)
			{
				for (int i = 0; i < RECEIVER_MAX_COUNT; i++)
					if (receivers[i] != nullptr)
					{
						message.to = receivers[i];
						MessageReceiver::addMessage(message);
					}
			}
		}
};

