// Description: Testing for signal transforms
// DocumentationOf: signal_transforms.h

#include "test_pastelgfx.h"

#include "pastel/gfx/loadpcx.h"
#include "pastel/gfx/savepcx.h"
#include "pastel/gfx/coloradapter.h"
#include "pastel/gfx/color_tools.h"
#include "pastel/gfx/packrange.h"

#include "pastel/gfx/signal_transforms.h"

#include "pastel/sys/arrayview.h"
#include "pastel/sys/view_tools.h"
#include "pastel/sys/adaptedview.h"
#include "pastel/sys/subarray_for_each.h"

#include <iostream>
#include <string>

using namespace Pastel;

namespace
{

	class Complex_Real_Adapter
	{
	public:
		using Logical = real32;
		using Physical = std::complex<real32>;

		Logical convert(const Physical& physical) const
		{
			return physical.real();
		}

		Physical revert(const Logical& logical) const
		{
			return Physical(logical, 0);
		}
	};

	using Real_Complex_Adapter = ReverseAdapter<Complex_Real_Adapter>;

	class Complex_Mod_Adapter
	{
	public:
		using Logical = real32;
		using Physical = std::complex<real32>;

		Logical convert(const Physical& physical) const
		{
			return std::abs(physical);
		}

		Physical revert(const Logical& logical) const
		{
			return Physical(logical, 0);
		}
	};

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
			testSimple();
			testRandom();
		}
	
		template <typename Complex_RandomAccessRange>
		bool testDft(
			const Complex_RandomAccessRange& input)
		{
			integer n = input.size();

			std::vector<std::complex<real> > output(n);
			dft(range(input.begin(), input.end()),
				range(output.begin(), output.end()));
			inverseDft(
				range(output.begin(), output.end()),
				range(output.begin(), output.end()));

			for (integer i = 0;i < n;++i)
			{
				if (std::abs(input[i] - output[i]) > 0.001)
				{
					return false;
				}
			}

			unitaryDft(
				range(input.begin(), input.end()),
				range(output.begin(), output.end()));
			inverseUnitaryDft(
				range(output.begin(), output.end()),
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

		bool testDft(const real (&input)[N])
		{
			return testDft(range(input));
		}

		template <
			typename Complex_RandomAccessRange,
			typename Transform_Algorithm,
			typename InverseTransform_Algorithm>
		bool test(
			const Complex_RandomAccessRange& input,
			const Transform_Algorithm& transform,
			const InverseTransform_Algorithm& inverse)
		{
			integer n = input.size();

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

		template <typename Complex_RandomAccessRange>
		bool testDct(
			const Complex_RandomAccessRange& input)
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

		template <typename Complex_RandomAccessRange>
		bool testHaar(
			const Complex_RandomAccessRange& input)
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

		template <typename Complex_RandomAccessRange>
		bool testHadamard(
			const Complex_RandomAccessRange& input)
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
				integer n = (1 << randomInteger(9));

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
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("Fourier", testFourier);
	}

	CallFunction run(addTest);

}
