#include "clock.h"

Clock::Clock()
	:	ITimer(TIMER_PERIOD)
{
	pTimer->start();
}

Clock::~Clock(){}

unsigned char Clock::getData(unsigned char address){
	unsigned char val;
	_asm{
		mov     al,address
		out     CLOCK_PORT,al
		in      al,0x71
		mov     val,al
	}	
	
	return val;
}

void Clock::setData(unsigned char address, unsigned char data){
	_asm{
		mov     al,address
		out     CLOCK_PORT,al
		mov		al, data
		out     0x71, al
	}	
}

Clock::DateTime Clock::getClock(){
	return DateTime(getData(CLOCK_SECOND_PORT),
		getData(CLOCK_MINUTE_PORT),
		getData(CLOCK_HOUR_PORT),	
		getData(CLOCK_DAY_PORT),
		getData(CLOCK_MONTH_PORT),
		getData(CLOCK_YEAR_PORT));
}

Clock::DateTime Clock::getClock2(){
	DateTime _dt;
	_dt.year = getData(CLOCK_YEAR_PORT);
	_dt.month = getData(CLOCK_MONTH_PORT);
	_dt.day = getData(CLOCK_DAY_PORT);	
	_dt.hour = getData(CLOCK_HOUR_PORT);
	_dt.minute = getData(CLOCK_MINUTE_PORT);
	_dt.second = getData(CLOCK_SECOND_PORT);

	DateTime resDt;
	resDt.year = ((_dt.year & 0xf0) >> 4) * 10 + (_dt.year & 0x0f);
	resDt.month = ((_dt.month & 0xf0) >> 4) * 10 + (_dt.month & 0x0f);
	resDt.day = ((_dt.day & 0xf0) >> 4) * 10 + (_dt.day & 0x0f);
	resDt.hour = ((_dt.hour & 0xf0) >> 4) * 10 + (_dt.hour & 0x0f);
	resDt.minute = ((_dt.minute & 0xf0) >> 4) * 10 + (_dt.minute & 0x0f);
	resDt.second = ((_dt.second & 0xf0) >> 4) * 10 + (_dt.second & 0x0f);
	return resDt;
}

void Clock::setClock(DateTime value){
	setData(CLOCK_SECOND_PORT, value.second);
	setData(CLOCK_MINUTE_PORT, value.minute);
	setData(CLOCK_HOUR_PORT, value.hour);
	setData(CLOCK_DAY_PORT, value.day);
	setData(CLOCK_MONTH_PORT, value.month);
	setData(CLOCK_YEAR_PORT, value.year);
}

void Clock::setSecond(unsigned char value){
	setData(CLOCK_SECOND_PORT, value);
}

void Clock::setMinute(unsigned char value){
	setData(CLOCK_MINUTE_PORT, value);
}

void Clock::setHour(unsigned char value){
	setData(CLOCK_HOUR_PORT, value);
}

void Clock::setDay(unsigned char value){
	setData(CLOCK_DAY_PORT, value);
}

void Clock::setMonth(unsigned char value){
	setData(CLOCK_MONTH_PORT, value);
}

void Clock::setYear(unsigned char value){
	setData(CLOCK_YEAR_PORT, value);
}

void Clock::timerHandler(){
	DateTime dt = getClock();
	unsigned int p1 = dt.hour;
	p1 = p1 << 8;
	p1 |= dt.minute;
	p1 = p1 << 8;
	p1 |= dt.second;
	unsigned int p2 = dt.year;
	p2 = p2 << 8;
	p2 |= dt.month;
	p2 = p2 << 8;
	p2 |= dt.day;
	sendMessage(Message(MESSAGE_FROM_OFFSET_CLOCK, MESSAGE_CLOCK_TICK, p1, p2));
}
