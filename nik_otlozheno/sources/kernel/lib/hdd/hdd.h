#ifndef HDD_H
#define HDD_H

class Hdd{
public:
	static const unsigned int SECTOR_SIZE = 512;

private:
	#define IDE_DATA_PORT_R 0x01f0
	#define IDE_ERROR_R 0x01f1
	#define IDE_SECTOR_COUNT_R 0x01f2
	#define IDE_LBA_0_R 0x01f3
	#define IDE_LBA_1_R 0x01f4
	#define IDE_LBA_2_R 0x01f5
	#define IDE_LBA_3_R 0x01f6
	#define IDE_STATUS_COMMAND_R 0x01f7
	#define IDE_DEVICE_CONTROL_R 0x03f6

	#define IDE_BUSY 0x80
	#define IDE_DRDY 0x40
	#define IDE_DWF 0x20
	#define IDE_DSC 0x10
	#define IDE_DRQ 0x08
	#define IDE_CORR 0x04
	#define IDE_INDEX 0x02
	#define IDE_ERROR 0x01

	#define READ_DMA_WITH_RETRY 0xc8
	#define WRITE_DMA_WITH_RETRY 0xca

	#define IDE_BM_CMD 0x00
	#define IDE_BM_CMD_RWCTL 0x08
	#define IDE_BM_CMD_BMCTL 0x01

	static unsigned short ideBaseAddress;
	static volatile unsigned int actionCounter;
	enum RW_FLAG{
		RW_FLAG_READ,
		RW_FLAG_WRITE
	};
	static RW_FLAG rwFlag;
public:
	static unsigned int getActionCounter();
private:
	struct PRDF{
		unsigned int MRPBA;
		unsigned int EOT_SIZE;
	};
	static const unsigned int PRDF_EOT = 0x80000000;
	static PRDF prdf;

	static const unsigned int PRDF_BUFFER_SIZE = 65536;
	static unsigned char buffer[PRDF_BUFFER_SIZE];
	
	static void staticIrqHandler();

	static unsigned char sectorsCount;
	static unsigned char* pData;

public:
	Hdd();
	~Hdd();

	bool read(unsigned int _startSectorNumber, unsigned char _sectorsCount, unsigned char* _pData);
	bool write(unsigned int _startSectorNumber, unsigned char _sectorsCount, unsigned char* _pData);

	static const unsigned int MAX_SECTOR_COUNT = PRDF_BUFFER_SIZE / SECTOR_SIZE;
};

#endif