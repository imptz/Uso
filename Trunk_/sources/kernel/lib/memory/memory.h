#pragma once

#include "../Singleton.h"
//#include "../processor.h"


struct MemoryMap{
	static const unsigned int CODE_START = 0x00100000;

	static const unsigned int CODE_STOP = 0x01dfffff;
	static const unsigned int DATA_START = 0x01e00000;
	static const unsigned int STACK_START = 0x0e000000;
};

extern "C" void* __cdecl memset(void *pTarget, int value, size_t size);
extern "C" void* __cdecl memcpy(void *pTarget, void *pSource, size_t size);

#pragma intrinsic(memset)

class MainMemoryList;

class MainMemoryList : public Singleton<MainMemoryList>{
	private:
		static const unsigned int ELEMENT_SIZE = 1024 * 1024;
		static const unsigned int ELEMENTS_COUNT = (MemoryMap::STACK_START - MemoryMap::DATA_START) / (1024 * 1024) - 1;

public:
		bool pointers[ELEMENTS_COUNT];
		struct BigBlock{
			unsigned char* address;
			unsigned int count;

			BigBlock()
				:	address(nullptr)
			{}
		};

		static const unsigned int BIG_BLOCKS_SIZE = 10;
		BigBlock bigBlocks[BIG_BLOCKS_SIZE];

		int getFreeBigBlock(){
			for (unsigned int i = 0; i < BIG_BLOCKS_SIZE; ++i)
				if (bigBlocks[i].address == nullptr)
					return i;

			return -1;
		}

		int findBigBlock(unsigned char* address){
			for (unsigned int i = 0; i < BIG_BLOCKS_SIZE; ++i)
				if (bigBlocks[i].address == address)
					return i;

			return -1;
		}

		int findArea(unsigned int blockCount){
			int index = -1;
			unsigned int count = 0;

			for (unsigned int i = 0; i < ELEMENTS_COUNT; ++i)
				if (!pointers[i]){
					count++;
					if (index == -1)
						index = i;

					if (count == blockCount)
						return index;
				}else{
					count = 0;
					index = -1;
				}

			return -1;
		}

	public:
		MainMemoryList(){
			memset(reinterpret_cast<unsigned char*>(MemoryMap::DATA_START), 0, ELEMENTS_COUNT * ELEMENT_SIZE);
			for (unsigned int i = 0; i < ELEMENTS_COUNT; ++i)
				pointers[i] = false;
		}

		unsigned char* allocate(unsigned int size = 0){
			if (size == 0){
				for (unsigned int i = 0; i < ELEMENTS_COUNT; ++i)
					if (!pointers[i]){
						pointers[i] = true;
						return reinterpret_cast<unsigned char*>(MemoryMap::DATA_START + i * ELEMENT_SIZE);
					}

				return nullptr;
			}else{
				if (size > 524288){
					unsigned int blockCount = size / ELEMENT_SIZE;
					if ((size % ELEMENT_SIZE) != 0)
						blockCount++;

					int index = findArea(blockCount);
					if (index != -1){
						int bIndex = getFreeBigBlock();
						if (bIndex == -1)
							return nullptr;

						for (unsigned int i = 0; i < blockCount; ++i)
							pointers[index + i] = true;

						bigBlocks[bIndex].address = reinterpret_cast<unsigned char*>(MemoryMap::DATA_START + index * ELEMENT_SIZE);
						bigBlocks[bIndex].count = blockCount;

						return reinterpret_cast<unsigned char*>(MemoryMap::DATA_START + index * ELEMENT_SIZE);
					}
				}
			}

			return nullptr;
		}

		bool deallocate(unsigned char* ptr){
			unsigned int index = (reinterpret_cast<unsigned int>(ptr) - MemoryMap::DATA_START) / ELEMENT_SIZE;

			int bIndex = findBigBlock(ptr);
			if (bIndex == -1){
				if (index < ELEMENTS_COUNT - 1){
					pointers[index] = false;
					return true;
				}
			}else{
				for (unsigned int i = 0; i < bigBlocks[bIndex].count; ++i)
					pointers[index + i] = false;

				bigBlocks[bIndex].address = nullptr;
				return true;
			}

			return false;
		}

