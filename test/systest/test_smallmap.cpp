#include "pastelsystest.h"

#include "pastel/sys/smallmap.h"
#include "pastel/sys/log.h"

using namespace Pastel;

namespace
{

	void print(const SmallMap<int, int>& that)
	{
		log() << "Contents: ";
		for (integer i = 0;i < that.size();++i)
		{
			log() << "(" << that.keyAt(i) << ", "
				<< that.valueAt(i) << "), ";
		}
		log() << "end." << logNewLine;
		log() << "Size: " << that.size() << logNewLine;
		log() << "Capacity: " << that.capacity() << logNewLine;
	}

	void testOne()
	{
		SmallMap<int, int> a;

		{
			a.insert(5, 0);
			print(a);
		}
	}

	void testTwoBefore()
	{
		SmallMap<int, int> a;

		{
			a.insert(5, 5);
			print(a);
		}
		{
			a.insert(3, 16);
			print(a);
		}
	}

	void testTwoAfter()
	{
		SmallMap<int, int> a;

		{
			a.insert(5, 54);
			print(a);
		}
		{
			a.insert(7, 31);
			print(a);
		}
	}

	void testTwoSame()
	{
		SmallMap<int, int> a;

		{
			a.insert(5, 13);
			print(a);
		}
		{
			a.insert(5, 14);
			print(a);
		}
	}

	void testThreeMiddle()
	{
		SmallMap<int, int> a;

		{
			a.insert(5, 53);
			print(a);
		}
		{
			a.insert(7, 49);
			print(a);
		}
		{
			a.insert(6, 3);
			print(a);
		}
	}

	void testHundredForward()
	{
		SmallMap<int, int> a;

		for (int i = 0;i < 100;++i)
		{
			a.insert(i, 99 - i);
		}
		print(a);
	}

	void testHundredBackward()
	{
		SmallMap<int, int> a;

		for (int i = 0;i < 100;++i)
		{
			a.insert(99 - i, i);
		}
		print(a);
	}

	void testOneErase()
	{
		SmallMap<int, int> a;

		{
			a.insert(5, 14);
			print(a);
		}

		{
			a.erase(5);
			print(a);
		}
	}

	void testTwoEraseBefore()
	{
		SmallMap<int, int> a;

		{
			a.insert(5, 77);
			print(a);
		}

		{
			a.insert(7, 99);
			print(a);
		}

		{
			a.erase(5);
			print(a);
		}
	}

	void testTwoEraseAfter()
	{
		SmallMap<int, int> a;

		{
			a.insert(5, 4);
			print(a);
		}

		{
			a.insert(7, 4);
			print(a);
		}

		{
			a.erase(7);
			print(a);
		}
	}

	void testThreeEraseMiddle()
	{
		SmallMap<int, int> a;

		{
			a.insert(5, 64);
			print(a);
		}

		{
			a.insert(7, 59);
			print(a);
		}

		{
			a.insert(6, 59);
			print(a);
		}

		{
			a.erase(6);
			print(a);
		}
	}

	void testEraseNotFound()
	{
		SmallMap<int, int> a;

		{
			a.erase(5);
			print(a);
		}

		{
			a.insert(5, 34);
			print(a);
		}

		{
			a.erase(7);
			print(a);
		}
	}

	void testBegin()
	{
		testOne();
		testTwoBefore();
		testTwoAfter();
		testTwoSame();
		testThreeMiddle();
		testHundredForward();
		testHundredBackward();
		testOneErase();
		testTwoEraseBefore();
		testTwoEraseAfter();
		testThreeEraseMiddle();
		testEraseNotFound();
	}

	void testAdd()
	{
		sysTestList().add("SmallMap", testBegin);
	}

	CallFunction run(testAdd);

}
