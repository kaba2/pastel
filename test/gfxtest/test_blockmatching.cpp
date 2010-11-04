#include "pastelgfxtest.h"

#include <pastel/device/timer.h>

#include <pastel/math/euclidean_normbijection.h>

#include <pastel/dsp/discretecosine.h>

#include <pastel/gfx/matchblock_bruteforce.h>
#include <pastel/gfx/savepcx.h>

#include <pastel/sys/subarray_tools.h>

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
			//testSimple();
			testBruteForce();
		}

		void testSimple()
		{
			Array<real> image(512, 512, 0.5);
			Vector<integer, 2> blockExtent(8, 8);

			SubArray<real> region =
				image(Vector<integer, 2>(ofDimension(image.dimension()), 0), 
				image.extent() - blockExtent);

			SubArray<real>::Iterator iter = region.begin();
			for (integer i = 0;i < region.size();++i)
			{
				iter = region.begin(i);
				*iter = (real)i / (region.size() - 1);
				++iter;				
			}

			SubArray<real>::ConstIterator constIter = region.begin();
			ENSURE(std::equal(region.begin(), region.end(), constIter));

			saveGrayscalePcx(image, "subarray.pcx");
		}

		void testApproximate()
		{
			const Array<real32>& image = 
				*gfxStorage().get<Array<real32>*>("lena_gray");
			
			Timer timer;

			timer.setStart();

			// Compute feature vectors

			Array<integer> nearestSet = matchBlockBrute(
				image, Vector2i(8, 8), 64, 
				infinity<real32>(), Euclidean_NormBijection<real32>());

			timer.store();

			std::cout << "Computation took " << 
				timer.seconds() << " seconds." << std::endl;

			std::ofstream file("lena-64-nearest-8x8-approx.txt");
			for (integer i = 0;i < nearestSet.height();++i)
			{
				for (integer j = 0;j < nearestSet.width();++j)
				{
					file << nearestSet(j, i) << " ";
				}
				file << std::endl;
			}
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

			std::ofstream file("lena-64-nearest-8x8-brute.txt");
			for (integer i = 0;i < nearestSet.height();++i)
			{
				for (integer j = 0;j < nearestSet.width();++j)
				{
					file << nearestSet(j, i) << " ";
				}
				file << std::endl;
			}

			saveGrayscalePcx(image, "lena-64-nearest.pcx");
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
