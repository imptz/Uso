#pragma once

template<class T>
class Iterator;

template<class T>
class IIterator
{
private:
	friend class Iterator<T>;
	virtual T* getNextPointer(T* _pItem) = 0;
public:
	virtual Iterator<T> begin() = 0;
	Iterator<T> end()
	{
		return Iterator<T>();
	}
};

template<class T>
class Iterator
{
private:
	IIterator<T>* pObject;
	T* pItem;

public:
	Iterator(void)
		:	pObject(nullptr), pItem(nullptr)
	{
	}

	Iterator(IIterator<T>* _pObject, T* _pItem)
		:	pObject(_pObject), pItem(_pItem)
	{
	}

	Iterator(Iterator<T>& iter)
		:	pObject(iter.pObject), pItem(iter.pItem)
	{
	}

	virtual ~Iterator(void)
	{
	}

	T& operator*()
	{
		return *pItem;
	}

	T* operator->()
	{
		return pItem;
	}

	T* operator++()
	{
		pItem = pObject->getNextPointer(pItem); 
		return pItem;
	}

	T operator=(Iterator<T> iter)
	{
		pObject = iter.pObject;
		pItem = iter.pItem;

		return *pItem;
	}

	bool operator==(Iterator<T>& iter)
	{
		return pItem == iter.pItem;
	}

	bool operator!=(Iterator<T>& iter)
	{
		return pItem != iter.pItem;
	}


};
