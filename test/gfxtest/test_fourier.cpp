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
		}

		template <int N>
		void testFourierCase(const real (&input)[N])
		{
			std::vector<std::complex<real> > output;

			dft(forwardRange(input),
				std::back_inserter(output));
			
			inverseDft(
				forwardRange(output.begin(), output.end()),
				output.begin());
			
			for (integer i = 0;i < N;++i)
			{
				TEST_ENSURE_OP(std::abs(input[i] - output[i]), <, 0.001);
			}

			unitaryDft(forwardRange(input),
				output.begin());
			
			inverseUnitaryDft(
				forwardRange(output.begin(), output.end()),
				output.begin());

			for (integer i = 0;i < N;++i)
			{
				TEST_ENSURE_OP(std::abs(input[i] - output[i]), <, 0.001);
			}

			//std::copy(output.begin(), output.end(),
			//	std::ostream_iterator<std::complex<real> >(std::cout, " "));
			//std::cout << std::endl;

		}

		template <int N>
		void testCosineCase(const real (&input)[N])
		{
			std::vector<real> output;

			orthogonalDct(forwardRange(input),
				std::back_inserter(output));
			
			//std::copy(output.begin(), output.end(),
			//	std::ostream_iterator<real>(std::cout, " "));
			//std::cout << std::endl;

			inverseOrthogonalDct(
				forwardRange(output.begin(), output.end()),
				output.begin());
			
			for (integer i = 0;i < N;++i)
			{
				TEST_ENSURE_OP(std::abs(input[i] - output[i]), <, 0.001);
			}

			dct(forwardRange(input),
				output.begin());
			
			//std::copy(output.begin(), output.end(),
			//	std::ostream_iterator<real>(std::cout, " "));
			//std::cout << std::endl;

			inverseDct(
				forwardRange(output.begin(), output.end()),
				output.begin());
			
			for (integer i = 0;i < N;++i)
			{
				TEST_ENSURE_OP(std::abs(input[i] - output[i]), <, 0.001);
			}

			//std::copy(output.begin(), output.end(),
			//	std::ostream_iterator<real>(std::cout, " "));
			//std::cout << std::endl;
		}

		void testSimple()
		{
			{
				const real input[] = {1};
				testFourierCase(input);
				testCosineCase(input);
			}
			{
				const real input[] = {1, 2};
				testFourierCase(input);
				testCosineCase(input);
			}
			{
				const real input[] = {1, 2, 3, 4};
				testFourierCase(input);
				testCosineCase(input);
			}
			{
				const real input[] = {4, 3, 2, 1};
				testFourierCase(input);
				testCosineCase(input);
			}
			{
				const real input[] = {1, 2, 3, 4, 5, 6, 7, 8};
				testFourierCase(input);
				testCosineCase(input);
			}
			{
				const real input[] = {1, 5, 2, 3, 4, 9, 5, 5};
				testFourierCase(input);
				testCosineCase(input);
			}
			{
				const real input[] = 
				{
					0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 8,
					0, 8, 0, 7, 0, 6, 0, 5, 0, 4, 0, 3, 0, 2, 0, 1
				};
				testFourierCase(input);
				testCosineCase(input);
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
