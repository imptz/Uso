#pragma once

#include <memory>

using namespace std;

// ================================
#define ICALLOBJECT_VARIANT_RESULT

#ifdef ICALLOBJECT_VARIANT_RESULT
class Variant
{
public:
    template <class T>
    Variant& operator = (T const& t)
    {
        typedef Type<T> inType;
        value = auto_ptr<inType>(new inType(t));
        return *this;
    }
    template <class T>
    operator T ()
    {
        typedef Type<T> inType;
        inType& sType = dynamic_cast<inType&>(*value);
        return sType.get();
    }
private:
    class IType
    {
    public:
        virtual ~IType() {}
    };
    typedef auto_ptr<IType> pIType;
    template <class T>
    class Type : public IType
    {
    public:
        Type(T const& t)
        : value(t)
        {
        }
        T get() const
        {
            return value;
        }
    private:
        T value;
    };
    pIType value;
};
#endif

class ICallObject
{
public:
	virtual void call() = 0;
#ifdef ICALLOBJECT_VARIANT_RESULT
	virtual Variant getResult() = 0;
#else
	virtual void* getResult() = 0;
#endif
	virtual void setReceiver(void* pObj) = 0;
	virtual ICallObject* clone() = 0;
	virtual ~ICallObject(){}
};

//=== R , Arg - 10 ==========================================================================
template <typename R, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
	typename Arg5,typename Arg6,typename Arg7,typename Arg8,typename Arg9,typename Arg10>
class CallObject : public ICallObject
{
typedef R (T::*TFunc)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10);
	TFunc pFunc;
	T* pObject;
#ifdef ICALLOBJECT_VARIANT_RESULT
	Variant result;
#else
	R result;
#endif
	Arg1 ar1;
	Arg2 ar2;
	Arg3 ar3;
	Arg4 ar4;
	Arg5 ar5;
	Arg6 ar6;
	Arg7 ar7;
	Arg8 ar8;
	Arg9 ar9;
	Arg10 ar10;

public:
	CallObject(TFunc _pFunc, T* _pObject, Arg1 _ar1, Arg2 _ar2, Arg3 _ar3, Arg4 _ar4, Arg5 _ar5,
		Arg6 _ar6, Arg7 _ar7, Arg8 _ar8, Arg9 _ar9, Arg10 _ar10) 
		:	pFunc(_pFunc), pObject(_pObject) , ar1(_ar1), ar2(_ar2), ar3(_ar3), ar4(_ar4), ar5(_ar5),
		ar6(_ar6), ar7(_ar7), ar8(_ar8), ar9(_ar9), ar10(_ar10)
	{ }

	void call()
	{
		result = (pObject->*pFunc)(ar1,ar2,ar3,ar4,ar5,ar6,ar7,ar8,ar9,ar10);
	}

#ifdef ICALLOBJECT_VARIANT_RESULT
	Variant getResult()
	{
		return result;
	}
#else
	void* getResult()
	{
		return &result;
	}
#endif

	void setReceiver(void* pObj)
	{
		pObject = static_cast<T*>(pObj);
	}

	ICallObject* clone()
	{
		return new CallObject<R,T,Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Arg8,Arg9,Arg10>(*this);
	}
};

template <typename R, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
	typename Arg5,typename Arg6,typename Arg7,typename Arg8,typename Arg9,typename Arg10>
inline ICallObject* createCallObject(
  R (T::*pFunc)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10), 
  T* pObject, 
  Arg1 ar1,
  Arg2 ar2,
  Arg3 ar3,
  Arg4 ar4,
  Arg5 ar5,
  Arg6 ar6,
  Arg7 ar7,
  Arg8 ar8,
  Arg9 ar9,
  Arg10 ar10) 
{
  return new CallObject<R, T, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10>
	  (pFunc, pObject, ar1,ar2,ar3,ar4,ar5,ar6,ar7,ar8,ar9,ar10);
}
//=== void , Arg - 10 ==========================================================================
template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
	typename Arg5,typename Arg6,typename Arg7,typename Arg8,typename Arg9,typename Arg10>
class CallObject<void, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
	typename Arg5,typename Arg6,typename Arg7,typename Arg8,typename Arg9,typename Arg10> : public ICallObject
{
typedef void (T::*TFunc)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10);
	TFunc pFunc;
	T* pObject;
	Arg1 ar1;
	Arg2 ar2;
	Arg3 ar3;
	Arg4 ar4;
	Arg5 ar5;
	Arg6 ar6;
	Arg7 ar7;
	Arg8 ar8;
	Arg9 ar9;
	Arg10 ar10;

public:
	CallObject(TFunc _pFunc, T* _pObject, Arg1 _ar1, Arg2 _ar2, Arg3 _ar3, Arg4 _ar4, Arg5 _ar5,
		Arg6 _ar6, Arg7 _ar7, Arg8 _ar8, Arg9 _ar9, Arg10 _ar10) 
		:	pFunc(_pFunc), pObject(_pObject) , ar1(_ar1), ar2(_ar2), ar3(_ar3), ar4(_ar4), ar5(_ar5),
		ar6(_ar6), ar7(_ar7), ar8(_ar8), ar9(_ar9), ar10(_ar10)
	{ }

	void call()
	{
		(pObject->*pFunc)(ar1,ar2,ar3,ar4,ar5,ar6,ar7,ar8,ar9,ar10);
	}

#ifdef ICALLOBJECT_VARIANT_RESULT
	Variant getResult()
	{
		Variant v;
		v = 0;
		return v;
	}
#else
	void* getResult()
	{
		return 0;
	}
