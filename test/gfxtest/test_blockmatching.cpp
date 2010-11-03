#include "pastelgfxtest.h"

#include <pastel/device/timer.h>

#include <pastel/math/euclidean_normbijection.h>

#include <pastel/gfx/matchblock_bruteforce.h>
#include <pastel/gfx/savepcx.h>

#include <fstream>

using namespace Pastel;

namespace
{

	class BlockMatching_Test
		: public TestSuite
	{
	public:
		BlockMatching_Test()
			: TestSuite(&gfxTestReport())
		{
		}

		virtual void run()
		{
			testBruteForce();
		}

		void testBruteForce()
		{
			const Array<real32>& image = 
				*gfxStorage().get<Array<real32>*>("lena_gray");
			
			Timer timer;

			timer.setStart();

			Array<integer> nearestSet = matchBlockBrute(
				image, Vector2i(8, 8), 64, 
				infinity<real32>(), Euclidean_NormBijection<real32>());

			timer.store();

			std::cout << "Computation took " << 
				timer.seconds() << " seconds." << std::endl;

			std::ofstream file("lena_64_nearest_8x8.txt");
			for (integer i = 0;i < nearestSet.height();++i)
			{
				for (integer j = 0;j < nearestSet.width();++j)
				{
					file << nearestSet(j, i) << " ";
				}
				file << std::endl;
			}

			saveGrayscalePcx(image, "muu.pcx");
		}

	};

	void testBlockMatching()
	{
		BlockMatching_Test test;
		test.run();
	}

	void addTest()
	{
		gfxTestList().add("BlockMatching", testBlockMatching);
	}

	CallFunction run(addTest);

}
