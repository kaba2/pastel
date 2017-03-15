// Description: Testing for ranked set.
// Documentation: unit_testing.txt

#include "test/test_init.h"
#include "pastel/sys/rankedset/rankedset.h"

TEST_CASE("Simple (RankedSet)")
{
	RankedSet<integer> aSet(3);
	REQUIRE(aSet.size() == 0);
	REQUIRE(aSet.empty());
	
	aSet.push(1);
	REQUIRE(aSet.top() == 1);
	REQUIRE(aSet.size() == 1);

	aSet.push(2);
	REQUIRE(aSet.top() == 2);
	REQUIRE(aSet.size() == 2);

	aSet.push(3);
	REQUIRE(aSet.top() == 3);
	REQUIRE(aSet.size() == 3);

	aSet.pop();
	REQUIRE(aSet.top() == 2);
	REQUIRE(aSet.size() == 2);

	aSet.pop();
	REQUIRE(aSet.top() == 1);
	REQUIRE(aSet.size() == 1);

	aSet.pop();
	REQUIRE(aSet.empty());
	REQUIRE(aSet.size() == 0);
}

TEST_CASE("Reversed (RankedSet)")
{
	RankedSet<integer> aSet(3);
	REQUIRE(aSet.size() == 0);
	REQUIRE(aSet.empty());
	
	aSet.push(3);
	REQUIRE(aSet.top() == 3);
	REQUIRE(aSet.size() == 1);

	aSet.push(2);
	REQUIRE(aSet.top() == 3);
	REQUIRE(aSet.size() == 2);

	aSet.push(1);
	REQUIRE(aSet.top() == 3);
	REQUIRE(aSet.size() == 3);

	aSet.pop();
	REQUIRE(aSet.top() == 2);
	REQUIRE(aSet.size() == 2);

	aSet.pop();
	REQUIRE(aSet.top() == 1);
	REQUIRE(aSet.size() == 1);

	aSet.pop();
	REQUIRE(aSet.empty());
	REQUIRE(aSet.size() == 0);
}

TEST_CASE("Overflow (RankedSet)")
{
	RankedSet<integer> aSet(3);
	REQUIRE(aSet.size() == 0);
	REQUIRE(aSet.empty());
	
	aSet.push(1);
	aSet.push(2);
	aSet.push(3);
	aSet.push(4);
	REQUIRE(aSet.top() == 3);
	REQUIRE(aSet.size() == 3);

	aSet.push(2);
	REQUIRE(aSet.top() == 2);
	REQUIRE(aSet.size() == 3);

	aSet.pop();
	REQUIRE(aSet.top() == 2);
	REQUIRE(aSet.size() == 2);

	aSet.pop();
	REQUIRE(aSet.top() == 1);
	REQUIRE(aSet.size() == 1);

	aSet.pop();
	REQUIRE(aSet.size() == 0);

	REQUIRE_THROWS_AS(aSet.pop(), InvariantFailure);
}