#endif

	void setReceiver(void* pObj)
	{
		pObject = static_cast<T*>(pObj);
	}

	ICallObject* clone()
	{
		return new CallObject<void,T,Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Arg8,Arg9,Arg10>(*this);
	}
};

template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
	typename Arg5,typename Arg6,typename Arg7,typename Arg8,typename Arg9,typename Arg10>
inline ICallObject* createCallObject(
  void (T::*pFunc)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10), 
  T* pObject, 
  Arg1 ar1,
  Arg2 ar2,
  Arg3 ar3,
  Arg4 ar4,
  Arg5 ar5,
  Arg6 ar6,
  Arg7 ar7,
  Arg8 ar8,
  Arg9 ar9,
  Arg10 ar10) 
{
  return new CallObject<void, T, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10>
	  (pFunc, pObject, ar1,ar2,ar3,ar4,ar5,ar6,ar7,ar8,ar9,ar10);
}
//=== R , Arg - 9 ==========================================================================
template <typename R, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
	typename Arg5,typename Arg6,typename Arg7,typename Arg8,typename Arg9>
class CallObject<R, T, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
	typename Arg5,typename Arg6,typename Arg7,typename Arg8,typename Arg9, void> : public ICallObject
{
typedef R (T::*TFunc)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9);
	TFunc pFunc;
	T* pObject;
#ifdef ICALLOBJECT_VARIANT_RESULT
	Variant result;
#else
	R result;
#endif
	Arg1 ar1;
	Arg2 ar2;
	Arg3 ar3;
	Arg4 ar4;
	Arg5 ar5;
	Arg6 ar6;
	Arg7 ar7;
	Arg8 ar8;
	Arg9 ar9;

public:
	CallObject(TFunc _pFunc, T* _pObject, Arg1 _ar1, Arg2 _ar2, Arg3 _ar3, Arg4 _ar4, Arg5 _ar5,
		Arg6 _ar6, Arg7 _ar7, Arg8 _ar8, Arg9 _ar9) 
		:	pFunc(_pFunc), pObject(_pObject) , ar1(_ar1), ar2(_ar2), ar3(_ar3), ar4(_ar4), ar5(_ar5),
		ar6(_ar6), ar7(_ar7), ar8(_ar8), ar9(_ar9)
	{ }

	void call()
	{
		result = (pObject->*pFunc)(ar1,ar2,ar3,ar4,ar5,ar6,ar7,ar8,ar9);
	}

#ifdef ICALLOBJECT_VARIANT_RESULT
	Variant getResult()
	{
		return result;
	}
#else
	void* getResult()
	{
		return &result;
	}
#endif

	void setReceiver(void* pObj)
	{
		pObject = static_cast<T*>(pObj);
	}

	ICallObject* clone()
	{
		return new CallObject<R,T,Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Arg8,Arg9, void>(*this);
	}
};

template <typename R, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
	typename Arg5,typename Arg6,typename Arg7,typename Arg8,typename Arg9>
inline ICallObject* createCallObject(
  R (T::*pFunc)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9), 
  T* pObject, 
  Arg1 ar1,
  Arg2 ar2,
  Arg3 ar3,
  Arg4 ar4,
  Arg5 ar5,
  Arg6 ar6,
  Arg7 ar7,
  Arg8 ar8,
  Arg9 ar9) 
{
  return new CallObject<R, T, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, void>
	  (pFunc, pObject, ar1,ar2,ar3,ar4,ar5,ar6,ar7,ar8,ar9);
}
//=== void , Arg - 9 ==========================================================================
template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
	typename Arg5,typename Arg6,typename Arg7,typename Arg8,typename Arg9>
class CallObject<void, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
	typename Arg5,typename Arg6,typename Arg7,typename Arg8,typename Arg9,void> : public ICallObject
{
typedef void (T::*TFunc)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9);
	TFunc pFunc;
	T* pObject;
	Arg1 ar1;
	Arg2 ar2;
	Arg3 ar3;
	Arg4 ar4;
	Arg5 ar5;
	Arg6 ar6;
	Arg7 ar7;
	Arg8 ar8;
	Arg9 ar9;

public:
	CallObject(TFunc _pFunc, T* _pObject, Arg1 _ar1, Arg2 _ar2, Arg3 _ar3, Arg4 _ar4, Arg5 _ar5,
		Arg6 _ar6, Arg7 _ar7, Arg8 _ar8, Arg9 _ar9) 
		:	pFunc(_pFunc), pObject(_pObject) , ar1(_ar1), ar2(_ar2), ar3(_ar3), ar4(_ar4), ar5(_ar5),
		ar6(_ar6), ar7(_ar7), ar8(_ar8), ar9(_ar9)
	{ }

	void call()
	{
		(pObject->*pFunc)(ar1,ar2,ar3,ar4,ar5,ar6,ar7,ar8,ar9);
	}

#ifdef ICALLOBJECT_VARIANT_RESULT
	Variant getResult()
	{
		Variant v;
		v = 0;
		return v;
	}
#else
	void* getResult()
	{
		return 0;
	}
#endif

	void setReceiver(void* pObj)
	{
		pObject = static_cast<T*>(pObj);
	}

	ICallObject* clone()
	{
		return new CallObject<void,T,Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Arg8,Arg9,void>(*this);
	}
};

template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
	typename Arg5,typename Arg6,typename Arg7,typename Arg8,typename Arg9>
