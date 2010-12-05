// Description: Testing for Fourier transform
// DocumentationOf: fourier_transform.h

#include "pasteldsptest.h"

#include "pastel/gfx/loadpcx.h"
#include "pastel/gfx/savepcx.h"
#include "pastel/gfx/coloradapter.h"
#include "pastel/gfx/color_tools.h"
#include "pastel/gfx/packrange.h"

#include "pastel/dsp/transforms.h"

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
			: TestSuite(&dspTestReport())
		{
		}

		virtual void run()
		{
			testSimple();
			testRandom();
			testImage(Haar(), InverseHaar(), "haar");
			testImage(Dct(), InverseDct(), "dct");
		}

		template <
			typename Transform_Algorithm,
			typename Inverse_Algorithm>
		void testImage(
			const Transform_Algorithm& transform,
			const Inverse_Algorithm& inverse,
			const std::string& name)
		{
			const Array<real32>& image = 
				*dspStorage().get<Array<real32>*>("lena_gray");
			
			Array<real32> tImage(image);

			forEachRowOnAllAxes(tImage(), transform);

			{
				Array<real32> sImage(tImage);

				for (integer i = 0;i < tImage.size();++i)
				{
					sImage(i) = std::log(std::abs(tImage(i)) + 1);
				}

				saveGrayscalePcx(sImage, name + "_lena.pcx", true);
			}

			for (integer y = 0;y < tImage.height();++y)
			{
				for (integer x = 0;x < tImage.width();++x)
				{
					tImage(x, y) *= std::exp(-(real32)(x * x + y * y) / 4000);
				}
			}

			forEachRowOnAllAxes(tImage(), inverse);

			saveGrayscalePcx(tImage, name + "_lena_blur.pcx", true);
		}

		template <typename Complex_Iterator>
		bool testDft(
			const RandomAccessRange<Complex_Iterator>& input)
		{
			const integer n = input.size();

			std::vector<std::complex<real> > output(n);
			dft(range(input.begin(), input.end()),
				output.begin());
			inverseDft(
				range(output.begin(), output.end()),
				output.begin());

			for (integer i = 0;i < n;++i)
			{
				if (std::abs(input[i] - output[i]) > 0.001)
				{
					return false;
				}
			}

			unitaryDft(range(input.begin(), input.end()),
				output.begin());
			inverseUnitaryDft(
				range(output.begin(), output.end()),
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
			return testDft(range(input));
		}

		template <
			typename Complex_Iterator,
			typename Transform_Algorithm,
			typename InverseTransform_Algorithm>
		bool test(
			const RandomAccessRange<Complex_Iterator>& input,
			const Transform_Algorithm& transform,
			const InverseTransform_Algorithm& inverse)
		{
			const integer n = input.size();

			std::vector<real> output(input.begin(), input.end());
			transform(
				range(output.begin(), output.end()));

			/*
			std::cout << "Input: " << std::endl;
			std::copy(
				input.begin(), input.end(),
				std::ostream_iterator<real>(std::cout, " "));
			std::cout << std::endl;

			std::cout << "Output: " << std::endl;
			std::copy(
				output.begin(), output.end(),
				std::ostream_iterator<real>(std::cout, " "));
			std::cout << std::endl;
			*/

			inverse(
				range(output.begin(), output.end()));

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
		bool testDct(const real (&input)[N])
		{
			return testDct(range(input));
		}

		template <typename Complex_Iterator>
		bool testDct(
			const RandomAccessRange<Complex_Iterator>& input)
		{
			if (!test(input, Dct(), InverseDct()) ||
				!test(input, OrthogonalDct(), InverseOrthogonalDct()))
			{
				return false;
			}

			return true;
		}

		template <int N>
		bool testHaar(const real (&input)[N])
		{
			return testHaar(range(input));
		}

		template <typename Complex_Iterator>
		bool testHaar(
			const RandomAccessRange<Complex_Iterator>& input)
		{
			if (!test(input, Haar(), InverseHaar()) ||
				!test(input, OrthogonalHaar(), InverseOrthogonalHaar()))
			{
				return false;
			}

			return true;
		}

		template <int N>
		bool testHadamard(const real (&input)[N])
		{
			return testHadamard(range(input));
		}

		template <typename Complex_Iterator>
		bool testHadamard(
			const RandomAccessRange<Complex_Iterator>& input)
		{
			if (!test(input, Hadamard(), InverseHadamard()) ||
				!test(input, OrthogonalHadamard(), InverseOrthogonalHadamard()))
			{
				return false;
			}

			return true;
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

				testDct(range(input.begin(), input.end()));
				testDft(range(input.begin(), input.end()));
			}			
		}

		void testSimple()
		{
			{
				const real input[] = {1};
				TEST_ENSURE(testDft(input));
				TEST_ENSURE(testDct(input));
				TEST_ENSURE(testHaar(input));
				TEST_ENSURE(testHadamard(input));
			}
			{
				const real input[] = {1, 2};
				TEST_ENSURE(testDft(input));
				TEST_ENSURE(testDct(input));
				TEST_ENSURE(testHaar(input));
				TEST_ENSURE(testHadamard(input));
			}
			{
				const real input[] = {1, 2, 3, 4};
				TEST_ENSURE(testDft(input));
				TEST_ENSURE(testDct(input));
				TEST_ENSURE(testHaar(input));
				TEST_ENSURE(testHadamard(input));
			}
			{
				const real input[] = {4, 3, 2, 1};
				TEST_ENSURE(testDft(input));
				TEST_ENSURE(testDct(input));
				TEST_ENSURE(testHaar(input));
				TEST_ENSURE(testHadamard(input));
			}
			{
				const real input[] = {1, 2, 3, 4, 5, 6, 7, 8};
				TEST_ENSURE(testDft(input));
				TEST_ENSURE(testDct(input));
				TEST_ENSURE(testHaar(input));
				TEST_ENSURE(testHadamard(input));
			}
			{
				const real input[] = {1, 5, 2, 3, 4, 9, 5, 5};
				TEST_ENSURE(testDft(input));
				TEST_ENSURE(testDct(input));
				TEST_ENSURE(testHaar(input));
				TEST_ENSURE(testHadamard(input));
			}
			{
				const real input[] = {2, 5, 8, 9, 7, 4, -1, 1};
				TEST_ENSURE(testDft(input));
				TEST_ENSURE(testDct(input));
				TEST_ENSURE(testHaar(input));
				TEST_ENSURE(testHadamard(input));
			}
			{
				const real input[] = 
				{
					0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 8,
					0, 8, 0, 7, 0, 6, 0, 5, 0, 4, 0, 3, 0, 2, 0, 1
				};
				TEST_ENSURE(testDft(input));
				TEST_ENSURE(testDct(input));
				TEST_ENSURE(testHaar(input));
				TEST_ENSURE(testHadamard(input));
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
		dspTestList().add("Fourier", testFourier);
	}

	CallFunction run(addTest);

}
