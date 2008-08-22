#ifndef Singleton_hpp
#define Singleton_hpp

template<class T>
class Singleton {
private:
	static T* sT;

public:
	Singleton()
	{
		if (!sT)
		{
			sT = new T();
		}
	}
	
	operator T* ()
	{
		return sT;
	}
	
};

template<class T>
T* Singleton<T>::sT = NULL;

#endif
