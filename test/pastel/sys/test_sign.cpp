// Description: Testing for sign
// DocumentationOf: sign.h

#include "test/test_init.h"

#include "pastel/sys/math/sign.h"

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
			REQUIRE(sign(-100) == -1);
			REQUIRE(sign(100) == 1);

			REQUIRE(sign(-1) == -1);
			REQUIRE(sign(0) == 0);
			REQUIRE(sign(1) == 1);

			REQUIRE(sign(-1.0) == -1);
			REQUIRE(sign(0.0) == 0);
			REQUIRE(sign(1.0) == 1);

			REQUIRE(sign(-1.0f) == -1);
			REQUIRE(sign(0.0f) == 0);
			REQUIRE(sign(1.0f) == 1);

			REQUIRE(sign(infinity<float>()) == 1);
			REQUIRE(sign(-infinity<float>()) == -1);
			REQUIRE(sign(nan<float>()) == 0);
			REQUIRE(sign(-0.0f) == 0);
		}
	};

}
