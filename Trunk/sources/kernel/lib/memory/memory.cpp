#include "memory.h"
#include "../display/display.h"

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

void* operator new(size_t size){
	return reinterpret_cast<void*>(MemoryAllocator::getSingleton().allocate(size));
}

void* operator new(size_t size, void* ptr){
	return ptr;
}

void* operator new[](size_t size){
	return reinterpret_cast<void*>(MemoryAllocator::getSingleton().allocate(size));
}

void* operator new[](size_t size, void* ptr){
	return ptr;
}

void operator delete(void* ptr){
	if (ptr != nullptr)
		MemoryAllocator::getSingleton().deallocate(ptr);
}

void operator delete[](void* ptr){
	if (ptr != nullptr)
		MemoryAllocator::getSingleton().deallocate(ptr);
}

void * malloc (size_t size){
	return reinterpret_cast<void*>(MemoryAllocator::getSingleton().allocate(size));
}

void free (void * ptr){
	if (ptr != nullptr)
		MemoryAllocator::getSingleton().deallocate(ptr);
}
