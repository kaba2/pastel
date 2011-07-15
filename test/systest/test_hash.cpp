// Description: Testing for hash functions
// DocumentationOf: hash.h

#include "pastelsystest.h"

#include "pastel/sys/array.h"
#include "pastel/sys/countingiterator.h"
#include "pastel/sys/hash.h"

#include "pastel/gfx/pcx.h"

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
			testTrivial();
		}

		void testTrivial()
		{
			Array<real32> image(512, 512);
			for (integer y = 0;y < image.height();++y)
			{
				for (integer x = 0;x < image.width();++x)
				{
					const integer h = computeHash(Vector2i(x, y));
					image(x, y) = (real32)(mod(h, 256)) / 255;
				}
			}

			saveGrayscalePcx(image, "hash.pcx");
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("Hash", test);
	}

	CallFunction run(addTest);

}
