// Description: Testing for bounded array
// DocumentationOf: bounded_array.h

#include "test/test_init.h"

#include <pastel/sys/bounded_array.h>

TEST_CASE("bounded_array (bounded_array)")
{
	using Set = BoundedArray<BoundedArray_Settings<integer, 3>>;
	using RealSet = BoundedArray<BoundedArray_Settings<real, 2>>;
	Set a;
	{
		REQUIRE(a.empty());
	}
	{
		a.emplaceBack(1);
		REQUIRE(!a.empty());
		REQUIRE(!a.full());
		REQUIRE(a.size() == 1);
		REQUIRE(a.front() == 1);
		REQUIRE(a.back() == 1);
		REQUIRE(a[0] == 1);
	}
	{
		a.emplaceBack(2);
		REQUIRE(!a.empty());
		REQUIRE(!a.full());
		REQUIRE(a.size() == 2);
		REQUIRE(a.front() == 1);
		REQUIRE(a.back() == 2);
		REQUIRE(a[0] == 1);
		REQUIRE(a[1] == 2);
	}
	{
		a.emplaceBack(3);
		REQUIRE(!a.empty());
		REQUIRE(a.full());
		REQUIRE(a.size() == 3);
		REQUIRE(a.front() == 1);
		REQUIRE(a.back() == 3);
		REQUIRE(a[0] == 1);
		REQUIRE(a[1] == 2);
		REQUIRE(a[2] == 3);
	}
	{
		Set b = { 5, 3, 4 };

		std::vector<integer> correctSet = { 5, 3, 4 };
		REQUIRE(boost::equal(b.range(), range(correctSet.begin(), correctSet.end())));
		REQUIRE(boost::equal(b.range(), correctSet));
		REQUIRE(boost::equal(b, correctSet));

		auto i = b.begin();
		REQUIRE(*i == 5);
		REQUIRE(i[0] == 5);
		REQUIRE(i[1] == 3);
		REQUIRE(i[2] == 4);

		++i;
		REQUIRE(*i == 3);
		REQUIRE(i[0] == 3);
		REQUIRE(i[1] == 4);

		++i;
		REQUIRE(*i == 4);
		REQUIRE(i[0] == 4);

		++i;
		REQUIRE(i == b.end());
	}
	{
		Set b = { 5, 3, 4 };
		REQUIRE(b.size() == 3);
		REQUIRE(b.back() == 4);

		b.popBack();
		REQUIRE(b.size() == 2);
		REQUIRE(b.back() == 3);

		b.popBack();
		REQUIRE(b.size() == 1);
		REQUIRE(b.back() == 5);

		b.popBack();
		REQUIRE(b.size() == 0);
		REQUIRE(b.empty());
	}
	{
		Set b = { 5, 3, 4 };
		b.clear();

		REQUIRE(b.empty());
		REQUIRE(b.size() == 0);
	}

	{
		Set a = { 1, 2 };
		Set b = { 5, 3, 4 };

		a.swap(b);
		REQUIRE(a.size() == 3);
		REQUIRE(b.size() == 2);

		REQUIRE(a[0] == 5);
		REQUIRE(a[1] == 3);
		REQUIRE(a[2] == 4);

		REQUIRE(b[0] == 1);
		REQUIRE(b[1] == 2);
	}
	{
		RealSet a = { 0.5, 1.5 };
		Set b(a);
		RealSet c(b);
		Set d(b);

		Set correct = { 0, 1 };
		REQUIRE(boost::equal(b, correct));
		REQUIRE(boost::equal(c, correct));
		REQUIRE(boost::equal(d, correct));
	}
}