inline ICallObject* createCallObject(
  void (T::*pFunc)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9), 
  T* pObject, 
  Arg1 ar1,
  Arg2 ar2,
  Arg3 ar3,
  Arg4 ar4,
  Arg5 ar5,
  Arg6 ar6,
  Arg7 ar7,
  Arg8 ar8,
  Arg9 ar9) 
{
  return new CallObject<void, T, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, void>
	  (pFunc, pObject, ar1,ar2,ar3,ar4,ar5,ar6,ar7,ar8,ar9);
}
//=== R , Arg - 8 ==========================================================================
template <typename R, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
	typename Arg5,typename Arg6,typename Arg7,typename Arg8>
class CallObject<R, T, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
	typename Arg5,typename Arg6,typename Arg7,typename Arg8,void,void> : public ICallObject
{
typedef R (T::*TFunc)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8);
	TFunc pFunc;
	T* pObject;
#ifdef ICALLOBJECT_VARIANT_RESULT
	Variant result;
#else
	R result;
#endif
	Arg1 ar1;
	Arg2 ar2;
	Arg3 ar3;
	Arg4 ar4;
	Arg5 ar5;
	Arg6 ar6;
	Arg7 ar7;
	Arg8 ar8;

public:
	CallObject(TFunc _pFunc, T* _pObject, Arg1 _ar1, Arg2 _ar2, Arg3 _ar3, Arg4 _ar4, Arg5 _ar5,
		Arg6 _ar6, Arg7 _ar7, Arg8 _ar8) 
		:	pFunc(_pFunc), pObject(_pObject) , ar1(_ar1), ar2(_ar2), ar3(_ar3), ar4(_ar4), ar5(_ar5),
		ar6(_ar6), ar7(_ar7), ar8(_ar8)
	{ }

	void call()
	{
		result = (pObject->*pFunc)(ar1,ar2,ar3,ar4,ar5,ar6,ar7,ar8);
	}

#ifdef ICALLOBJECT_VARIANT_RESULT
	Variant getResult()
	{
		return result;
	}
#else
	void* getResult()
	{
		return &result;
	}
#endif

	void setReceiver(void* pObj)
	{
		pObject = static_cast<T*>(pObj);
	}

	ICallObject* clone()
	{
		return new CallObject<R,T,Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Arg8, void, void>(*this);
	}
};

template <typename R, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
	typename Arg5,typename Arg6,typename Arg7,typename Arg8>
inline ICallObject* createCallObject(
  R (T::*pFunc)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8), 
  T* pObject, 
  Arg1 ar1,
  Arg2 ar2,
  Arg3 ar3,
  Arg4 ar4,
  Arg5 ar5,
  Arg6 ar6,
  Arg7 ar7,
  Arg8 ar8) 
{
  return new CallObject<R, T, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, void, void>
	  (pFunc, pObject, ar1,ar2,ar3,ar4,ar5,ar6,ar7,ar8);
}
//=== void , Arg - 8 ==========================================================================
template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
	typename Arg5,typename Arg6,typename Arg7,typename Arg8>
class CallObject<void, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
	typename Arg5,typename Arg6,typename Arg7,typename Arg8,void,void> : public ICallObject
{
typedef void (T::*TFunc)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8);
	TFunc pFunc;
	T* pObject;
	Arg1 ar1;
	Arg2 ar2;
	Arg3 ar3;
	Arg4 ar4;
	Arg5 ar5;
	Arg6 ar6;
	Arg7 ar7;
	Arg8 ar8;

public:
	CallObject(TFunc _pFunc, T* _pObject, Arg1 _ar1, Arg2 _ar2, Arg3 _ar3, Arg4 _ar4, Arg5 _ar5,
		Arg6 _ar6, Arg7 _ar7, Arg8 _ar8) 
		:	pFunc(_pFunc), pObject(_pObject) , ar1(_ar1), ar2(_ar2), ar3(_ar3), ar4(_ar4), ar5(_ar5),
		ar6(_ar6), ar7(_ar7), ar8(_ar8)
	{ }

	void call()
	{
		(pObject->*pFunc)(ar1,ar2,ar3,ar4,ar5,ar6,ar7,ar8);
	}

#ifdef ICALLOBJECT_VARIANT_RESULT
	Variant getResult()
	{
		Variant v;
		v = 0;
		return v;
	}
#else
	void* getResult()
	{
		return 0;
	}
#endif

	void setReceiver(void* pObj)
	{
		pObject = static_cast<T*>(pObj);
	}

	ICallObject* clone()
	{
		return new CallObject<void,T,Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,Arg8,void,void>(*this);
	}
};

template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
	typename Arg5,typename Arg6,typename Arg7,typename Arg8>
inline ICallObject* createCallObject(
  void (T::*pFunc)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8), 
  T* pObject, 
  Arg1 ar1,
  Arg2 ar2,
  Arg3 ar3,
  Arg4 ar4,
  Arg5 ar5,
  Arg6 ar6,
  Arg7 ar7,
  Arg8 ar8) 
{
  return new CallObject<void, T, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, void, void>
	  (pFunc, pObject, ar1,ar2,ar3,ar4,ar5,ar6,ar7,ar8);
}
//=== R , Arg - 7 ==========================================================================
template <typename R, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
	typename Arg5,typename Arg6,typename Arg7>
class CallObject<R, T, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
	typename Arg5,typename Arg6,typename Arg7,void,void,void> : public ICallObject
{
typedef R (T::*TFunc)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7);
	TFunc pFunc;
	T* pObject;
#ifdef ICALLOBJECT_VARIANT_RESULT
	Variant result;
#else
	R result;
#endif
	Arg1 ar1;
	Arg2 ar2;
	Arg3 ar3;
	Arg4 ar4;
	Arg5 ar5;
	Arg6 ar6;
	Arg7 ar7;

