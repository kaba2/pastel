// Description: Testing for new range types
// DocumentationOf: range_concept.h

#include "pastelsystest.h"

#include "pastel/sys/ranges.h"
#include "pastel/sys/random.h"

#include <iostream>

using namespace Pastel;

namespace
{

	template <
		typename In_ConstForwardRange, 
		typename Out_ForwardRange>
	void rangeCopy(
		In_ConstForwardRange inRange, 
		Out_ForwardRange outRange)
	{
		while(!inRange.empty() && !outRange.empty())
		{
			outRange.front() = inRange.front();
			inRange.pop_front();
			outRange.pop_front();
		}
	}

	template <
		typename Left_ConstForwardRange, 
		typename Right_ConstForwardRange,
		typename Predicate>
	bool rangeEqual(
		Left_ConstForwardRange leftRange, 
		Right_ConstForwardRange rightRange,
		Predicate predicate)
	{
		while(!leftRange.empty() && !rightRange.empty())
		{
			if (!predicate(leftRange.front(), rightRange.front()))
			{
				return false;
			}
			leftRange.pop_front();
			rightRange.pop_front();
		}

		return true;
	}

	template <
		typename Left_ConstForwardRange, 
		typename Right_ConstForwardRange>
	bool rangeEqual(
		Left_ConstForwardRange leftRange, 
		Right_ConstForwardRange rightRange)
	{
		return rangeEqual(leftRange, rightRange, EqualTo());
	}

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
			test();
		}

		void test()
		{
			rangeCopy(
				constantRange2(1, 10),
				streamRange<integer>(std::cout));

			integer iSet[] = 
			{
				1, 6, 3, -4
			};

			rangeCopy(
				arrayRange(iSet),
				streamRange<integer>(std::cout));

			rangeCopy(
				generatedRange<real>(randomOpen<real>, 10),
				streamRange<real>(std::cout));
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTests()
	{
		sysTestList().add("Range_New", test);
	}

	CallFunction run(addTests);

}
