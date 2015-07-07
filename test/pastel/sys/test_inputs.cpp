// Description: Testing for inputs
// Documentation: input.txt

#include "test/test_init.h"

#include "pastel/sys/input.h"

#include <vector>
#include <list>

TEST_CASE("Sequential (Input)")
{
	std::list<integer> aSet = { 0, 1, 2, 3 };
	auto input = rangeInput(aSet.begin(), aSet.end());
	REQUIRE(!input.empty());

	PASTEL_STATIC_ASSERT(
		(std::is_same<Input_Return<decltype(input)>, integer&>::value));
	PASTEL_STATIC_ASSERT(
		(std::is_same<Input_Value<decltype(input)>, integer>::value));

	// The length of the sequence is not known,
	// since the input range was constructed from two
	// iterators.
	REQUIRE(input.nHint() == 0);

	// Since the range is no random-access, this 
	// should not compile.
	//REQUIRE(input[0] == 0);
}

TEST_CASE("Indexed (Input)")
{
	std::vector<integer> aSet = { 0, 1, 2, 3 };
	auto input = rangeInput(aSet.begin(), aSet.end());
	REQUIRE(!input.empty());

	PASTEL_STATIC_ASSERT(
		(std::is_same<Input_Return<decltype(input)>, integer&>::value));
	PASTEL_STATIC_ASSERT(
		(std::is_same<Input_Value<decltype(input)>, integer>::value));

	// Since the range is random-access, so is the input.
	REQUIRE(input.n() == 4);
	REQUIRE(input[0] == 0);
	REQUIRE(input[1] == 1);
	REQUIRE(input[2] == 2);
	REQUIRE(input[3] == 3);
}

TEST_CASE("Scalar (Input)")
{
	{
		auto input = Scalar_Input<integer>();
		REQUIRE(input.n() == 1);
		REQUIRE(!input.empty());

		PASTEL_STATIC_ASSERT(
			(std::is_same<Input_Return<decltype(input)>, const integer&>::value));
		PASTEL_STATIC_ASSERT(
			(std::is_same<Input_Value<decltype(input)>, integer>::value));

		REQUIRE(input.get() == 0);
		REQUIRE(input[0] == 0);

		input.pop();
		REQUIRE(input.empty());
	}

	integer n = 10;
	{
		auto input = scalarInput(5, n);
		for (integer i = 0; i < n; ++i)
		{
			REQUIRE(!input.empty());
			REQUIRE(input.n() == n);

			integer j = input[i];
			REQUIRE(j == 5);
		}
	}

	{
		auto input = scalarInput(5, n);
		for (integer i = 0; i < n; ++i)
		{
			REQUIRE(!input.empty());
			REQUIRE(input.n() == n - i);

			integer j = input.get();
			REQUIRE(j == 5);

			input.pop();
		}
		REQUIRE(input.empty());
	}

	{
		auto input = scalarInput(5);
		REQUIRE(!input.empty());
		REQUIRE(input.n() == 1);

		integer j = input.get();
		REQUIRE(j == 5);

		input.pop();
		REQUIRE(input.empty());
	}
}

TEST_CASE("various (Input)")
{
	{
		auto input = countingInput();
		for (integer i = 0; i < 50;++i)
		{
			REQUIRE(!input.empty());

			integer j = input.get();
			input.pop();
			REQUIRE(j == i);
		}
	}

	{
		auto input = countingInput(-50);
		for (integer i = -50; i < 50;++i)
		{
			REQUIRE(!input.empty());

			integer j = input.get();
			input.pop();
			REQUIRE(j == i);
		}
	}

	{
		auto input = transformInput(
			countingInput(-50),
			[&](integer i)
		{
			return i * i;
		});

		for (integer i = -50; i < 50; ++i)
		{
			REQUIRE(!input.empty());

			integer j = input.get();
			input.pop();
			REQUIRE(j == i * i);
		}
	}
}