	public:
		int getFreeMemory(){
			int c = 0;
			for (unsigned int i = 0; i < ELEMENTS_COUNT; ++i)
				if (!pointers[i])
					c++;

			return c * ELEMENT_SIZE;
		}

		int getAllocMemory(){
			int c = 0;
			for (unsigned int i = 0; i < ELEMENTS_COUNT; ++i)
				if (pointers[i])
					c++;

			return c * ELEMENT_SIZE;
		}

		unsigned int getThisAddress(){
			return reinterpret_cast<unsigned int>(this);
		}
};

template<unsigned int elementSize>
class MemoryList{
	public:
		static const unsigned int ELEMENT_SIZE = elementSize;
	private:
		static const unsigned int AREA_SIZE = 1024 * 1024;
		static const unsigned int DATA_OFFSET = 1024 * 1024 / 2;
		static const unsigned int MAX_POINTERS_COUNT = 65536;

		unsigned int count;
		bool pointers[MAX_POINTERS_COUNT];

		MemoryList* next;

	public:
		MemoryList()
			:	next(nullptr){
			count = (AREA_SIZE / 2) / elementSize; 
			memset(reinterpret_cast<unsigned char*>(reinterpret_cast<unsigned int>(this) + DATA_OFFSET), 0, AREA_SIZE / 2);
			for (unsigned int i = 0; i < count; ++i)
				pointers[i] = false;
		}

		unsigned char* allocate(){
			for (unsigned int i = 0; i < count; ++i)
				if (!pointers[i]){
					pointers[i] = true;
					return reinterpret_cast<unsigned char*>(reinterpret_cast<unsigned int>(this) + DATA_OFFSET + i * elementSize);
				}
				
			if (next != nullptr)
				return next->allocate();
			
			next = reinterpret_cast<MemoryList<elementSize>*>(MainMemoryList::getSingleton().allocate());
			if (next == nullptr)
				return nullptr;

			next->MemoryList<elementSize>::MemoryList();

			return next->allocate();			
		}

		bool deallocate(void* ptr){
			if ((ptr < this) || (reinterpret_cast<unsigned int>(ptr) > reinterpret_cast<unsigned int>(this) + AREA_SIZE)){
				if (next != nullptr)
					return next->deallocate(ptr);
				else
					return false;
			}

			unsigned int index = (reinterpret_cast<unsigned int>(ptr) - reinterpret_cast<unsigned int>(this) - DATA_OFFSET) / elementSize;
			if (pointers[index]){
				pointers[index] = false;
				return true;
			}

			return false;
		}

	public:
		int getFreeMemory(){
			int c = 0;
			for (unsigned int i = 0; i < count; ++i)
				if (!pointers[i])
					c++;
			
			c *= elementSize;

			if (next != nullptr)
				c += next->getFreeMemory();

			return c;
		}

		int getAllocMemory(){
			int c = 0;
			for (unsigned int i = 0; i < count; ++i)
				if (pointers[i])
					c++;
			
			c *= elementSize;

			if (next != nullptr)
				c += next->getAllocMemory();

			return c;
		}

		unsigned int getThisAddress(){
			return reinterpret_cast<unsigned int>(this);
		}
};