public:
	CallObject(TFunc _pFunc, T* _pObject, Arg1 _ar1, Arg2 _ar2, Arg3 _ar3, Arg4 _ar4, Arg5 _ar5,
		Arg6 _ar6, Arg7 _ar7) 
		:	pFunc(_pFunc), pObject(_pObject) , ar1(_ar1), ar2(_ar2), ar3(_ar3), ar4(_ar4), ar5(_ar5),
		ar6(_ar6), ar7(_ar7)
	{ }

	void call()
	{
		result = (pObject->*pFunc)(ar1,ar2,ar3,ar4,ar5,ar6,ar7);
	}

#ifdef ICALLOBJECT_VARIANT_RESULT
	Variant getResult()
	{
		return result;
	}
#else
	void* getResult()
	{
		return &result;
	}
#endif

	void setReceiver(void* pObj)
	{
		pObject = static_cast<T*>(pObj);
	}

	ICallObject* clone()
	{
		return new CallObject<R,T,Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7, void, void, void>(*this);
	}
};

template <typename R, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
	typename Arg5,typename Arg6,typename Arg7>
inline ICallObject* createCallObject(
  R (T::*pFunc)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7), 
  T* pObject, 
  Arg1 ar1,
  Arg2 ar2,
  Arg3 ar3,
  Arg4 ar4,
  Arg5 ar5,
  Arg6 ar6,
  Arg7 ar7) 
{
  return new CallObject<R, T, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, void, void, void>
	  (pFunc, pObject, ar1,ar2,ar3,ar4,ar5,ar6,ar7);
}
//=== void , Arg - 7 ==========================================================================
template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
	typename Arg5,typename Arg6,typename Arg7>
class CallObject<void, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
	typename Arg5,typename Arg6,typename Arg7,void,void,void> : public ICallObject
{
typedef void (T::*TFunc)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7);
	TFunc pFunc;
	T* pObject;
	Arg1 ar1;
	Arg2 ar2;
	Arg3 ar3;
	Arg4 ar4;
	Arg5 ar5;
	Arg6 ar6;
	Arg7 ar7;

public:
	CallObject(TFunc _pFunc, T* _pObject, Arg1 _ar1, Arg2 _ar2, Arg3 _ar3, Arg4 _ar4, Arg5 _ar5,
		Arg6 _ar6, Arg7 _ar7) 
		:	pFunc(_pFunc), pObject(_pObject) , ar1(_ar1), ar2(_ar2), ar3(_ar3), ar4(_ar4), ar5(_ar5),
		ar6(_ar6), ar7(_ar7)
	{ }

	void call()
	{
		(pObject->*pFunc)(ar1,ar2,ar3,ar4,ar5,ar6,ar7);
	}

#ifdef ICALLOBJECT_VARIANT_RESULT
	Variant getResult()
	{
		Variant v;
		v = 0;
		return v;
	}
#else
	void* getResult()
	{
		return 0;
	}
#endif

	void setReceiver(void* pObj)
	{
		pObject = static_cast<T*>(pObj);
	}

	ICallObject* clone()
	{
		return new CallObject<void,T,Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,Arg7,void,void,void>(*this);
	}
};

template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
	typename Arg5,typename Arg6,typename Arg7>
inline ICallObject* createCallObject(
  void (T::*pFunc)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7), 
  T* pObject, 
  Arg1 ar1,
  Arg2 ar2,
  Arg3 ar3,
  Arg4 ar4,
  Arg5 ar5,
  Arg6 ar6,
  Arg7 ar7) 
{
  return new CallObject<void, T, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, void, void, void>
	  (pFunc, pObject, ar1,ar2,ar3,ar4,ar5,ar6,ar7);
}
//=== R , Arg - 6 ==========================================================================
template <typename R, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
	typename Arg5,typename Arg6>
class CallObject<R, T, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
	typename Arg5,typename Arg6,void,void,void,void> : public ICallObject
{
typedef R (T::*TFunc)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6);
	TFunc pFunc;
	T* pObject;
#ifdef ICALLOBJECT_VARIANT_RESULT
	Variant result;
#else
	R result;
#endif
	Arg1 ar1;
	Arg2 ar2;
	Arg3 ar3;
	Arg4 ar4;
	Arg5 ar5;
	Arg6 ar6;

public:
	CallObject(TFunc _pFunc, T* _pObject, Arg1 _ar1, Arg2 _ar2, Arg3 _ar3, Arg4 _ar4, Arg5 _ar5,
		Arg6 _ar6) 
		:	pFunc(_pFunc), pObject(_pObject) , ar1(_ar1), ar2(_ar2), ar3(_ar3), ar4(_ar4), ar5(_ar5),
		ar6(_ar6)
	{ }

	void call()
	{
		result = (pObject->*pFunc)(ar1,ar2,ar3,ar4,ar5,ar6);
	}

#ifdef ICALLOBJECT_VARIANT_RESULT
	Variant getResult()
	{
		return result;
	}
#else
	void* getResult()
	{
		return &result;
	}
#endif

	void setReceiver(void* pObj)
	{
		pObject = static_cast<T*>(pObj);
	}

	ICallObject* clone()
	{
		return new CallObject<R,T,Arg1,Arg2,Arg3,Arg4,Arg5,Arg6, void, void, void, void>(*this);
	}
};

template <typename R, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
	typename Arg5,typename Arg6>
