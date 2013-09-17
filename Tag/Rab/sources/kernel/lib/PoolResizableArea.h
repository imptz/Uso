//#ifndef POOL_RESIZABLE_AREA_H 
//#define POOL_RESIZABLE_AREA_H
//
//#include "memory\memory.h"
//
//template<typename T>
//struct ResizableArea
//{
//	bool busy;
//	unsigned int size;
//	T* pointer;
//
//	ResizableArea(unsigned int _size)
//		:	busy(false), size(_size), pointer(new unsigned char[_size])
//	{
//	}
//
//	~ResizableArea()
//	{
//		delete[] pointer;
//	}
//
//	void release()
//	{
//		busy = false;
//	}
//
//	void lock()
//	{
//		busy = true;
//	}
//};
//
//template<typename T>
//class PoolResizableArea
//{
//	private:
//		static const unsigned int SIZE = 100;
//		ResizableArea<T>** list;
//
//	public:
//		PoolResizableArea()
//			:	list(new ResizableArea<T>*[SIZE])
//		{
//			for (unsigned int i = 0; i < SIZE; i++)
//				list[i] = nullptr;
//		}
//
//		~PoolResizableArea()
//		{
//			for (unsigned int i = 0; i < SIZE; i++)
//				if (list[i] != nullptr)
//					delete list[i];
//
//			delete[] list;
//		}
//
//		ResizableArea<T>* get(unsigned int size)
//		{
//			int index = -1;
//			unsigned int i;
//			
//			for (i = 0; i < SIZE; i++)
//			{
//				if (list[i] == nullptr)
//					break;
//
//				if (!list[i]->busy)
//					if (list[i]->size >= size)
//					{
//						if (index == -1)
//							index = i;
//						else
//							if (list[index]->size >= list[i]->size)
//								index = i;
//					}	
//			}
//
//			if (index != -1)
//			{
//				list[index]->lock();
//				return list[index];
//			}
//
//			if (i != SIZE)
//			{
//				list[i] = new ResizableArea<T>(size);
//				list[i]->lock();
//				return list[i];
//			}
//
//			return nullptr;
//		}
//
//		unsigned int getLockCount()
//		{
//			unsigned int res = 0;
//			for (unsigned int i = 0; i < SIZE; i++)
//			{
//				if ((list[i] != nullptr) && (list[i]->busy))
//					res++;
//			}
//
//			return res;
//		}
//};
//
//
//#endif