#include "pastelsystest.h"

using namespace Pastel;

namespace
{

	template <typename Type>
	class CountedPtr
	{
	public:
		CountedPtr()
		{
		}

		CountedPtr(const CountedPtr<Type>& that)
		{
		}

		template <typename ThatType>
		CountedPtr(const CountedPtr<ThatType>& that)
		{
		}
	};

	class Base
	{
	};

	class Derived
		: public Base
	{
	};

	void f(const Base* base)
	{
	}

	void f(const CountedPtr<const Base>& base)
	{
	}

	class SomeClass
	{
	public:
		SomeClass(const Base* base)
		{
		}

		SomeClass(const CountedPtr<const Base>& base)
		{
		}
	};

	void g(const SomeClass& someClass)
	{
	}

	void testSyntax()
	{
		{
			Derived* derived = 0;
			const Derived* constDerived = derived;
			Base* base = derived;
			const Base* constBase = base;

			f(constBase);
			f(base);
			f(constDerived);
			f(derived);

			base = derived;
			constBase = constDerived;
			constBase = derived;

			g(constBase);
			g(base);
			g(constDerived);
			g(derived);
		}

		{
			CountedPtr<Derived> derived;
			CountedPtr<const Derived> constDerived(derived);
			CountedPtr<Base> base(derived);
			CountedPtr<const Base> constBase(base);

			f(constBase);
			f(base);
			f(constDerived);
			f(derived);

			base = derived;
			constBase = constDerived;
			constBase = derived;

			g(constBase);
			/*
			g(base);
			g(constDerived);
			g(derived);
			*/
		}
	}

	void testBegin()
	{
		testSyntax();
	}

	void testAdd()
	{
		//sysTestList().add("CountedPtr", testBegin);
	}

	CallFunction run(testAdd);

}
