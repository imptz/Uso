#include "crc32_testing.h"

TEST_RESULT crc32_test()
{
	char data[] = {0,1,2,3,4,5,6,7,8,9};

	if (calcCRC32(data, 10) == 0x670f9a3b)
		return TEST_RESULT_PASS;
	else
		return TEST_RESULT_ERROR;
}
