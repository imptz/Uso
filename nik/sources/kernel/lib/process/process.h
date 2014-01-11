#ifndef PROCESS_H
#define PROCESS_H

#include "../Singleton.h"

template<class T>
struct CPointer;

template<class T>
struct TFPointer{
	typedef CPointer<T> (T::*FPointer)();
};

template <class T>
struct CPointer{
	typedef typename TFPointer<T>::FPointer TFP;

	CPointer(TFP _p)
		:	p(_p)
	{}

	operator TFP(){
		return p;
	}

	TFP p;
};

class ITask{
public:
	virtual void step() = 0;
};

template<class T>
class Task : public ITask{
protected:
	CPointer<T> ptr;

public:
	Task(typename TFPointer<T>::FPointer _ptr = nullptr)
		:	ptr(_ptr)
	{}

	virtual ~Task(void){}

	virtual void step(){
		if (ptr != nullptr)
			ptr = (reinterpret_cast<T*>(this)->*ptr)();
	}
};

class Process : public Singleton<Process>{
public:
	Process();
	void addTask(ITask *task);
	void step();

protected:
	static const unsigned int TASKS_COUNT = 4;
	unsigned int nextTaskIndex;
	ITask* *tasks;
};

#endif