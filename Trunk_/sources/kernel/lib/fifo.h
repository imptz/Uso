/*! \file fifo.h
    \brief 
    
*/

#pragma once

#include "memory\memory.h"

//! Порядок расположения указателей чтения и записи очереди от физического начала области памяти (для реализации кольца).
enum FIFO_ORDER_FLAG
{
	FIFO_ORDER_R_BEFORE_W = 1, /*!< Указатель чтения перед указателем записи */
	FIFO_ORDER_W_BEFORE_R = 2  /*!< Указатель записи перед указателем чтения */
};

//! Шаблонный класс хранилища FIFO
/*!
	Класс организует хранение данных в виде кольца для исклбчения перемещения областей памяти в процессе работы.
	Шаблонный параметр T определяет тип сохраняемого значения.
*/
template<class T> class Fifo
{
private:
	T *data; /*!< Указатель на данные хранилища */
	unsigned int size; /*!< Максимальное число элементов в хранилище */
	unsigned int rPosition; /*!< позиция (от 0 до size - 1) элемента для чтения  */
	unsigned int wPosition; /*!< позиция (от 0 до size - 1) элемента для записи  */
public:
	FIFO_ORDER_FLAG order;  /*!< порядок расположения указателей чтения и записи  */

	static const int DEFAULT_SIZE = 1000; /*!< размер хранилища по умолчанию */

	Fifo()
	{}

	/**
	* Получить число свободных ячеек.
	* @return Сколько еще элементов можно записать в хранилище.
	*/
	unsigned int getAvailableWriteSize()
	{
		if (order == FIFO_ORDER_W_BEFORE_R)
			return rPosition - wPosition;
		else
			return rPosition + size - wPosition;
	}

public:

	//! Конструктор.
	/*!
		Распределяет память под заданное число элементов

		@param _size максимальное число элементов в хранилище
	*/
	Fifo(unsigned int _size)
		:	size(_size), rPosition(0), wPosition(0), order(FIFO_ORDER_R_BEFORE_W)
	{
		data = new T[size];
	}

	//! Деструктор.
	/*!
		Освобождает память элементов
	*/
	~Fifo()
	{
		if (data != nullptr)
			delete[] data;
	}

	//! Поместить элемент в хранилище
	/*!
		Проверяет есть ли в хранилище свободное место. Если есть, то сохраняет эелемент. Если нет, то не сохраняет.

		@param value размещаемый элемент

		@return true - элемент помещен в хранилище, false - элемент не помещен в хранилище
	*/
	bool put(T value)
	{
		if (order == FIFO_ORDER_W_BEFORE_R)
			if (wPosition == rPosition)
				return false;

		data[wPosition] = value;
		wPosition++;
		if (wPosition == size)
		{
			wPosition = 0;
			order = FIFO_ORDER_W_BEFORE_R;
		}
		return true;
	}

	//! Поместить несколько элементов в хранилище
	/*!
		Проверяет есть ли в хранилище свободное место (для всех элементов). Если есть, то сохраняет все элементы. Если нет, то все не сохраняет.

		@param pData - указатель на элементы
		@param length - число элементов

		@return true - элементы помещены в хранилище, false - элементы не помещены в хранилище
	*/
	bool put(T *pData, unsigned int length = 1)
	{
		if (length > getAvailableWriteSize())
			return false;

		for (unsigned int i = 0; i < length; i++)
			put(pData[i]);
		return true;
	}

	//! Получить несколько элементов из хранилища
	/*!
		Проверяет сколько элементов находится в хранилище и возвращает не более запрашиваемого числа элементов.

		@param pData - указатель на область памяти куда копируются получаемые элементы
		@param count - число элементов которые требуется получить
		@param remove - true - удалить получаемые элементы из хранилища, false - не удалять

		@return число полученных элементов:
			0, если в хранилище нет элементов; 
			меньше count, если в хранилище меньше count элементов; 
			count, если в хранилище не меньше count элементов
	*/
	unsigned int get(T* pData, unsigned int count = 1, bool remove = true)
	{
		if (order == FIFO_ORDER_R_BEFORE_W)
		{
			if (wPosition == rPosition)
				return 0;
			else
			{
				unsigned int rSize = wPosition - rPosition;
				
				if (count > rSize)
					count = rSize;

				memcpy(reinterpret_cast<unsigned char*>(pData), data + rPosition, count * sizeof(T));

				if (remove)
					rPosition += count;

				return count;
			}
		}

		unsigned int rSize = size - rPosition;
		
		if (count <= rSize)
		{
			memcpy(reinterpret_cast<unsigned char*>(pData), data + rPosition, count * sizeof(T));

			if (remove)
			{
				rPosition += count;
				if (rPosition == size)
				{
					rPosition = 0;
					order = FIFO_ORDER_R_BEFORE_W;
				}
			}

			return count;
		}

		memcpy(reinterpret_cast<unsigned char*>(pData), data + rPosition, rSize * sizeof(T));

		unsigned int dopCount = 0;
		if (count - rSize > wPosition)
			dopCount = wPosition;
		else
			dopCount = count - rSize;

		memcpy(reinterpret_cast<unsigned char*>(pData) + rSize, data, dopCount * sizeof(T));

		if (remove)
		{
			rPosition = dopCount;
			order = FIFO_ORDER_R_BEFORE_W;
		}

		return rSize + dopCount;
	}

	//! Проверка хранилища на пустоту.
	/*!
		@return true - в хранилище нет элементов, false - в хранилище есть хотя бы один элемент.
	*/
	bool isEmpty()
	{
		if (order == FIFO_ORDER_R_BEFORE_W)
			return (rPosition == wPosition);
		else
			return false;
	}

	//! Удаление элементов из хранилища.
	/*!
		@param count - количество удаляемых элементов
	*/	
	void remove(unsigned int count = 1)
	{
		if (order == FIFO_ORDER_R_BEFORE_W)
		{
			unsigned int rSize = wPosition - rPosition;
			if (count > rSize)
				count = rSize;

			rPosition += count;
			return;
		}

		unsigned int rSize = size - rPosition;
		if (count <= rSize)
		{
			rPosition += count;
			if (rPosition == size)
			{
				rPosition = 0;
				order = FIFO_ORDER_R_BEFORE_W;
			}
			return;
		}

		unsigned int dopCount = 0;
		if (count - rSize > wPosition)
			dopCount = wPosition;
		else
			dopCount = count - rSize;

		rPosition = dopCount;
		order = FIFO_ORDER_R_BEFORE_W;
	}

	//! Очистка хранилища.
	/*!
		Удаление всех элементов из хранилища.
	*/	
	void clear()
	{
		rPosition = 0;
		wPosition = 0;
		order = FIFO_ORDER_R_BEFORE_W;
	}

	//! Получить число элементов в хранилище.
	/*!
		@return число элементов в хранилище.
	*/	
	unsigned int getDataSize()
	{
		if (order == FIFO_ORDER_R_BEFORE_W)
			return (wPosition - rPosition);
		else
			return size - rPosition + wPosition;
	}
};
