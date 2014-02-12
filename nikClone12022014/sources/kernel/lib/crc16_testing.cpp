#include "crc16_testing.h"

TEST_RESULT crc16_test()
{
	unsigned char data[] = {0,1,2,3,4,5,6,7,8,9};

	if (Crc16::calcCRC16(data, 10) == 0x00004574)
		return TEST_RESULT_PASS;
	else
		return TEST_RESULT_ERROR;
}
