#include "memory.h"
#include "../display/display.h"
#include "../debug/serialDebug.h"

#ifdef MEMORY_INFO_DISPLAY
bool serialMemoryDebug = true;
#else
bool serialMemoryDebug = false;
#endif


#pragma function(memset)
void* __cdecl memset(void *pTarget, int value, size_t size){
	char *t = reinterpret_cast<char *>(pTarget);

	while (size-- > 0){
		*t++ = static_cast<char>(value);
	}
	
	return pTarget;
}

#pragma function(memcpy)
void* __cdecl memcpy(void *pTarget, void *pSource, size_t size){
	char *t = reinterpret_cast<char *>(pTarget);
	char *s = reinterpret_cast<char *>(pSource);

	while (size-- > 0){
		*t++ = *s++;
	}
	return pTarget;
}

#ifdef MEMORY_INFO_DISPLAY

void memoryInfoDisplay(unsigned int posX, unsigned int posY, bool nd, void* ptr, bool deleteResult = true){
	static unsigned int newCount = 0;
	static unsigned int newCountNull = 0;
	static unsigned int deleteCount = 0;
	static unsigned int deleteCountNull = 0;
	static unsigned int deleteResultFalse = 0;
	
	if (nd){
		if (ptr != nullptr)
			newCount++;
		else
			newCountNull++;
	}else{
		if (!deleteResult){
			deleteResultFalse++;
		}else{
			if (ptr != nullptr)
				deleteCount++;
			else
				deleteCountNull++;
		}
	}

	Display* pDisp = Display::getSingletonPtr(false);
	if (pDisp != nullptr)
	pDisp->printUInt(MemoryAllocator::getSingleton().getFreeMemory(), posX, posY);
	pDisp->printUInt(MemoryAllocator::getSingleton().getBusyMemory(), posX + 10, posY);
	pDisp->printUInt(newCount, posX, posY + 1);
	pDisp->printUInt(deleteCount, posX, posY + 2);
	pDisp->printUInt(newCountNull, posX + 10, posY + 1);
	pDisp->printUInt(deleteCountNull, posX + 10, posY + 2);
	pDisp->printUInt(deleteResultFalse, posX + 20, posY + 2);


//	Display::getSingleton().printUInt(MemoryAllocator::getSingleton().getFree16Memory(), 0,0);
//	Display::getSingleton().printUInt(MemoryAllocator::getSingleton().getFree32Memory(), 10,0);
}

#endif

void* operator new(size_t size){
#ifdef MEMORY_INFO_DISPLAY
	void* ptr = reinterpret_cast<void*>(MemoryAllocator::getSingleton().allocate(size));
	memoryInfoDisplay(40, 0, true, ptr);

#ifdef MEMORY_SERIAL_INFO_DISPLAY
#ifdef USO_DEBUG
if (serialMemoryDebug){
SerialDebug* pSd = SerialDebug::getSingletonPtr(false);
if (pSd != nullptr)
	SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("memory_NEW: ",size);
}
#endif
#endif
	return ptr;
#else
	return reinterpret_cast<void*>(MemoryAllocator::getSingleton().allocate(size));
#endif
}

void* operator new(size_t size, void* ptr){
	return ptr;
}

void* operator new[](size_t size){
#ifdef MEMORY_INFO_DISPLAY
	void* ptr = reinterpret_cast<void*>(MemoryAllocator::getSingleton().allocate(size));
	memoryInfoDisplay(40, 0, true, ptr);
#ifdef MEMORY_SERIAL_INFO_DISPLAY
#ifdef USO_DEBUG
if (serialMemoryDebug){
SerialDebug* pSd = SerialDebug::getSingletonPtr(false);
if (pSd != nullptr)
	SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("memory_NEW[]: ",size);
}
#endif
#endif
	return ptr;
#else
	return reinterpret_cast<void*>(MemoryAllocator::getSingleton().allocate(size));
#endif	
}

void* operator new[](size_t size, void* ptr){
	return ptr;
}

void operator delete(void* ptr){
#ifdef MEMORY_INFO_DISPLAY
#ifdef MEMORY_SERIAL_INFO_DISPLAY
#ifdef USO_DEBUG
if (serialMemoryDebug){
SerialDebug* pSd = SerialDebug::getSingletonPtr(false);
if (pSd != nullptr)
	SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("memory_DELETE: ",reinterpret_cast<unsigned int>(ptr));
}
#endif
#endif
	if (ptr != nullptr){
		if (MemoryAllocator::getSingleton().deallocate(ptr))
			memoryInfoDisplay(40, 0, false, reinterpret_cast<void*>(1), true);
		else
			memoryInfoDisplay(40, 0, false, nullptr, false);
	}
	else
		memoryInfoDisplay(40, 0, false, nullptr, true);
#else
	if (ptr != nullptr)
		MemoryAllocator::getSingleton().deallocate(ptr);
#endif
}

void operator delete[](void* ptr){
#ifdef MEMORY_INFO_DISPLAY
#ifdef MEMORY_SERIAL_INFO_DISPLAY
#ifdef USO_DEBUG
if (serialMemoryDebug){
SerialDebug* pSd = SerialDebug::getSingletonPtr(false);
if (pSd != nullptr)
	SERIAL_DEBUG_ADD_DEBUG_MESSAGE_STRING1("memory_DELETE[]: ",reinterpret_cast<unsigned int>(ptr));
}
#endif
#endif
	if (ptr != nullptr){
		if (MemoryAllocator::getSingleton().deallocate(ptr))
			memoryInfoDisplay(40, 0, false, reinterpret_cast<void*>(1), true);
		else
			memoryInfoDisplay(40, 0, false, nullptr, false);
	}
	else
		memoryInfoDisplay(40, 0, false, nullptr, true);
#else
	if (ptr != nullptr)
		MemoryAllocator::getSingleton().deallocate(ptr);
#endif
}

void * malloc (size_t size){
	return reinterpret_cast<void*>(MemoryAllocator::getSingleton().allocate(size));
}

void free (void * ptr){
	if (ptr != nullptr)
		MemoryAllocator::getSingleton().deallocate(ptr);
}