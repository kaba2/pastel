// Description: Testing template
// Documentation: unit_testing.txt

#include "test/test_init.h"

namespace
{

	class Test
	{
	public:
		virtual void run()
		{
			test();
		}

		void test()
		{
		}
	};

	TEST_CASE("None", "[None]")
	{
	}

}