class MemoryAllocator : public Singleton<MemoryAllocator>{
public:
	MainMemoryList mainMemoryList;
	MemoryList<16>* memoryList_16;
	MemoryList<32>* memoryList_32;
	MemoryList<64>* memoryList_64;
	MemoryList<128>* memoryList_128;
	MemoryList<256>* memoryList_256;
	MemoryList<512>* memoryList_512;
	MemoryList<1024>* memoryList_1024;
	MemoryList<2048>* memoryList_2048;
	MemoryList<4096>* memoryList_4096;
	MemoryList<8192>* memoryList_8192;
	MemoryList<16384>* memoryList_16384;
	MemoryList<32768>* memoryList_32768;
	MemoryList<65536>* memoryList_65536;
	MemoryList<131072>* memoryList_131072;
	MemoryList<262144>* memoryList_262144;
	MemoryList<524288>* memoryList_524288;
public:
	MemoryAllocator(){
		memoryList_16 = reinterpret_cast<MemoryList<16>*>(MainMemoryList::getSingleton().allocate());
		if (memoryList_16 != nullptr)
			memoryList_16->MemoryList<16>::MemoryList();

		memoryList_32 = reinterpret_cast<MemoryList<32>*>(MainMemoryList::getSingleton().allocate());
		if (memoryList_32 != nullptr)
			memoryList_32->MemoryList<32>::MemoryList();

		memoryList_64 = reinterpret_cast<MemoryList<64>*>(MainMemoryList::getSingleton().allocate());
		if (memoryList_64 != nullptr)
			memoryList_64->MemoryList<64>::MemoryList();

		memoryList_128 = reinterpret_cast<MemoryList<128>*>(MainMemoryList::getSingleton().allocate());
		if (memoryList_128 != nullptr)
			memoryList_128->MemoryList<128>::MemoryList();

		memoryList_256 = reinterpret_cast<MemoryList<256>*>(MainMemoryList::getSingleton().allocate());
		if (memoryList_256 != nullptr)
			memoryList_256->MemoryList<256>::MemoryList();

		memoryList_512 = reinterpret_cast<MemoryList<512>*>(MainMemoryList::getSingleton().allocate());
		if (memoryList_512 != nullptr)
			memoryList_512->MemoryList<512>::MemoryList();

		memoryList_1024 = reinterpret_cast<MemoryList<1024>*>(MainMemoryList::getSingleton().allocate());
		if (memoryList_1024 != nullptr)
			memoryList_1024->MemoryList<1024>::MemoryList();

		memoryList_2048 = reinterpret_cast<MemoryList<2048>*>(MainMemoryList::getSingleton().allocate());
		if (memoryList_2048 != nullptr)
			memoryList_2048->MemoryList<2048>::MemoryList();

		memoryList_4096 = reinterpret_cast<MemoryList<4096>*>(MainMemoryList::getSingleton().allocate());
		if (memoryList_4096 != nullptr)
			memoryList_4096->MemoryList<4096>::MemoryList();

		memoryList_8192 = reinterpret_cast<MemoryList<8192>*>(MainMemoryList::getSingleton().allocate());
		if (memoryList_8192 != nullptr)
			memoryList_8192->MemoryList<8192>::MemoryList();

		memoryList_16384 = reinterpret_cast<MemoryList<16384>*>(MainMemoryList::getSingleton().allocate());
		if (memoryList_16384 != nullptr)
			memoryList_16384->MemoryList<16384>::MemoryList();

		memoryList_32768 = reinterpret_cast<MemoryList<32768>*>(MainMemoryList::getSingleton().allocate());
		if (memoryList_32768 != nullptr)
			memoryList_32768->MemoryList<32768>::MemoryList();

		memoryList_65536 = reinterpret_cast<MemoryList<65536>*>(MainMemoryList::getSingleton().allocate());
		if (memoryList_65536 != nullptr)
			memoryList_65536->MemoryList<65536>::MemoryList();

		memoryList_131072 = reinterpret_cast<MemoryList<131072>*>(MainMemoryList::getSingleton().allocate());
		if (memoryList_131072 != nullptr)
			memoryList_131072->MemoryList<131072>::MemoryList();

		memoryList_262144 = reinterpret_cast<MemoryList<262144>*>(MainMemoryList::getSingleton().allocate());
		if (memoryList_262144 != nullptr)
			memoryList_262144->MemoryList<262144>::MemoryList();

		memoryList_524288 = reinterpret_cast<MemoryList<524288>*>(MainMemoryList::getSingleton().allocate());
		if (memoryList_524288 != nullptr)
			memoryList_524288->MemoryList<524288>::MemoryList();
	}

	unsigned char* allocate(size_t size){
		if (size <= 16)
			return memoryList_16->allocate();

		if (size <= 32)
			return memoryList_32->allocate();
			
		if (size <= 64)
			return memoryList_64->allocate();
			
		if (size <= 128)
			return memoryList_128->allocate();
	
		if (size <= 256)
			return memoryList_256->allocate();
			
		if (size <= 512)
			return memoryList_512->allocate();
			
		if (size <= 1024)
			return memoryList_1024->allocate();
			
		if (size <= 2048)
			return memoryList_2048->allocate();
			
		if (size <= 4096)
			return memoryList_4096->allocate();
			
		if (size <= 8192)
			return memoryList_8192->allocate();
			
		if (size <= 16384)
			return memoryList_16384->allocate();
			
		if (size <= 32768)
			return memoryList_32768->allocate();
			
		if (size <= 65536)
			return memoryList_65536->allocate();
			
		if (size <= 131072)
			return memoryList_131072->allocate();
			
		if (size <= 262144)
			return memoryList_262144->allocate();
			
		if (size <= 524288)
			return memoryList_524288->allocate();
			
		return MainMemoryList::getSingleton().allocate(size);
	}

