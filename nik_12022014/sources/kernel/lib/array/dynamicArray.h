#pragma once

#include "../iterator/iterator.h"
#include "../memory/memory.h"

template<class T>
class DynamicArray : public IIterator<T>
{
public:
	typedef Iterator<T> iterator;

private:
	friend class Iterator<T>;
	
	int position;

	virtual T* getNextPointer(T* _pItem)
	{
		if (position < size - 1)
		{
			position++;
			return ++_pItem;
		}
		else
			return nullptr;
	}

public:
	virtual Iterator<T> begin()
	{
		position = 0;
		return Iterator<T>(this, pData);
	}

protected:
	T* pData;
	int size;

public:
	DynamicArray()
		:	pData(nullptr), size(0)
	{
	}

	virtual ~DynamicArray()
	{
		if (pData != nullptr)
			delete[] pData;
	}

	T& operator[](int index)
	{
		return pData[index];
	}

	void resize(int newSize)
	{
		size = newSize;
		T* newDataPointer = new T[newSize];
		if (pData != nullptr)
		{
			memcpy(newDataPointer, pData, newSize * sizeof(T));
			delete[] pData;
		}

		pData = newDataPointer;
	}

	int getSize()
	{
		return size;
	}

	void swap(int indexL, int indexR)
	{
		T temp = pData[indexL];
		pData[indexL] = pData[indexR];
		pData[indexR] = temp;
	}
};

template<class T>
class SortedDynamicArray : public DynamicArray<T>
{
public:
	template<typename D>
	void orderBy(D T::* field, bool descending = false)
	{
		if (field == nullptr || pData == nullptr || size == 0)
			return;

		int jump = size;
		bool swapped = true;
 
		while (jump > 1 || swapped)
		{
			if (jump > 1)
				jump = static_cast<int>(jump / 1.25);
			
			swapped = false;
			
			for (int i = 0; i + jump < size; i++)
				if (!descending)
				{
					if (pData[i].*field > pData[i + jump].*field)
					{
						swap(i, i + jump);
						swapped = true;
					}
				}
				else
				{
					if (pData[i].*field < pData[i + jump].*field)
					{
						swap(i, i + jump);
						swapped = true;
					}
				}
		}
	}

	template<typename D, typename E>
	void orderBy(D T::* field1, E T::* field2, bool descending1 = false, bool descending2 = false)
	{
		orderBy(field1, descending1);

		if (field2 == nullptr || pData == nullptr || size == 0)
			return;

		int jump = size;
		bool swapped = true;
 
		while (jump > 1 || swapped)
		{
			if (jump > 1)
				jump = static_cast<int>(jump / 1.25);
			
			swapped = false;
			
			for (int i = 0; i + jump < size; i++)
			{
				bool res1 = (pData[i].*field1 == pData[i + jump].*field1);
				bool res2 = (pData[i].*field2 > pData[i + jump].*field2);

				if (descending2)
					res2 = !res2;

				if (res1 && res2)
				{
					swap(i, i + jump);
					swapped = true;
				}
			}
		}
	}

	template<typename D>
	D getMax(D T::* field)
	{
		if (field == nullptr || pData == nullptr || size == 0)
			return D();

		D res;
		bool first = true;

		for (int i = 0; i < size; i++)
		{
			if (first)
			{
				first = false;
				res = pData[i].*field;
			}
			else
			{
				if (res < pData[i].*field)
					res = pData[i].*field;
			}			
		} 

		return res;
	}

	template<typename D>
	D getMin(D T::* field)
	{
		if (field == nullptr || pData == nullptr || size == 0)
			return D();

		D res;
		bool first = true;

		for (int i = 0; i < size; i++)
		{
			if (first)
			{
				first = false;
				res = pData[i].*field;
			}
			else
			{
				if (res > pData[i].*field)
					res = pData[i].*field;
			}			
		} 

		return res;
	}
};
