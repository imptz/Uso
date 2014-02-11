#pragma once

namespace Fr
{

	/** 
		@class Vector

		Динамический массив
	*/
	template<typename T>
	class Vector
	{
	protected:
		static const unsigned int START_ITEMS_COUNT = 100;
		static const unsigned int INC_ITEMS_COUNT = 100;
		T* items;
		unsigned int itemsCount;
		unsigned int itemsCountAllocated;

	public:
		Vector()
			:	items(nullptr), itemsCount(START_ITEMS_COUNT), itemsCountAllocated(0)
		{
			items = reinterpret_cast<T*>(malloc(sizeof(T) * itemsCount));
		}

		virtual ~Vector()
		{
			free(items);
		}

		unsigned int getSize()
		{
			return itemsCountAllocated;
		}

		T& operator[](unsigned int index)
		{
			if (index < itemsCountAllocated)
				return items[index];
			else
				return items[0];
		}

		void del(unsigned int index)
		{
			if (index < itemsCountAllocated)
			{
				for (unsigned int i = index; i < itemsCountAllocated - 1; i++)
					items[i] = items[i + 1];

				itemsCountAllocated--;
			}
		}

		void clear()
		{
			itemsCountAllocated = 0;
		}

		void add(T item)
		{
			if (itemsCountAllocated >= itemsCount)
			{
				itemsCount += INC_ITEMS_COUNT;
				T* newItems = reinterpret_cast<T*>(malloc(sizeof(T) * itemsCount));
				for (unsigned int i = 0; i < itemsCountAllocated; i++)
				{
					new (&newItems[i])(T);
					newItems[i] = items[i];
				}
				
				free(items);
				items = newItems;
			}

			new (&items[itemsCountAllocated]) T(item);
			itemsCountAllocated++;
		}

		int getItemIndex(T value)
		{
			for (unsigned int i = 0; i < itemsCountAllocated; i++)
				if (items[i] == value)
					return i;

			return -1;
		}
	};

	/** 
		@class Vector

		Динамический массив с методом поиска объекта массива у которого заданное свойство находится в заданном значении.
		При этом объекты, содержащиеся в массиве должны определять поля в виде свойств (property.h)
	*/
	template<typename T>
	class PropertyVector : public Vector<T>
	{
	public:
		template<typename D>
		int find(typename D::property_type value, unsigned int startIndex = 0)
		{
			for (unsigned int i = startIndex; i < itemsCountAllocated; i++)
				if (items[i].get<D>() == value)
					return i;

			return -1;
		}
	};
}