	bool deallocate(void* ptr){
		if (memoryList_16->deallocate(ptr))
			return true;

		if (memoryList_32->deallocate(ptr))
			return true;

		if (memoryList_64->deallocate(ptr))
			return true;

		if (memoryList_128->deallocate(ptr))
			return true;

		if (memoryList_256->deallocate(ptr))
			return true;

		if (memoryList_512->deallocate(ptr))
			return true;

		if (memoryList_1024->deallocate(ptr))
			return true;

		if (memoryList_2048->deallocate(ptr))
			return true;

		if (memoryList_4096->deallocate(ptr))
			return true;

		if (memoryList_8192->deallocate(ptr))
			return true;

		if (memoryList_16384->deallocate(ptr))
			return true;

		if (memoryList_32768->deallocate(ptr))
			return true;

		if (memoryList_65536->deallocate(ptr))
			return true;

		if (memoryList_131072->deallocate(ptr))
			return true;

		if (memoryList_262144->deallocate(ptr))
			return true;

		if (memoryList_524288->deallocate(ptr))
			return true;
						
		if (MainMemoryList::getSingleton().deallocate(reinterpret_cast<unsigned char*>(ptr)))
			return true;

		return false;
	}

	int getFreeMainMemory(){
		return MainMemoryList::getSingleton().getFreeMemory();
	}

	int getFree16Memory(){
		return memoryList_16->getFreeMemory();
	}

	int getFree32Memory(){
		return memoryList_32->getFreeMemory();
	}

	int getFree64Memory(){
		return memoryList_64->getFreeMemory();
	}

	int getFree128Memory(){
		return memoryList_128->getFreeMemory();
	}

	int getFree256Memory(){
		return memoryList_256->getFreeMemory();
	}

	int getFree512Memory(){
		return memoryList_512->getFreeMemory();
	}

	int getFree1024Memory(){
		return memoryList_1024->getFreeMemory();
	}

	int getFree2048Memory(){
		return memoryList_2048->getFreeMemory();
	}

	int getFree4096Memory(){
		return memoryList_4096->getFreeMemory();
	}

	int getFree8192Memory(){
		return memoryList_8192->getFreeMemory();
	}

	int getFree16384Memory(){
		return memoryList_16384->getFreeMemory();
	}

	int getFree32768Memory(){
		return memoryList_32768->getFreeMemory();
	}

	int getFree65536Memory(){
		return memoryList_65536->getFreeMemory();
	}

	int getFree131072Memory(){
		return memoryList_131072->getFreeMemory();
	}

	int getFree262144Memory(){
		return memoryList_262144->getFreeMemory();
	}

	int getFree524288Memory(){
		return memoryList_524288->getFreeMemory();
	}

	int getFreeMemory(){
		int c = 0;

		c += MainMemoryList::getSingleton().getFreeMemory();
		c += memoryList_16->getFreeMemory();
		c += memoryList_32->getFreeMemory();
		c += memoryList_64->getFreeMemory();
		c += memoryList_128->getFreeMemory();
		c += memoryList_256->getFreeMemory();
		c += memoryList_512->getFreeMemory();
		c += memoryList_1024->getFreeMemory();
		c += memoryList_2048->getFreeMemory();
		c += memoryList_4096->getFreeMemory();
		c += memoryList_8192->getFreeMemory();
		c += memoryList_16384->getFreeMemory();
		c += memoryList_32768->getFreeMemory();
		c += memoryList_65536->getFreeMemory();
		c += memoryList_131072->getFreeMemory();
		c += memoryList_262144->getFreeMemory();
		c += memoryList_524288->getFreeMemory();

		return c;
	}

	int getAllocMainMemory(){
		return MainMemoryList::getSingleton().getAllocMemory();
	}

	int getAlloc16Memory(){
		return memoryList_16->getAllocMemory();
	}

	int getAlloc32Memory(){
		return memoryList_32->getAllocMemory();
	}

