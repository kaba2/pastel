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
			testRandom();
		}

		template <typename Complex_Iterator>
		void testDft(
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
				TEST_ENSURE_OP(std::abs(input[i] - output[i]), <, 0.001);
			}

			unitaryDft(forwardRange(input.begin(), input.end()),
				output.begin());
			inverseUnitaryDft(
				forwardRange(output.begin(), output.end()),
				output.begin());

			for (integer i = 0;i < n;++i)
			{
				TEST_ENSURE_OP(std::abs(input[i] - output[i]), <, 0.001);
			}
		}

		template <int N>
		void testDft(const real (&input)[N])
		{
			testDft(randomAccessRange(input));
		}

		template <typename Complex_Iterator>
		void testDct(
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
				TEST_ENSURE_OP(std::abs(input[i] - output[i]), <, 0.001);
			}

			orthogonalDct(forwardRange(input.begin(), input.end()),
				output.begin());
			inverseOrthogonalDct(
				forwardRange(output.begin(), output.end()),
				output.begin());

			for (integer i = 0;i < n;++i)
			{
				TEST_ENSURE_OP(std::abs(input[i] - output[i]), <, 0.001);
			}
		}

		template <int N>
		void testDct(const real (&input)[N])
		{
			testDct(randomAccessRange(input));
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
				testDft(input);
				testDct(input);
			}
			{
				const real input[] = {1, 2};
				testDft(input);
				testDct(input);
			}
			{
				const real input[] = {1, 2, 3, 4};
				testDft(input);
				testDct(input);
			}
			{
				const real input[] = {4, 3, 2, 1};
				testDft(input);
				testDct(input);
			}
			{
				const real input[] = {1, 2, 3, 4, 5, 6, 7, 8};
				testDft(input);
				testDct(input);
			}
			{
				const real input[] = {1, 5, 2, 3, 4, 9, 5, 5};
				testDft(input);
				testDct(input);
			}
			{
				const real input[] = 
				{
					0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 8,
					0, 8, 0, 7, 0, 6, 0, 5, 0, 4, 0, 3, 0, 2, 0, 1
				};
				testDft(input);
				testDct(input);
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