inline ICallObject* createCallObject(
  R (T::*pFunc)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), 
  T* pObject, 
  Arg1 ar1,
  Arg2 ar2,
  Arg3 ar3,
  Arg4 ar4,
  Arg5 ar5,
  Arg6 ar6) 
{
  return new CallObject<R, T, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, void, void, void, void>
	  (pFunc, pObject, ar1,ar2,ar3,ar4,ar5,ar6);
}
//=== void , Arg - 6 ==========================================================================
template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
	typename Arg5,typename Arg6>
class CallObject<void, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
	typename Arg5,typename Arg6,void,void,void,void> : public ICallObject
{
typedef void (T::*TFunc)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6);
	TFunc pFunc;
	T* pObject;
	Arg1 ar1;
	Arg2 ar2;
	Arg3 ar3;
	Arg4 ar4;
	Arg5 ar5;
	Arg6 ar6;

public:
	CallObject(TFunc _pFunc, T* _pObject, Arg1 _ar1, Arg2 _ar2, Arg3 _ar3, Arg4 _ar4, Arg5 _ar5,
		Arg6 _ar6) 
		:	pFunc(_pFunc), pObject(_pObject) , ar1(_ar1), ar2(_ar2), ar3(_ar3), ar4(_ar4), ar5(_ar5),
		ar6(_ar6)
	{ }

	void call()
	{
		(pObject->*pFunc)(ar1,ar2,ar3,ar4,ar5,ar6);
	}

#ifdef ICALLOBJECT_VARIANT_RESULT
	Variant getResult()
	{
		Variant v;
		v = 0;
		return v;
	}
#else
	void* getResult()
	{
		return 0;
	}
#endif

	void setReceiver(void* pObj)
	{
		pObject = static_cast<T*>(pObj);
	}

	ICallObject* clone()
	{
		return new CallObject<void,T,Arg1,Arg2,Arg3,Arg4,Arg5,Arg6,void,void,void,void>(*this);
	}
};

template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
	typename Arg5,typename Arg6>
inline ICallObject* createCallObject(
  void (T::*pFunc)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), 
  T* pObject, 
  Arg1 ar1,
  Arg2 ar2,
  Arg3 ar3,
  Arg4 ar4,
  Arg5 ar5,
  Arg6 ar6) 
{
  return new CallObject<void, T, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, void, void, void, void>
	  (pFunc, pObject, ar1,ar2,ar3,ar4,ar5,ar6);
}
//=== R , Arg - 5 ==========================================================================
template <typename R, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
	typename Arg5>
class CallObject<R, T, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
	typename Arg5,void,void,void,void,void> : public ICallObject
{
typedef R (T::*TFunc)(Arg1, Arg2, Arg3, Arg4, Arg5);
	TFunc pFunc;
	T* pObject;
#ifdef ICALLOBJECT_VARIANT_RESULT
	Variant result;
#else
	R result;
#endif
	Arg1 ar1;
	Arg2 ar2;
	Arg3 ar3;
	Arg4 ar4;
	Arg5 ar5;

public:
	CallObject(TFunc _pFunc, T* _pObject, Arg1 _ar1, Arg2 _ar2, Arg3 _ar3, Arg4 _ar4, Arg5 _ar5) 
		:	pFunc(_pFunc), pObject(_pObject) , ar1(_ar1), ar2(_ar2), ar3(_ar3), ar4(_ar4), ar5(_ar5)
	{ }

	void call()
	{
		result = (pObject->*pFunc)(ar1,ar2,ar3,ar4,ar5);
	}

#ifdef ICALLOBJECT_VARIANT_RESULT
	Variant getResult()
	{
		return result;
	}
#else
	void* getResult()
	{
		return &result;
	}
#endif

	void setReceiver(void* pObj)
	{
		pObject = static_cast<T*>(pObj);
	}

	ICallObject* clone()
	{
		return new CallObject<R,T,Arg1,Arg2,Arg3,Arg4,Arg5, void, void, void, void, void>(*this);
	}
};

template <typename R, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
	typename Arg5>
inline ICallObject* createCallObject(
  R (T::*pFunc)(Arg1, Arg2, Arg3, Arg4, Arg5), 
  T* pObject, 
  Arg1 ar1,
  Arg2 ar2,
  Arg3 ar3,
  Arg4 ar4,
  Arg5 ar5) 
{
  return new CallObject<R, T, Arg1, Arg2, Arg3, Arg4, Arg5, void, void, void, void, void>
	  (pFunc, pObject, ar1,ar2,ar3,ar4,ar5);
}
//=== void , Arg - 5 ==========================================================================
template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
	typename Arg5>
class CallObject<void, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
	typename Arg5,void,void,void,void,void> : public ICallObject
{
typedef void (T::*TFunc)(Arg1, Arg2, Arg3, Arg4, Arg5);
	TFunc pFunc;
	T* pObject;
	Arg1 ar1;
	Arg2 ar2;
	Arg3 ar3;
	Arg4 ar4;
	Arg5 ar5;

public:
	CallObject(TFunc _pFunc, T* _pObject, Arg1 _ar1, Arg2 _ar2, Arg3 _ar3, Arg4 _ar4, Arg5 _ar5) 
		:	pFunc(_pFunc), pObject(_pObject) , ar1(_ar1), ar2(_ar2), ar3(_ar3), ar4(_ar4), ar5(_ar5)
	{ }

	void call()
	{
		(pObject->*pFunc)(ar1,ar2,ar3,ar4,ar5);
	}

#ifdef ICALLOBJECT_VARIANT_RESULT
	Variant getResult()
	{
		Variant v;
		v = 0;
		return v;
	}
#else
	void* getResult()
	{
		return 0;
	}
#endif

	void setReceiver(void* pObj)
	{
		pObject = static_cast<T*>(pObj);
	}

	ICallObject* clone()
	{
		return new CallObject<void,T,Arg1,Arg2,Arg3,Arg4,Arg5,void,void,void,void,void>(*this);
	}
};

