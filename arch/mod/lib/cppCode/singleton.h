#pragma once

template<class T>
class SingletonM
{
private:
	SingletonM();
public:
	T& getInstance()
	{
		static T instance;
		return instance;
	}
};

template<class T>
class Singleton
{
protected:
	static T *pInstance;
public:
	Singleton()
	{		
		pInstance = static_cast<T*>(this);
	}
	
	virtual ~Singleton()
	{		
		if (pInstance != nullptr)
			pInstance = nullptr;
	}
	
	static T* getInstancePtr(bool autoCreate = true)
	{
		if ((pInstance == nullptr) && (autoCreate))
			pInstance = new T();
		return pInstance;
	}
	
	static T& getInstance(bool autoCreate = true)
	{
		if ((pInstance == nullptr) && (autoCreate))
			pInstance = new T();
		return (*pInstance);
	}
};

template<class T>
T *Singleton<T>::pInstance = nullptr;
