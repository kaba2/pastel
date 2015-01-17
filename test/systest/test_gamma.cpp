// Description: Testing for the gamma function
// DocumentationOf: gamma.h

#include "test_pastelsys.h"

#include <pastel/sys/math/gamma.h>
#include <pastel/sys/math/factorial.h>
#include <pastel/sys/math/error.h>
#include <boost/math/special_functions/gamma.hpp>

using namespace Pastel;

namespace
{

	class Test
		: public TestSuite
	{
	public:
		Test()
			: TestSuite(&testReport())
		{
		}

		virtual void run()
		{
			testInteger<real64>();
		}

		template <typename Real>
		void testInteger()
		{
			for (integer i = 1;i < 140;++i)
			{
				//std::cout << gamma<Real>(i) << " : " << std::exp(lnGamma<Real>(i)) << std::endl;
				TEST_ENSURE_OP(relativeError<Real>(gamma<Real>(i), std::exp(lnGamma<Real>(i))), <, 9e-13);
			}

			// 20! < 2^64 - 1 < 21!
			for (integer i = 0; i < 140; ++i)
			{
				//std::cout << factorial<Real>(i) << " : " << gamma<Real>(i + 1) << std::endl;
				TEST_ENSURE_OP(relativeError<Real>(factorial<Real>(i),  gamma<Real>(i + 1)), <, 5e-15);
			}
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("gamma", test);
	}

	CallFunction run(addTest);

}
