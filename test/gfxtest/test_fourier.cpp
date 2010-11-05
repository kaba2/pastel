#include "pastelgfxtest.h"

#include "pastel/gfx/loadpcx.h"
#include "pastel/gfx/savepcx.h"
#include "pastel/gfx/coloradapter.h"
#include "pastel/gfx/color_tools.h"
#include "pastel/gfx/packrange.h"

#include "pastel/dsp/discretefourier.h"
#include "pastel/dsp/discretecosine.h"

#include "pastel/sys/arrayview.h"
#include "pastel/sys/view_tools.h"
#include "pastel/sys/adaptedview.h"
#include "pastel/sys/subarray_tools.h"

#include <iostream>
#include <string>

using namespace Pastel;

namespace
{

	class Complex_Real_Adapter
	{
	public:
		typedef real32 Logical;
		typedef std::complex<real32> Physical;

		Logical convert(const Physical& physical) const
		{
			return physical.real();
		}

		Physical revert(const Logical& logical) const
		{
			return Physical(logical, 0);
		}
	};

	typedef ReverseAdapter<Complex_Real_Adapter> Real_Complex_Adapter;

	class Complex_Mod_Adapter
	{
	public:
		typedef real32 Logical;
		typedef std::complex<real32> Physical;

		Logical convert(const Physical& physical) const
		{
			return std::abs(physical);
		}

		Physical revert(const Logical& logical) const
		{
			return Physical(logical, 0);
		}
	};

	class Fourier_Test
		: public TestSuite
	{
	public:
		Fourier_Test()
			: TestSuite(&gfxTestReport())
		{
		}

		virtual void run()
		{
			testSimple();
			testRandom();
			testImage();
		}

		void testImage()
		{
			const Array<real32>& image = 
				*gfxStorage().get<Array<real32>*>("lena_gray");
			
			Array<real32> dctImage(image);

			forEachRowOnAllAxes(dctImage(), Dct());

			for (integer y = 0;y < dctImage.height();++y)
			{
				for (integer x = 0;x < dctImage.width();++x)
				{
					dctImage(x, y) *= std::exp(-(real32)(x * x + y * y) / 4000);
				}
			}

			forEachRowOnAllAxes(dctImage(), InverseDct());

			//for (integer i = 0;i < dctImage.size();++i)
			//{
			//	dctImage(i) = std::log(std::abs(dctImage(i)) + 1);
			//}

			saveGrayscalePcx(dctImage, "dct_lena.pcx", true);
		}

		template <typename Complex_Iterator>
		bool testDft(
			const RandomAccessRange<Complex_Iterator>& input)
		{
			const integer n = input.size();

			std::vector<std::complex<real> > output(n);
			dft(forwardRange(input.begin(), input.end()),
				output.begin());
			inverseDft(
				forwardRange(output.begin(), output.end()),
				output.begin());

			for (integer i = 0;i < n;++i)
			{
				if (std::abs(input[i] - output[i]) > 0.001)
				{
					return false;
				}
			}

			unitaryDft(forwardRange(input.begin(), input.end()),
				output.begin());
			inverseUnitaryDft(
				forwardRange(output.begin(), output.end()),
				output.begin());

			for (integer i = 0;i < n;++i)
			{
				if (std::abs(input[i] - output[i]) > 0.001)
				{
					return false;
				}
			}

			return true;
		}

		template <int N>
		bool testDft(const real (&input)[N])
		{
			return testDft(randomAccessRange(input));
		}

		template <typename Complex_Iterator>
		bool testDct(
			const RandomAccessRange<Complex_Iterator>& input)
		{
			const integer n = input.size();

			std::vector<real> output(n);
			dct(forwardRange(input.begin(), input.end()),
				output.begin());
			inverseDct(
				forwardRange(output.begin(), output.end()),
				output.begin());

			for (integer i = 0;i < n;++i)
			{
				if (std::abs(input[i] - output[i]) > 0.001)
				{
					return false;
				}
			}

			orthogonalDct(forwardRange(input.begin(), input.end()),
				output.begin());
			inverseOrthogonalDct(
				forwardRange(output.begin(), output.end()),
				output.begin());

			for (integer i = 0;i < n;++i)
			{
				if (std::abs(input[i] - output[i]) > 0.001)
				{
					return false;
				}
			}

			//std::copy(output.begin(), output.end(),
			//	std::ostream_iterator<real>(std::cout, " "));
			//std::cout << std::endl;

			return true;
		}

		template <int N>
		bool testDct(const real (&input)[N])
		{
			return testDct(randomAccessRange(input));
		}

		void testRandom()
		{
			for (integer i = 0;i < 100;++i)
			{
				const integer n = (1 << (randomInteger() % 9));

				std::vector<real> input;
				input.reserve(n);
				for (integer j = 0;j < n;++j)
				{
					input.push_back(random<real>(-1, 1));
				}

				testDct(randomAccessRange(input.begin(), input.end()));
				testDft(randomAccessRange(input.begin(), input.end()));
			}			
		}

		void testSimple()
		{
			{
				const real input[] = {1};
				TEST_ENSURE(testDft(input));
				TEST_ENSURE(testDct(input));
			}
			{
				const real input[] = {1, 2};
				TEST_ENSURE(testDft(input));
				TEST_ENSURE(testDct(input));
			}
			{
				const real input[] = {1, 2, 3, 4};
				TEST_ENSURE(testDft(input));
				TEST_ENSURE(testDct(input));
			}
			{
				const real input[] = {4, 3, 2, 1};
				TEST_ENSURE(testDft(input));
				TEST_ENSURE(testDct(input));
			}
			{
				const real input[] = {1, 2, 3, 4, 5, 6, 7, 8};
				TEST_ENSURE(testDft(input));
				TEST_ENSURE(testDct(input));
			}
			{
				const real input[] = {1, 5, 2, 3, 4, 9, 5, 5};
				TEST_ENSURE(testDft(input));
				TEST_ENSURE(testDct(input));
			}
			{
				const real input[] = 
				{
					0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 8,
					0, 8, 0, 7, 0, 6, 0, 5, 0, 4, 0, 3, 0, 2, 0, 1
				};
				TEST_ENSURE(testDft(input));
				TEST_ENSURE(testDct(input));
			}
		}
	};

	void testFourier()
	{
		Fourier_Test test;
		test.run();
	}

	void addTest()
	{
		gfxTestList().add("Fourier", testFourier);
	}

	CallFunction run(addTest);

}
