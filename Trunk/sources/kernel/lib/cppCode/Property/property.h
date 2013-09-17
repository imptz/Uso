/*
	class P
	{
		INIT_PROPERTY

		PROPERTY(int, a)
		PROPERTY(int, b)
		PROPERTY(float, c)
	};

	void func()
	{
		set<P::b>(23);
		get<P::c>();
	}
*/

#pragma once 

#define INIT_PROPERTY								\
	public:											\
	template<typename D>							\
	void set(typename D::property_type value)		\
	{}												\
	template<typename D>							\
	typename D::property_type get()					\
	{}												\

#define PROPERTY(type, name)						\
	private:										\
	public:											\
		type	_##name;							\
	public:											\
		struct name									\
		{											\
			typedef type property_type;				\
		};											\
		template<>									\
		void set<name>(name::property_type value)	\
		{											\
			_##name## = value;						\
		}											\
		template<>									\
		name::property_type get<name>()				\
		{											\
			return _##name##;						\
		}
