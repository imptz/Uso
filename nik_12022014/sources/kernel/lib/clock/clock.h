#ifndef CLOCK_H
#define CLOCK_H

#include "../Singleton.h"
#include "../timer/Timer.h"
#include "../message/Messages.h"
#include "../display/display.h"

class Clock : public Singleton<Clock>, public ITimer, public MessageSender
{
private:
	static const unsigned int TIMER_PERIOD = 1000;
	virtual void timerHandler();

	#define CLOCK_PORT 0x0070
	#define CLOCK_SECOND_PORT 0x00
	#define CLOCK_MINUTE_PORT 0x02
	#define CLOCK_HOUR_PORT 0x04
	#define CLOCK_DAY_PORT 0x07
	#define CLOCK_MONTH_PORT 0x08
	#define CLOCK_YEAR_PORT 0x09

	void setData(unsigned char address, unsigned char data);
	unsigned char getData(unsigned char address);

public:
	Clock();
	~Clock();

	struct DateTime
	{
		unsigned char second;
		unsigned char minute;
		unsigned char hour;
		unsigned char day;
		unsigned char month;
		unsigned char year;

		DateTime() 
			:	second(0),
				minute(0),
				hour(0),
				day(0),
				month(0),
				year(0)
		{
		}

		DateTime(unsigned char _second,
			unsigned char _minute,
			unsigned char _hour,
			unsigned char _day,
			unsigned char _month,
			unsigned char _year)
			:	second(_second),
				minute(_minute),
				hour(_hour),
				day(_day),
				month(_month),
				year(_year)
		{
		}

		char* toString(char* cStr)
		{
			cStr[0] = Display::getSingleton().toChar((day & 0xf0) >> 4);
			cStr[1] = Display::getSingleton().toChar(day & 0x0f);
			cStr[2] = '.';
			cStr[3] = Display::getSingleton().toChar((month & 0xf0) >> 4);
			cStr[4] = Display::getSingleton().toChar(month & 0x0f);
			cStr[5] = '.';
			cStr[6] = Display::getSingleton().toChar((year & 0xf0) >> 4);
			cStr[7] = Display::getSingleton().toChar(year & 0x0f);
			cStr[8] = ' ';
			cStr[9] = Display::getSingleton().toChar((hour & 0xf0) >> 4);
			cStr[10] = Display::getSingleton().toChar(hour & 0x0f);
			cStr[11] = ':';
			cStr[12] = Display::getSingleton().toChar((minute & 0xf0) >> 4);
			cStr[13] = Display::getSingleton().toChar(minute & 0x0f);
			cStr[14] = 0;

			return cStr;
		}
	};

	DateTime getClock();
	DateTime getClock2();

	void setClock(DateTime value);
	void setSecond(unsigned char value);
	void setMinute(unsigned char value);
	void setHour(unsigned char value);
	void setDay(unsigned char value);
	void setMonth(unsigned char value);
	void setYear(unsigned char value);

	enum CLOCK_MESSAGE
	{
		CLOCK_MESSAGE_TICK = 3
	};
};

#endif