template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
	typename Arg5>
inline ICallObject* createCallObject(
  void (T::*pFunc)(Arg1, Arg2, Arg3, Arg4, Arg5), 
  T* pObject, 
  Arg1 ar1,
  Arg2 ar2,
  Arg3 ar3,
  Arg4 ar4,
  Arg5 ar5) 
{
  return new CallObject<void, T, Arg1, Arg2, Arg3, Arg4, Arg5, void, void, void, void, void>
	  (pFunc, pObject, ar1,ar2,ar3,ar4,ar5);
}
//=== R , Arg - 4 ==========================================================================
template <typename R, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
class CallObject<R, T, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
	void,void,void,void,void,void> : public ICallObject
{
typedef R (T::*TFunc)(Arg1, Arg2, Arg3, Arg4);
	TFunc pFunc;
	T* pObject;
#ifdef ICALLOBJECT_VARIANT_RESULT
	Variant result;
#else
	R result;
#endif
	Arg1 ar1;
	Arg2 ar2;
	Arg3 ar3;
	Arg4 ar4;

public:
	CallObject(TFunc _pFunc, T* _pObject, Arg1 _ar1, Arg2 _ar2, Arg3 _ar3, Arg4 _ar4) 
		:	pFunc(_pFunc), pObject(_pObject) , ar1(_ar1), ar2(_ar2), ar3(_ar3), ar4(_ar4)
	{ }

	void call()
	{
		result = (pObject->*pFunc)(ar1,ar2,ar3,ar4);
	}

#ifdef ICALLOBJECT_VARIANT_RESULT
	Variant getResult()
	{
		return result;
	}
#else
	void* getResult()
	{
		return &result;
	}
#endif

	void setReceiver(void* pObj)
	{
		pObject = static_cast<T*>(pObj);
	}

	ICallObject* clone()
	{
		return new CallObject<R,T,Arg1,Arg2,Arg3,Arg4, void, void, void, void, void, void>(*this);
	}
};

template <typename R, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
inline ICallObject* createCallObject(
  R (T::*pFunc)(Arg1, Arg2, Arg3, Arg4), 
  T* pObject, 
  Arg1 ar1,
  Arg2 ar2,
  Arg3 ar3,
  Arg4 ar4) 
{
  return new CallObject<R, T, Arg1, Arg2, Arg3, Arg4, void, void, void, void, void, void>
	  (pFunc, pObject, ar1,ar2,ar3,ar4);
}
//=== void , Arg - 4 ==========================================================================
template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
class CallObject<void, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
	void,void,void,void,void,void> : public ICallObject
{
typedef void (T::*TFunc)(Arg1, Arg2, Arg3, Arg4);
	TFunc pFunc;
	T* pObject;
	Arg1 ar1;
	Arg2 ar2;
	Arg3 ar3;
	Arg4 ar4;

public:
	CallObject(TFunc _pFunc, T* _pObject, Arg1 _ar1, Arg2 _ar2, Arg3 _ar3, Arg4 _ar4) 
		:	pFunc(_pFunc), pObject(_pObject) , ar1(_ar1), ar2(_ar2), ar3(_ar3), ar4(_ar4)
	{ }

	void call()
	{
		(pObject->*pFunc)(ar1,ar2,ar3,ar4);
	}

#ifdef ICALLOBJECT_VARIANT_RESULT
	Variant getResult()
	{
		Variant v;
		v = 0;
		return v;
	}
#else
	void* getResult()
	{
		return 0;
	}
#endif

	void setReceiver(void* pObj)
	{
		pObject = static_cast<T*>(pObj);
	}

	ICallObject* clone()
	{
		return new CallObject<void,T,Arg1,Arg2,Arg3,Arg4,void,void,void,void,void,void>(*this);
	}
};

template <typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
inline ICallObject* createCallObject(
  void (T::*pFunc)(Arg1, Arg2, Arg3, Arg4), 
  T* pObject, 
  Arg1 ar1,
  Arg2 ar2,
  Arg3 ar3,
  Arg4 ar4) 
{
  return new CallObject<void, T, Arg1, Arg2, Arg3, Arg4, void, void, void, void, void, void>
	  (pFunc, pObject, ar1,ar2,ar3,ar4);
}
//=== R , Arg - 3 ==========================================================================
template <typename R, typename T, typename Arg1, typename Arg2, typename Arg3>
class CallObject<R, T, typename Arg1, typename Arg2, typename Arg3, void,
	void,void,void,void,void,void> : public ICallObject
{
typedef R (T::*TFunc)(Arg1, Arg2, Arg3);
	TFunc pFunc;
	T* pObject;
#ifdef ICALLOBJECT_VARIANT_RESULT
	Variant result;
#else
	R result;
#endif
	Arg1 ar1;
	Arg2 ar2;
	Arg3 ar3;

public:
	CallObject(TFunc _pFunc, T* _pObject, Arg1 _ar1, Arg2 _ar2, Arg3 _ar3) 
		:	pFunc(_pFunc), pObject(_pObject) , ar1(_ar1), ar2(_ar2), ar3(_ar3)
	{ }

	void call()
	{
		result = (pObject->*pFunc)(ar1,ar2,ar3);
	}

#ifdef ICALLOBJECT_VARIANT_RESULT
	Variant getResult()
	{
		return result;
	}
#else
	void* getResult()
	{
		return &result;
	}
#endif

	void setReceiver(void* pObj)
	{
		pObject = static_cast<T*>(pObj);
	}

	ICallObject* clone()
	{
		return new CallObject<R,T,Arg1,Arg2,Arg3,void, void, void, void, void, void, void>(*this);
	}
};

