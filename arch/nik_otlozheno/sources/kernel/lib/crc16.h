#pragma once

class Crc16{
public:
	static unsigned short calcCRC16(unsigned char *data, int length);
};

