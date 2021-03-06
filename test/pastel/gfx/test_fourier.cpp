// Description: Testing for signal transforms
// DocumentationOf: fourier_transform.txt

#include "test/test_init.h"

#include "pastel/gfx/image_file.h"
#include "pastel/gfx/color.h"
#include "pastel/gfx/image_processing.h"
#include "pastel/gfx/transform.h"

#include "pastel/sys/view.h"
#include "pastel/sys/subarray_for_each.h"

#include <iostream>
#include <string>

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

	template <typename Complex_RandomAccessRange>
	bool testDft(
		const Complex_RandomAccessRange& input)
	{
		integer n = input.size();

		std::vector<std::complex<dreal> > output(n);
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
	bool testDft(const dreal (&input)[N])
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

		std::vector<dreal> output(input.begin(), input.end());
		transform(
			range(output.begin(), output.end()));

		/*
		std::cout << "Input: " << std::endl;
		std::copy(
			input.begin(), input.end(),
			std::ostream_iterator<dreal>(std::cout, " "));
		std::cout << std::endl;

		std::cout << "Output: " << std::endl;
		std::copy(
			output.begin(), output.end(),
			std::ostream_iterator<dreal>(std::cout, " "));
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
	bool testDct(const dreal (&input)[N])
	{
		return testDct(range(input));
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
	bool testHaar(const dreal (&input)[N])
	{
		return testHaar(range(input));
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

	template <int N>
	bool testHadamard(const dreal (&input)[N])
	{
		return testHadamard(range(input));
	}

}

TEST_CASE("Random (Fourier)")
{
	for (integer i = 0;i < 100;++i)
	{
		integer n = (integer)1 << randomInteger(9);

		std::vector<dreal> input;
		input.reserve(n);
		for (integer j = 0;j < n;++j)
		{
			input.push_back(random<dreal>(-1, 1));
		}

		testDct(range(input.begin(), input.end()));
		testDft(range(input.begin(), input.end()));
	}			
}

TEST_CASE("Simple (Fourier)")
{
	{
		dreal input[] = {1};
		REQUIRE(testDft(input));
		REQUIRE(testDct(input));
		REQUIRE(testHaar(input));
		REQUIRE(testHadamard(input));
	}
	{
		dreal input[] = {1, 2};
		REQUIRE(testDft(input));
		REQUIRE(testDct(input));
		REQUIRE(testHaar(input));
		REQUIRE(testHadamard(input));
	}
	{
		dreal input[] = {1, 2, 3, 4};
		REQUIRE(testDft(input));
		REQUIRE(testDct(input));
		REQUIRE(testHaar(input));
		REQUIRE(testHadamard(input));
	}
	{
		dreal input[] = {4, 3, 2, 1};
		REQUIRE(testDft(input));
		REQUIRE(testDct(input));
		REQUIRE(testHaar(input));
		REQUIRE(testHadamard(input));
	}
	{
		dreal input[] = {1, 2, 3, 4, 5, 6, 7, 8};
		REQUIRE(testDft(input));
		REQUIRE(testDct(input));
		REQUIRE(testHaar(input));
		REQUIRE(testHadamard(input));
	}
	{
		dreal input[] = {1, 5, 2, 3, 4, 9, 5, 5};
		REQUIRE(testDft(input));
		REQUIRE(testDct(input));
		REQUIRE(testHaar(input));
		REQUIRE(testHadamard(input));
	}
	{
		dreal input[] = {2, 5, 8, 9, 7, 4, -1, 1};
		REQUIRE(testDft(input));
		REQUIRE(testDct(input));
		REQUIRE(testHaar(input));
		REQUIRE(testHadamard(input));
	}
	{
		dreal input[] = 
		{
			0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 8,
			0, 8, 0, 7, 0, 6, 0, 5, 0, 4, 0, 3, 0, 2, 0, 1
		};
		REQUIRE(testDft(input));
		REQUIRE(testDct(input));
		REQUIRE(testHaar(input));
		REQUIRE(testHadamard(input));
	}
}
