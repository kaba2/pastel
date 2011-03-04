// Description: Testing for ParallelArray
// DocumentationOf: parallelarray.h

#include "pastelsystest.h"
#include "pastel/sys/parallelarray.h"

using namespace Pastel;

namespace
{

	class A
	{
	};

	class Test
		: public TestSuite
	{
	public:
		Test()
			: TestSuite(&sysTestReport())
		{
		}

		virtual void run()
		{
			testCreate();
			testPropertyCreate();
			testResize();
			testUse();
		}

		void testCreate()
		{
			ParallelArray<integer> a;

			TEST_ENSURE_OP(a.size(), ==, 0);
			TEST_ENSURE_OP(a.properties(), ==, 0);
			TEST_ENSURE(a.empty());

			ParallelArray<integer> b(a);
			a = b;
			a.swap(b);
			a.clear();

			TEST_ENSURE_OP(a.size(), ==, 0);
			TEST_ENSURE_OP(a.properties(), ==, 0);
			TEST_ENSURE(a.empty());

			b.freeMem();

			TEST_ENSURE_OP(b.size(), ==, 0);
			TEST_ENSURE_OP(b.properties(), ==, 0);
			TEST_ENSURE(b.empty());
		}

		void testPropertyCreate()
		{
			bool result = false;

			ParallelArray<integer> a;
			typedef PropertyId<integer, A> MyProperty;

			MyProperty aProperty(0);

			result = a.add(aProperty);

			TEST_ENSURE(result);

			MemoryView<A> data;
			result = a.get(aProperty, data);

			TEST_ENSURE(result);

			ConstMemoryView<A> readData;
			result = a.get(aProperty, readData);

			TEST_ENSURE(result);

			result = a.remove(aProperty);

			TEST_ENSURE(result);
		}

		void testResize()
		{
			ParallelArray<integer> a;

			const integer Size = 1043;

			a.resize(Size);

			TEST_ENSURE_OP(a.size(), ==, Size);
			TEST_ENSURE(!a.empty());

			a.clear();

			TEST_ENSURE_OP(a.size(), ==, 0);
			TEST_ENSURE(a.empty());

			a.resize(Size);

			TEST_ENSURE_OP(a.size(), ==, Size);
			TEST_ENSURE(!a.empty());

			a.resize(0);
			TEST_ENSURE_OP(a.size(), ==, 0);
			TEST_ENSURE(a.empty());
		}

		void testUse()
		{
			ParallelArray<integer> a;
			typedef PropertyId<integer, A> MyProperty;
			MyProperty aProperty(0);

			bool result = false;

			result = a.add(aProperty);

			TEST_ENSURE(result);

			const integer Size = 512;

			a.resize(Size);

			typedef PropertyId<integer, integer> IntegerProperty;

			IntegerProperty bProperty(1);

			result = a.add(bProperty);
			TEST_ENSURE(result);

			MemoryView<integer> intData;
			result = a.get(bProperty, intData);
			TEST_ENSURE(result);
			TEST_ENSURE_OP(intData.size(), ==, Size);

			for (integer i = 0;i < intData.size();++i)
			{
				intData[i] = i;
			}

			ConstMemoryView<integer> readIntData;
			result = a.get(bProperty, readIntData);
			TEST_ENSURE(result);
			TEST_ENSURE_OP(readIntData.size(), ==, Size);

			for (integer i = 0;i < readIntData.size();++i)
			{
				log() << readIntData[i] << ", ";
			}

			a.clear();
			TEST_ENSURE_OP(a.properties(), ==, 2);

			a.freeMem();
			TEST_ENSURE_OP(a.properties(), ==, 0);
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		sysTestList().add("ParallelArray", test);
	}

	CallFunction run(addTest);

}