template <typename R, typename T, typename Arg1, typename Arg2, typename Arg3>
inline ICallObject* createCallObject(
  R (T::*pFunc)(Arg1, Arg2, Arg3), 
  T* pObject, 
  Arg1 ar1,
  Arg2 ar2,
  Arg3 ar3) 
{
  return new CallObject<R, T, Arg1, Arg2, Arg3, void, void, void, void, void, void, void>
	  (pFunc, pObject, ar1,ar2,ar3);
}
//=== void , Arg - 3 ==========================================================================
template <typename T, typename Arg1, typename Arg2, typename Arg3>
class CallObject<void, typename T, typename Arg1, typename Arg2, typename Arg3, void,
	void,void,void,void,void,void> : public ICallObject
{
typedef void (T::*TFunc)(Arg1, Arg2, Arg3);
	TFunc pFunc;
	T* pObject;
	Arg1 ar1;
	Arg2 ar2;
	Arg3 ar3;

public:
	CallObject(TFunc _pFunc, T* _pObject, Arg1 _ar1, Arg2 _ar2, Arg3 _ar3) 
		:	pFunc(_pFunc), pObject(_pObject) , ar1(_ar1), ar2(_ar2), ar3(_ar3)
	{ }

	void call()
	{
		(pObject->*pFunc)(ar1,ar2,ar3);
	}

#ifdef ICALLOBJECT_VARIANT_RESULT
	Variant getResult()
	{
		Variant v;
		v = 0;
		return v;
	}
#else
	void* getResult()
	{
		return 0;
	}
#endif

	void setReceiver(void* pObj)
	{
		pObject = static_cast<T*>(pObj);
	}

	ICallObject* clone()
	{
		return new CallObject<void,T,Arg1,Arg2,Arg3,void,void,void,void,void,void,void>(*this);
	}
};

template <typename T, typename Arg1, typename Arg2, typename Arg3>
inline ICallObject* createCallObject(
  void (T::*pFunc)(Arg1, Arg2, Arg3), 
  T* pObject, 
  Arg1 ar1,
  Arg2 ar2,
  Arg3 ar3) 
{
  return new CallObject<void, T, Arg1, Arg2, Arg3, void, void, void, void, void, void, void>
	  (pFunc, pObject, ar1,ar2,ar3);
}
//=== R , Arg - 2 ==========================================================================
template <typename R, typename T, typename Arg1, typename Arg2>
class CallObject<R, T, typename Arg1, typename Arg2,void,void,void,void,void,void,void,void> : public ICallObject
{
typedef R (T::*TFunc)(Arg1, Arg2);
	TFunc pFunc;
	T* pObject;
#ifdef ICALLOBJECT_VARIANT_RESULT
	Variant result;
#else
	R result;
#endif
	Arg1 ar1;
	Arg2 ar2;

public:
	CallObject(TFunc _pFunc, T* _pObject, Arg1 _ar1, Arg2 _ar2) 
		:	pFunc(_pFunc), pObject(_pObject) , ar1(_ar1), ar2(_ar2)
	{ }

	void call()
	{
		result = (pObject->*pFunc)(ar1,ar2);
	}

#ifdef ICALLOBJECT_VARIANT_RESULT
	Variant getResult()
	{
		return result;
	}
#else
	void* getResult()
	{
		return &result;
	}
#endif

	void setReceiver(void* pObj)
	{
		pObject = static_cast<T*>(pObj);
	}

	ICallObject* clone()
	{
		return new CallObject<R,T,Arg1,Arg2,void, void, void, void, void, void, void, void>(*this);
	}
};

template <typename R, typename T, typename Arg1, typename Arg2>
inline ICallObject* createCallObject(
  R (T::*pFunc)(Arg1, Arg2), 
  T* pObject, 
  Arg1 ar1,
  Arg2 ar2) 
{
  return new CallObject<R, T, Arg1, Arg2, void, void, void, void, void, void, void, void>
	  (pFunc, pObject, ar1,ar2);
}
//=== void , Arg - 2 ==========================================================================
template <typename T, typename Arg1, typename Arg2>
class CallObject<void, typename T, typename Arg1, typename Arg2, void, void,
	void,void,void,void,void,void> : public ICallObject
{
typedef void (T::*TFunc)(Arg1, Arg2);
	TFunc pFunc;
	T* pObject;
	Arg1 ar1;
	Arg2 ar2;

public:
	CallObject(TFunc _pFunc, T* _pObject, Arg1 _ar1, Arg2 _ar2) 
		:	pFunc(_pFunc), pObject(_pObject) , ar1(_ar1), ar2(_ar2)
	{ }

	void call()
	{
		(pObject->*pFunc)(ar1,ar2);
	}

#ifdef ICALLOBJECT_VARIANT_RESULT
	Variant getResult()
	{
		Variant v;
		v = 0;
		return v;
	}
#else
	void* getResult()
	{
		return 0;
	}
#endif

	void setReceiver(void* pObj)
	{
		pObject = static_cast<T*>(pObj);
	}

	ICallObject* clone()
	{
		return new CallObject<void,T,Arg1,Arg2,void,void,void,void,void,void,void,void>(*this);
	}
};

