// Description: Testing for PiecewiseLinear
// DocumentationOf: piecewiselinear.h

#include "pastelmathtest.h"

#include "pastel/math/piecewiselinear.h"

#include "pastel/gfx/pcx.h"

#include "pastel/sys/array.h"

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
			const integer Width = 500;
			const integer Height = 100;
			Array<bool, 2> image(Width, Height, false);
			
			PiecewiseLinear<real> f;

			f.insert(0.0, 0.0);
			f.insert(0.1, 1.0);
			f.insert(0.2, 0.0);
			f.insert(0.3, 0.5);
			f.insert(0.4, 0.5);
			f.insert(0.5, 0.5);
			f.insert(0.6, 1.0);
			f.insert(0.7, 0.9);
			f.insert(0.8, 0.8);
			f.insert(0.9, 0.7);
			f.insert(1.0, 1.0);

			for (integer x = 0;x < Width;++x)
			{
				const real t = (real)x / (Width - 1);
				
				image(x, quantizeUnsigned(f(t), Height)) = true;
			}

			saveBinaryPcx(image, "piecewiselinear.pcx");
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("PiecewiseLinear", test);
	}

	CallFunction run(addTest);

}
