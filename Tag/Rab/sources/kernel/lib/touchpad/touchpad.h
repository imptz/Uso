/*! \file touchpad.h
    \brief Работа с сенсорной панелью    
*/

#pragma once

#include "../serialport/serialport.h"
#include "../timer/Timer.h"
#include "../Singleton.h"
#include "../message/Messages.h"
#include "../DEBUG/serialDebug.h"

//! Класс сенсорной панели.
/*!
	Синглетон класс.
	Настраивает последовательный порт для работы с сенсорной панелью, обеспечивает инициализацию панели и получение координат нажатия на панель. 
	При нажатии постоянно обновляет координаты точки нажатия, поэтому координаты актуальны так же при перемещении нажатого пера вплоть до его отпускания.
	При нажатии и отпускании отправляет подписчикам сообщения соответственно о нажатии и отпускани.
*/
class Touchpad : public ITimer, public Singleton<Touchpad>, public MessageSender, public MessageReceiver
{
private:
        static const int TIMER_PERIOD = 5; /*!< Период таймера опроса панели */
        static const SERIAL_PORT TOUCHPAD_SERIAL_PORT; /*!< Порт панели */
        static const SERIAL_PORT_SPEED TOUCHPAD_SERIAL_PORT_SPEED; /*!< Скорость обмена */

       	//! Обработчик таймера.
		virtual void timerHandler();
        unsigned int preXPos; /*!< Текущая некалиброванная координата X */
        unsigned int preYPos; /*!< Текущая некалиброванная координата Y */
        unsigned int xPos; /*!< Текущая координата X */
        unsigned int yPos; /*!< Текущая координата Y */

public:
		//! Состояние пера
        enum PEN_STATE
        {
            PEN_STATE_UP, /*!< Опущено на панель (нажато) */
            PEN_STATE_DOWN /*!< Не опущено на панель (не нажато) */
        };

private:
        PEN_STATE penState; /*!< Состояние нажатия пера */

		//! Состояния получаемые от панели
		enum TOUCH_COMMAND
		{
			TOUCH_COMMAND_PEN_UP = 0x00bf, /*!< Перо поднято */
			TOUCH_COMMAND_PEN_DOWN = 0x00ff /*!< Перо опущено */
		};

public:
		static const int TOUCHPAD_MESSAGE_FROM = MESSAGE_FROM_OFFSET_SYSTEM + 1; /*!<  */

		//! Сообщения подписчикам
		enum TOUCHPAD_MESSAGE
		{
			TOUCHPAD_MESSAGE_PEN_UP = 37, /*!< Перо поднято */
			TOUCHPAD_MESSAGE_PEN_DOWN = 38 /*!< Перо опущено */
		};
		
public:
		//! Конструктор
        Touchpad();
		//! Деструктор
        virtual ~Touchpad();

		//! Получить x координату пера
		/*!
			@return x координата опущенного пера. Если перо поднято, то возвращается последняя координата непосредственно перед моментом поднятия пера.
		*/
        unsigned int get_xPos();
		//! Получить y координату пера
		/*!
			@return y координата опущенного пера. Если перо поднято, то возвращается последняя координата непосредственно перед моментом поднятия пера.
		*/
        unsigned int get_yPos();

		//! Получить состояние пера
		/*!
			@return состояние пера: поднято, опущено
		*/
        PEN_STATE getPenState();

private:
		//! Преобразование координат из панелльных в экранные
		/*!
			Преобразует переданные параметрами координаты из панельных в экранные и сохраняет их в xPos и yPos соответственно.

			@param x - x координата пера
			@param y - y координата пера
		*/
		void correctCoords(int x, int y);

public:
		void setCalibrationData(float leftTopPointX, float leftTopPointY, float rightBottomPointX, float rightBottomPointY);
		
		virtual void onMessage(Message message);
private:
		bool calibrationMode;
		static const int SCREEN_WIDTH = 640;
		static const int SCREEN_HEIGHT = 400;
		struct CalibrationData
		{
			float leftTopPointX;
			float leftTopPointY;
			float rightBottomPointX;
			float rightBottomPointY;

			void clear()
			{
				leftTopPointX = 0.0f;
				leftTopPointY = 0.0f;
				rightBottomPointX = 0.0f;
				rightBottomPointY = 0.0f;
			}

			CalibrationData& operator=(CalibrationData& cd)
			{
				leftTopPointX = cd.leftTopPointX;
				leftTopPointY = cd.leftTopPointY;
				rightBottomPointX = cd.rightBottomPointX;
				rightBottomPointY = cd.rightBottomPointY;

				return *this;
			}
		};

		CalibrationData calibrationData;
		CalibrationData preCalibrationData;

		bool saveCalibration();
		bool loadCalibration();

		static const unsigned int START_SECTOR = 40000;
		static const unsigned int HDD_BUFFER_SIZE = ((sizeof(CalibrationData) / 512) + 1) * 512; 
		unsigned char hddBuffer[HDD_BUFFER_SIZE];
};
