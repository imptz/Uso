#pragma once

#pragma region example
/* ѕример использовани€:

#include <Thread.h>
#include <iostream>

class A : public Fr::Thread<A>
{
public:
	A()
		:	Thread(&A::f1)
	{}

private:
	Fr::CPointer<A> f1()
	{
		static bool g = false;
	
		if (g)
		{
			g = false;
			std::cout << "void f1() - true" << std::endl;
			return &A::f2;
		}
		else
		{
			g = true;
			std::cout << "void f1() - false" << std::endl;
			return &A::f1;
		}
	}


	Fr::CPointer<A> f2()
	{
		static bool g = false;
	
		if (g)
		{
			g = false;
			std::cout << "void f2() - true" << std::endl;
			return &A::f1;
		}
		else
		{
			g = true;
			std::cout << "void f2() - false" << std::endl;
			return &A::f2;
		}
	}
};


int _tmain(int argc, _TCHAR* argv[])
{
	A a;

	a.threadAction();
	a.threadAction();
	a.threadAction();
	a.threadAction();
	a.threadAction();
	
	return 0;
}
*/
#pragma endregion

namespace Fr
{
	template<class T>
	struct CPointer;
	
	//! —труктура хранит тип указател€ на метод класса.
	/*!
		Ќевозможно объ€вить шаблонный typedef, поэтому объ€вление типа указател€ упаковано в структуру. 
	*/
	template<class T>
	struct TFPointer
	{
		typedef CPointer<T> (T::*FPointer)();
	};

	//! —труктура - указатель на метод класса, метод возвращает указатель такого же типа.
	/*!
		Ќевозможно объ€вить тип указател€ вида: typedef FPointer (T::*FPointer)()
		ѕоэтому используетс€ typedef CPointer<T> (T::*FPointer)();
	*/
	template <class T>
	struct CPointer
	{
		typedef typename TFPointer<T>::FPointer TFP; ///< дл€ сокращени€ записи

		CPointer(TFP _p)
			:	p(_p)
		{}

		operator TFP()
		{
			return p;
		}

		typename TFP p;
	};

	class IThread
	{
		public:
			virtual void threadAction() = 0;
	};

	//!  Thread class
	/*!
		Ѕазовый класс задач дл€ псевдопараллельного выполнени€ в однопотоковой среде. 
		«адачи представл€ют собой классы наследуемые от Thread.

		¬ базовом классе отсутствуют методы выполн€емые задачей, они добавл€ютс€ только в базовом классе.  онструктор производного класса указывает стартовый метод задачи.
		ѕланировщик вызывает метод action, который запускает метод по указателю. ѕри выполнении метода не допускаютс€ циклы ожидани€ внешних данных, он должен максимально быстро 
		выполнитьс€ и вернуть управление. ≈сли метод выполнилс€ полностью, то он возвращает указатель на следующий по алгоритму метод задачи, если требуетс€ еще один вызов метода
		в следующем кванте времени, то он возвращает указатель на себ€.
	*/
	template<class T>
	class Thread : public IThread
	{
		CPointer<T> ptr; ///< указатель на текущий исполн€емый метод задачи

	public:
	    //! A constructor.
		/*!
			»нициализируетс€ указателем на метод задачи с которого начинаетс€ выполнение задачи.
		*/
		Thread(typename TFPointer<T>::FPointer _ptr = nullptr)
			:	ptr(_ptr)
		{
		}

		virtual ~Thread(void)
		{
		}

	    //! ћетод вызываетс€ планировщиком при активации задачи
		/*!
			¬ыполн€ет метод по указателю и обновл€ет указатель значением, которое вернул выполн€емый метод.
		*/
		virtual void threadAction()
		{
			if (ptr != nullptr)
				ptr = (reinterpret_cast<T*>(this)->*ptr)();
		}
	};
}