template <typename T, typename Arg1, typename Arg2>
inline ICallObject* createCallObject(
  void (T::*pFunc)(Arg1, Arg2), 
  T* pObject, 
  Arg1 ar1,
  Arg2 ar2) 
{
  return new CallObject<void, T, Arg1, Arg2, void, void, void, void, void, void, void, void>
	  (pFunc, pObject, ar1,ar2);
}
//=== R , Arg - 1 ==========================================================================
template <typename R, typename T, typename Arg1>
class CallObject<R, T, typename Arg1,void,void,void,void,void,void,void,void,void> : public ICallObject
{
typedef R (T::*TFunc)(Arg1);
	TFunc pFunc;
	T* pObject;
#ifdef ICALLOBJECT_VARIANT_RESULT
	Variant result;
#else
	R result;
#endif
	Arg1 ar1;

public:
	CallObject(TFunc _pFunc, T* _pObject, Arg1 _ar1) 
		:	pFunc(_pFunc), pObject(_pObject) , ar1(_ar1)
	{ }

	void call()
	{
		result = (pObject->*pFunc)(ar1);
	}

#ifdef ICALLOBJECT_VARIANT_RESULT
	Variant getResult()
	{
		return result;
	}
#else
	void* getResult()
	{
		return &result;
	}
#endif

	void setReceiver(void* pObj)
	{
		pObject = static_cast<T*>(pObj);
	}

	ICallObject* clone()
	{
		return new CallObject<R,T,Arg1,void, void, void, void, void, void, void, void, void>(*this);
	}
};

template <typename R, typename T, typename Arg1>
inline ICallObject* createCallObject(
  R (T::*pFunc)(Arg1), 
  T* pObject, 
  Arg1 ar1) 
{
  return new CallObject<R, T, Arg1, void, void, void, void, void, void, void, void, void>
	  (pFunc, pObject, ar1);
}
//=== void , Arg - 1 ==========================================================================
template <typename T, typename Arg1>
class CallObject<void, typename T, typename Arg1,void,void,void,void,void,void,void,void,void> : public ICallObject
{
typedef void (T::*TFunc)(Arg1);
	TFunc pFunc;
	T* pObject;
	Arg1 ar1;

public:
	CallObject(TFunc _pFunc, T* _pObject, Arg1 _ar1) 
		:	pFunc(_pFunc), pObject(_pObject) , ar1(_ar1)
	{ }

	void call()
	{
		(pObject->*pFunc)(ar1);
	}

#ifdef ICALLOBJECT_VARIANT_RESULT
	Variant getResult()
	{
		Variant v;
		v = 0;
		return v;
	}
#else
	void* getResult()
	{
		return 0;
	}
#endif

	void setReceiver(void* pObj)
	{
		pObject = static_cast<T*>(pObj);
	}

	ICallObject* clone()
	{
		return new CallObject<void,T,Arg1,void,void,void,void,void,void,void,void,void>(*this);
	}
};

template <typename T, typename Arg1>
inline ICallObject* createCallObject(
  void (T::*pFunc)(Arg1), 
  T* pObject, 
  Arg1 ar1) 
{
  return new CallObject<void, T, Arg1, void, void, void, void, void, void, void, void, void>
	  (pFunc, pObject, ar1);
}
// R , Arg - void ==================================
template <typename R, typename T>
class CallObject<R,T,void,void,void,void,void,void,void,void,void,void> : public ICallObject
{
typedef R (T::*TFunc)();
	TFunc pFunc;
	T* pObject;
#ifdef ICALLOBJECT_VARIANT_RESULT
	Variant result;
#else
	R result;
#endif

public:
	CallObject(TFunc _pFunc, T* _pObject) 
		:	pFunc(_pFunc), pObject(_pObject)
	{ }

	void call()
	{
		result = (pObject->*pFunc)();
	}

#ifdef ICALLOBJECT_VARIANT_RESULT
	Variant getResult()
	{
		return result;
	}
#else
	void* getResult()
	{
		return &result;
	}
#endif

	void setReceiver(void* pObj)
	{
		pObject = static_cast<T*>(pObj);
	}

	ICallObject* clone()
	{
		return new CallObject<R,T,void,void,void,void,void,void,void,void,void,void>(*this);
	}
};

template <typename R, typename T>
inline ICallObject* createCallObject(
  R (T::*pFunc)(), 
  T* pObject) 
{
  return new CallObject<R, T,void,void,void,void,void,void,void,void,void,void>(pFunc, pObject);
}
// R - void, Arg - void ===============================
template <typename T>
class CallObject<void,T,void,void,void,void,void,void,void,void,void,void> : public ICallObject
{
typedef void (T::*TFunc)();
	TFunc pFunc;
	T* pObject;

public:
	CallObject(TFunc _pFunc, T* _pObject) 
		:	pFunc(_pFunc), pObject(_pObject)
	{ }

	void call()
	{
		(pObject->*pFunc)();
	}

#ifdef ICALLOBJECT_VARIANT_RESULT
	Variant getResult()
	{
		Variant v;
		v = 0;
		return v;
	}
#else
	void* getResult()
	{
		return 0;
	}
#endif

	void setReceiver(void* pObj)
	{
		pObject = static_cast<T*>(pObj);
	}

	ICallObject* clone()
	{
		return new CallObject<void,T,void,void,void,void,void,void,void,void,void,void>(*this);
	}
};

template <typename T>
inline ICallObject* createCallObject(
  void (T::*pFunc)(), 
  T* pObject) 
{
  return new CallObject<void, T,void,void,void,void,void,void,void,void,void,void>(pFunc, pObject);
}
// ================================ 
