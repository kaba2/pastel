#include "pastelsystest.h"
#include "pastel/sys/parallelarray.h"

using namespace Pastel;

namespace
{

	class A
	{
	};

	void testCreate()
	{
		ParallelArray<integer> a;

		REPORT1(a.size() != 0, a.size());
		REPORT1(a.properties() != 0, a.properties());
		REPORT(!a.empty());

		ParallelArray<integer> b(a);
		a = b;
		a.swap(b);
		a.clear();

		REPORT1(a.size() != 0, a.size());
		REPORT1(a.properties() != 0, a.properties());
		REPORT(!a.empty());

		b.freeMem();

		REPORT1(b.size() != 0, b.size());
		REPORT1(b.properties() != 0, b.properties());
		REPORT(!b.empty());
	}

	void testPropertyCreate()
	{
		bool result = false;

		ParallelArray<integer> a;
		typedef PropertyId<integer, A> MyProperty;

		MyProperty aProperty(0);

		result = a.add(aProperty);

		REPORT(!result);

		MemoryView<A> data;
		result = a.get(aProperty, data);

		REPORT(!result);

		ConstMemoryView<A> readData;
		result = a.get(aProperty, readData);

		REPORT(!result);

		result = a.remove(aProperty);

		REPORT(!result);
	}

	void testResize()
	{
		ParallelArray<integer> a;

		const integer Size = 1043;

		a.resize(Size);

		REPORT(a.size() != Size);
		REPORT(a.empty());

		a.clear();

		REPORT(a.size() != 0);
		REPORT(!a.empty());

		a.resize(Size);

		REPORT(a.size() != Size);
		REPORT(a.empty());

		a.resize(0);
		REPORT(a.size() != 0);
		REPORT(!a.empty());
	}

	void testUse()
	{
		ParallelArray<integer> a;
		typedef PropertyId<integer, A> MyProperty;
		MyProperty aProperty(0);

		bool result = false;

		result = a.add(aProperty);

		REPORT(!result);

		const integer Size = 512;

		a.resize(Size);

		typedef PropertyId<integer, integer> IntegerProperty;

		IntegerProperty bProperty(1);

		result = a.add(bProperty);
		REPORT(!result);

		MemoryView<integer> intData;
		result = a.get(bProperty, intData);
		REPORT(!result);
		REPORT(intData.size() != Size);

		for (integer i = 0;i < intData.size();++i)
		{
			intData[i] = i;
		}

		ConstMemoryView<integer> readIntData;
		result = a.get(bProperty, readIntData);
		REPORT(!result);
		REPORT(readIntData.size() != Size);

		for (integer i = 0;i < readIntData.size();++i)
		{
			log() << readIntData[i] << ", ";
		}

		a.clear();
		REPORT(a.properties() != 2);

		a.freeMem();
		REPORT(a.properties() != 0);
	}

	void testBegin()
	{
		testCreate();
		testPropertyCreate();
		testResize();
		testUse();
	}

	void testAdd()
	{
		sysTestList().add("ParallelArray", testBegin);
	}

	CallFunction run(testAdd);

}
