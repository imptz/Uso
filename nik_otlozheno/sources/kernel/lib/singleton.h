#pragma once

template<class T>
class Singleton{
protected:
	static T *pInstance;
public:
	Singleton(){		
		pInstance = static_cast<T*>(this);
	}
	
	virtual ~Singleton(){		
		if (pInstance != nullptr)
			pInstance = nullptr;
	}
	
	static T* getSingletonPtr(bool autoCreate = false){
		if ((pInstance == nullptr) && (autoCreate))
			pInstance = new T();
		return pInstance;
	}
	
	static T& getSingleton(bool autoCreate = true){
		if ((pInstance == nullptr) && (autoCreate))
			pInstance = new T();
		return (*pInstance);
	}
};

template<class T>
T *Singleton<T>::pInstance = nullptr;
