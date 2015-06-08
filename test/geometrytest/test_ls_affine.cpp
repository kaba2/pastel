// Description: Testing for least-squares transformatins
// DocumentationOf: ls_affine.h

#include "test_pastelgeometry.h"

#include <pastel/geometry/pattern_matching/ls_affine.h>

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
			test();
		}

		void test()
		{
			arma::Mat<real> fromSet = arma::eye(10, 10);
			arma::Mat<real> toSet = arma::eye(10, 10);
			auto result = lsAffine(fromSet, toSet);
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("lsAffine", test);
	}

	CallFunction run(addTest);

}
