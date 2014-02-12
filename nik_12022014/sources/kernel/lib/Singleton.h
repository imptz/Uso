/*! \file Singleton.h
    \brief Содержит класс Singleton.
    
*/
#pragma once

//! Класс синглетонов.
template<class T>
class Singleton
{
protected:
	//! Статический указатель на единственный объект класса.
	static T *pInstance;
public:
	//! Конструктор.
	/*!
		Создает объект класса.
	*/
	Singleton()
	{		
		pInstance = static_cast<T*>(this);
	}
	
	//! Деструктор.
	/*!
		Обнуляет статический указатель на объект класса.
	*/
	virtual ~Singleton()
	{		
		if (pInstance != nullptr)
			pInstance = nullptr;
	}
	
	//! Возвращает указатель на объект класса.
	/*!
		\param autoCreate если этот параметр == true и объект класса еще не создан, то он создается.
		\return указатель на объект класса.
	*/
	static T* getSingletonPtr(bool autoCreate = false)
	{
		if ((pInstance == nullptr) && (autoCreate))
			pInstance = new T();
		return pInstance;
	}
	
	//! Возвращает ссылку на объект класса.
	/*!
		\param autoCreate если этот параметр == true и объект класса еще не создан, то он создается.
		\return ссылку на объект класса.
	*/
	static T& getSingleton(bool autoCreate = true)
	{
		if ((pInstance == nullptr) && (autoCreate))
			pInstance = new T();
		return (*pInstance);
	}
};

template<class T>
T *Singleton<T>::pInstance = nullptr;
