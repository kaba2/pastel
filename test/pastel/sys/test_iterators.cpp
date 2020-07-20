// Description: Testing for iterators
// Documentation: iterator.txt

#include "test/test_init.h"

#include "pastel/sys/iterator/sparse_iterator.h"
#include "pastel/sys/iterator/rectangle_iterator.h"

#include "pastel/sys/iterator/second_iterator.h"

#include <boost/iterator/transform_iterator.hpp>
#include <map>
#include <compare>

namespace
{

	class A;

	using PairSet = std::map<A, A>;
	using Pair_Iterator = Second_Iterator<PairSet::iterator, false>;
	using Pair_ConstIterator = Second_Iterator<PairSet::const_iterator, true>;

	class A
	{
	public:
		A(int a)
			: a_(a)
		{
		}

		auto operator<=>(const A& that) const = default;

		int a_;
	};

}

TEST_CASE("Second (Second)")
{
	PairSet a;
	a.insert(std::make_pair(1, 3));
	a.insert(std::make_pair(2, 4));
	a.insert(std::make_pair(4, -2));
	a.insert(std::make_pair(5, 1));

	std::vector<A> b;
	b.push_back(3);
	b.push_back(4);
	b.push_back(-2);
	b.push_back(1);

	REQUIRE(std::equal(b.begin(), b.end(), 
		Pair_Iterator(a.begin())));

	{
		Pair_Iterator aIter(a.begin());
		Pair_ConstIterator bIter(aIter);
		//Pair_Iterator c(b);

		REQUIRE(aIter == aIter);
		REQUIRE(aIter == bIter);
		REQUIRE(!(aIter != aIter));
		REQUIRE(!(aIter != bIter));

		++aIter;
		--aIter;
		aIter++;
		aIter--;

		++bIter;
		--bIter;
		bIter++;
		bIter--;
	}

	integer i = 0;
	std::for_each(
		Pair_Iterator(a.begin()), Pair_Iterator(a.end()),
		[&](A& a)
	{
		a.a_ = i;
		++i;
	});

	b[0] = 0;
	b[1] = 1;
	b[2] = 2;
	b[3] = 3;

	REQUIRE(std::equal(b.begin(), b.end(), 
		Pair_ConstIterator(a.begin())));
}

namespace
{

	template <typename Iterator>
	void f(const ConstSparseIterator<Iterator>& that)
	{
	}

}

TEST_CASE("Sparse (Iterators)")
{
	integer a[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	// SparseIterator

	SparseIterator<integer*> iter(a, 2);
	REQUIRE(*iter == 0);
	++iter;
	REQUIRE(*iter == 2);
	--iter;
	REQUIRE(*iter == 0);
	iter += 2;
	REQUIRE(*iter == 4);
	iter -= 2;
	REQUIRE(*iter == 0);
	iter = iter + 1;
	REQUIRE(*iter == 2);
	iter = iter - 1;
	REQUIRE(*iter == 0);
	REQUIRE(iter == iter);
	REQUIRE(!(iter != iter));
	REQUIRE(!(iter < iter));
	REQUIRE(!(iter > iter));
	REQUIRE(iter <= iter);
	REQUIRE(iter >= iter);
	REQUIRE(iter - iter == 0);

	// ConstSparseIterator

	ConstSparseIterator<const integer*> cIter(iter);
	REQUIRE(*cIter == 0);
	++cIter;
	REQUIRE(*cIter == 2);
	--cIter;
	REQUIRE(*cIter == 0);
	cIter += 2;
	REQUIRE(*cIter == 4);
	cIter -= 2;
	REQUIRE(*cIter == 0);
	cIter = cIter + 1;
	REQUIRE(*cIter == 2);
	cIter = cIter - 1;
	REQUIRE(*cIter == 0);
	REQUIRE(cIter == cIter);
	REQUIRE(!(cIter != cIter));
	REQUIRE(!(cIter < cIter));
	REQUIRE(!(cIter > cIter));
	REQUIRE(cIter <= cIter);
	REQUIRE(cIter >= cIter);
	REQUIRE(cIter - cIter == 0);

	// Co-operation

	cIter = iter;
	cIter += 2;

	REQUIRE(cIter - iter == 2);
	REQUIRE(iter - cIter == -2);
	REQUIRE(cIter != iter);

	cIter = iter;
	REQUIRE(cIter == cIter);
	REQUIRE(iter == cIter);

	*iter = 2;
	REQUIRE(*iter == 2);

	*cIter;

	//*cIter = 2;
	//f(iter);
}

TEST_CASE("Rectangle (Iterators)")
{
	{
		RectangleIterator<2> iter(
			Vector2i(1, 2), Vector2i(3, 4));

		REQUIRE(iter.position() == Vector2i(1, 2));
		REQUIRE(!iter.done());
		++iter;

		REQUIRE(iter.position() == Vector2i(2, 2));
		REQUIRE(!iter.done());
		++iter;

		REQUIRE(iter.position() == Vector2i(1, 3));
		REQUIRE(!iter.done());
		++iter;

		REQUIRE(iter.position() == Vector2i(2, 3));
		REQUIRE(!iter.done());
		++iter;

		REQUIRE(iter.position() == Vector2i(1, 2));
		REQUIRE(iter.done());
	}
	{
		RectangleIterator<2> iter(
			Vector2i(2, 3));

		REQUIRE(iter.position() == Vector2i(0, 0));
		REQUIRE(!iter.done());
		++iter;

		REQUIRE(iter.position() == Vector2i(1, 0));
		REQUIRE(!iter.done());
		++iter;

		REQUIRE(iter.position() == Vector2i(0, 1));
		REQUIRE(!iter.done());
		++iter;

		REQUIRE(iter.position() == Vector2i(1, 1));
		REQUIRE(!iter.done());
		++iter;

		REQUIRE(iter.position() == Vector2i(0, 2));
		REQUIRE(!iter.done());
		++iter;

		REQUIRE(iter.position() == Vector2i(1, 2));
		REQUIRE(!iter.done());
		++iter;

		REQUIRE(iter.position() == Vector2i(0, 0));
		REQUIRE(iter.done());
	}
}

TEST_CASE("Counting (Iterators)")
{
	{
		std::vector<integer> a;
		a.push_back(1);
		a.push_back(2);
		a.push_back(3);
		a.push_back(4);
		a.push_back(5);
		a.push_back(6);

		REQUIRE(
			ranges::equal(a, 
			intervalRange((integer)1, (integer)7)));
	}
	{
		std::vector<integer> a;
		a.push_back(1);
		a.push_back(3);
		a.push_back(5);
		a.push_back(7);

		REQUIRE(
			ranges::equal(a, 
			sparseRange(intervalRange((integer)1, (integer)8), 2)));
	}
}

