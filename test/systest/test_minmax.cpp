#include "pastelsystest.h"

#include "pastel/sys/minmax.h"

namespace
{

	using namespace Pastel;

	void testMinMax()
	{
		integer min = 0;
		integer max = 0;

		minMax(1, 2, 3, min, max);
		REPORT2(min != 1 || max != 3, min, max);

		minMax(1.0, 2, 3.0f, min, max);
		REPORT2(min != 1 || max != 3, min, max);

		minMax(1, 3, 2, min, max);
		REPORT2(min != 1 || max != 3, min, max);

		minMax(2, 1, 3, min, max);
		REPORT2(min != 1 || max != 3, min, max);

		minMax(2, 3, 1, min, max);
		REPORT2(min != 1 || max != 3, min, max);

		minMax(3, 1, 2, min, max);
		REPORT2(min != 1 || max != 3, min, max);

		minMax(3, 2, 1, min, max);
		REPORT2(min != 1 || max != 3, min, max);
	}

	void testBegin()
	{
		testMinMax();
	}

	void testAdd()
	{
		sysTestList().add("MinMax", testBegin);
	}

	CallFunction run(testAdd);

}
