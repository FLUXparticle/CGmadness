#ifndef Callback_hpp
#define Callback_hpp

class Functor
{
public:
	virtual ~Functor() {}
	
  virtual void operator() () = 0;
};

class FunctionFunctor : public Functor
{
	void (*mF) ();
public:
	FunctionFunctor(void (*f) ()) :
		mF(f)
	{
		// empty
	}
	
	void operator() ()
	{
		mF();
	}
};

class Caller : public Functor
{
	Functor* mF;
public:
	Caller()
	{
		// empty
	}
	Caller(Functor* f) :
		mF(f)
	{
		// empty
	}
	Caller(void (*f) ()) :
		mF(new FunctionFunctor(f))
	{
		// empty
	}
	void operator() ()
	{
		(*mF) ();
	};
};

template<class T>
class Callback : public Functor
{
	T* mObject;
	void (T::*mF)();
	
public:
  Callback(T* object, void (T::*f)()) :
  	mObject(object),
  	mF(f)
  {
  	// empty
  }
  
  void operator() ()
  {
  	(mObject ->* mF) ();
  }
};

#define CALLBACK(X, x) Caller(new Callback<X>(this, &X::x))

#endif