	int getAlloc64Memory(){
		return memoryList_64->getAllocMemory();
	}

	int getAlloc128Memory(){
		return memoryList_128->getAllocMemory();
	}

	int getAlloc256Memory(){
		return memoryList_256->getAllocMemory();
	}

	int getAlloc512Memory(){
		return memoryList_512->getAllocMemory();
	}

	int getAlloc1024Memory(){
		return memoryList_1024->getAllocMemory();
	}

	int getAlloc2048Memory(){
		return memoryList_2048->getAllocMemory();
	}

	int getAlloc4096Memory(){
		return memoryList_4096->getAllocMemory();
	}

	int getAlloc8192Memory(){
		return memoryList_8192->getAllocMemory();
	}

	int getAlloc16384Memory(){
		return memoryList_16384->getAllocMemory();
	}

	int getAlloc32768Memory(){
		return memoryList_32768->getAllocMemory();
	}

	int getAlloc65536Memory(){
		return memoryList_65536->getAllocMemory();
	}

	int getAlloc131072Memory(){
		return memoryList_131072->getAllocMemory();
	}

	int getAlloc262144Memory(){
		return memoryList_262144->getAllocMemory();
	}

	int getAlloc524288Memory(){
		return memoryList_524288->getAllocMemory();
	}

	int getAllocMemory(){
		int c = 0;

		c += memoryList_16->getAllocMemory();
		c += memoryList_32->getAllocMemory();
		c += memoryList_64->getAllocMemory();
		c += memoryList_128->getAllocMemory();
		c += memoryList_256->getAllocMemory();
		c += memoryList_512->getAllocMemory();
		c += memoryList_1024->getAllocMemory();
		c += memoryList_2048->getAllocMemory();
		c += memoryList_4096->getAllocMemory();
		c += memoryList_8192->getAllocMemory();
		c += memoryList_16384->getAllocMemory();
		c += memoryList_32768->getAllocMemory();
		c += memoryList_65536->getAllocMemory();
		c += memoryList_131072->getAllocMemory();
		c += memoryList_262144->getAllocMemory();
		c += memoryList_524288->getAllocMemory();

		return c;
	}

	int getThisAddressMainMemory(){
		return MainMemoryList::getSingleton().getThisAddress();
	}

	int getThisAddress16Memory(){
		return memoryList_16->getThisAddress();
	}

	int getThisAddress32Memory(){
		return memoryList_32->getThisAddress();
	}

	int getThisAddress64Memory(){
		return memoryList_64->getThisAddress();
	}

	int getThisAddress128Memory(){
		return memoryList_128->getThisAddress();
	}

	int getThisAddress256Memory(){
		return memoryList_256->getThisAddress();
	}

	int getThisAddress512Memory(){
		return memoryList_512->getThisAddress();
	}

	int getThisAddress1024Memory(){
		return memoryList_1024->getThisAddress();
	}

	int getThisAddress2048Memory(){
		return memoryList_2048->getThisAddress();
	}

	int getThisAddress4096Memory(){
		return memoryList_4096->getThisAddress();
	}

	int getThisAddress8192Memory(){
		return memoryList_8192->getThisAddress();
	}

	int getThisAddress16384Memory(){
		return memoryList_16384->getThisAddress();
	}

	int getThisAddress32768Memory(){
		return memoryList_32768->getThisAddress();
	}

	int getThisAddress65536Memory(){
		return memoryList_65536->getThisAddress();
	}

	int getThisAddress131072Memory(){
		return memoryList_131072->getThisAddress();
	}

	int getThisAddress262144Memory(){
		return memoryList_262144->getThisAddress();
	}

	int getThisAddress524288Memory(){
		return memoryList_524288->getThisAddress();
	}
};

void* operator new(size_t size);

void* operator new(size_t size, void* ptr);

void* operator new[](size_t size);

void* operator new[](size_t size, void* ptr);

void operator delete(void* ptr);

void operator delete[](void* ptr);

#define SAFE_DELETE(p)		{ if(p != nullptr) { delete (p);     (p) = nullptr;} }

#define SAFE_DELETE_ARRAY(p){ if(p != nullptr) { delete[] (p);   (p) = nullptr;} }

void * malloc (size_t size);
void free (void * ptr);
