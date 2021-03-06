// Description: Testing for Array
// DocumentationOf: array.h

#include "test/test_init.h"

#include "pastel/sys/array.h"

namespace
{

	template <typename Type>
	bool equal(const Array<Type>& left, const Array<Type>& right)
	{
		if (left.extent() != right.extent())
		{
			return false;
		}

		integer width = left.width();
		integer height = left.height();
		for (integer y = 0;y < height;++y)
		{
			for (integer x = 0;x < width;++x)
			{
				if (left(x, y) != right(x, y))
				{
					return false;
				}
			}
		}

		return true;
	}

	void f(const SubArray<integer>& that,
		integer value)
	{
		that = value;
	}

	void print(const Array<integer, 2>& that)
	{
		integer height = that.height();
		integer width = that.width();
		for (integer y = 0;y < height;++y)
		{
			for (integer x = 0;x < width;++x)
			{
				std::cout << that(x, y) << " ";
			}
			std::cout << std::endl;
		}
	}

}

TEST_CASE("Position (Array)")
{
	{
		Array<integer> a(Vector2i(2, 3), 0, StorageOrder::RowMajor);

		REQUIRE(a.position(0) == Vector2i(0, 0));
		REQUIRE(a.position(1) == Vector2i(1, 0));
		REQUIRE(a.position(2) == Vector2i(0, 1));
		REQUIRE(a.position(3) == Vector2i(1, 1));
		REQUIRE(a.position(4) == Vector2i(0, 2));
		REQUIRE(a.position(5) == Vector2i(1, 2));

		REQUIRE(a.index(Vector2i(0, 0)) == 0);
		REQUIRE(a.index(Vector2i(1, 0)) == 1);
		REQUIRE(a.index(Vector2i(0, 1)) == 2);
		REQUIRE(a.index(Vector2i(1, 1)) == 3);
		REQUIRE(a.index(Vector2i(0, 2)) == 4);
		REQUIRE(a.index(Vector2i(1, 2)) == 5);
	}
	{
		Array<integer> a(Vector2i(2, 3), 0, StorageOrder::ColumnMajor);

		REQUIRE(a.position(0) == Vector2i(0, 0));
		REQUIRE(a.position(1) == Vector2i(0, 1));
		REQUIRE(a.position(2) == Vector2i(0, 2));
		REQUIRE(a.position(3) == Vector2i(1, 0));
		REQUIRE(a.position(4) == Vector2i(1, 1));
		REQUIRE(a.position(5) == Vector2i(1, 2));

		REQUIRE(a.index(Vector2i(0, 0)) == 0);
		REQUIRE(a.index(Vector2i(0, 1)) == 1);
		REQUIRE(a.index(Vector2i(0, 2)) == 2);
		REQUIRE(a.index(Vector2i(1, 0)) == 3);
		REQUIRE(a.index(Vector2i(1, 1)) == 4);
		REQUIRE(a.index(Vector2i(1, 2)) == 5);
	}
}

TEST_CASE("CopyAssign (Array)")
{
	integer Width = 100;
	integer Height = 200;

	Array<int, 2> a(Vector2i(Width, Height));
	for (integer i = 0;i < a.size();++i)
	{
		a(i) = i;
	}

	REQUIRE(a.size() == Width * Height);
	REQUIRE(a.extent() == Vector2i(Width, Height));
	REQUIRE(a.storageOrder() == StorageOrder::RowMajor);

	Array<int, 2> b(a);

	REQUIRE(b.size() == a.size());
	REQUIRE(b.extent() == a.extent());
	REQUIRE(b.storageOrder() == StorageOrder::RowMajor);
	REQUIRE(equal(b, a));

	Array<int, 2> c(a, StorageOrder::RowMajor);

	REQUIRE(c.size() == a.size());
	REQUIRE(c.extent() == a.extent());
	REQUIRE(c.storageOrder() == StorageOrder::RowMajor);
	REQUIRE(equal(c, a));

	a.clear();
	REQUIRE(a.size() == 0);
	REQUIRE(allEqual(a.extent(), 0));
	REQUIRE(a.empty());

	a = std::move(c);
	REQUIRE(a.size() == b.size());
	REQUIRE(a.extent() == b.extent());
	REQUIRE(!a.empty());
	REQUIRE(equal(a, b));
	REQUIRE(c.empty());
	REQUIRE(c.size() == 0);
	REQUIRE(allEqual(c.extent(), 0));

	a = a;
	REQUIRE(a.size() == b.size());
	REQUIRE(a.extent() == b.extent());
	REQUIRE(equal(a, b));

	a = std::move(a);
	REQUIRE(a.size() == b.size());
	REQUIRE(a.extent() == b.extent());
	REQUIRE(equal(a, b));

	b.clear();
	b.setExtent(Vector2i(Width + 100, Height + 50));

	b = a;
	REQUIRE(equal(a, b));

	a.setExtent(Vector2i(53, 45), 15);
	a.setExtent(Vector2i(20, 200), 7);
}

TEST_CASE("Iterator (Array)")
{
	Array<integer, 2> a(Vector2i(6, 6));
	for (integer i = 0;i < a.size();++i)
	{
		a(i) = i;
	}

	Array<integer, 2> b(Vector2i(6, 6));
	ranges::copy(
		intervalRange((integer)0, b.size()),
		b.begin());

	REQUIRE(std::equal(a.begin(), a.end(), b.begin()));

	for (integer i = 0;i < a.width();++i)
	{
		a(i, 0) = a(i, 1);
	}

	std::copy(
		a.rowBegin(1), a.rowEnd(1),
		a.rowBegin(0));

	REQUIRE(std::equal(a.rowBegin(0), a.rowEnd(0), a.rowBegin(1)));
}

TEST_CASE("SubArray (Array)")
{
	Array<integer, 2> a(Vector2i(6, 6));

	a = { 0, 1, 2, 3, 4, 5,
		6, 7, 8, 9, 10, 11,
		12, 13, 14, 15, 16, 17,
		18, 19, 20, 21, 22, 23,
		24, 25, 26, 27, 28, 29,
		30, 31, 32, 33, 34, 35 };

	REQUIRE(ranges::equal(a, ranges::view::ints(0, 36)));

	a(Vector2i(0, 0), Vector2i(3, 3)) = 
		a(Vector2i(3, 3), Vector2i(6, 6));

	Array<integer, 2> b(Vector2i(6, 6));
	b = { 21, 22, 23, 3, 4, 5,
		27, 28, 29, 9, 10, 11,
		33, 34, 35, 15, 16, 17,
		18, 19, 20, 21, 22, 23,
		24, 25, 26, 27, 28, 29,
		30, 31, 32, 33, 34, 35 };

	REQUIRE(ranges::equal(a, b));

	a(Vector2i(0, 0), Vector2i(6, 6), Vector2i(2, 2)) = 0;

	b = { 0, 22, 0, 3, 0, 5,
		27, 28, 29, 9, 10, 11,
		0, 34, 0, 15, 0, 17,
		18, 19, 20, 21, 22, 23,
		0, 25, 0, 27, 0, 29,
		30, 31, 32, 33, 34, 35 };

	REQUIRE(ranges::equal(a, b));

	a(Vector2i(1, 1), Vector2i(6, 6), Vector2i(2, 2)) = 1;

	b = { 0, 22, 0, 3, 0, 5,
		27, 1, 29, 1, 10, 1,
		0, 34, 0, 15, 0, 17,
		18, 1, 20, 1, 22, 1,
		0, 25, 0, 27, 0, 29,
		30, 1, 32, 1, 34, 1 };

	REQUIRE(ranges::equal(a, b));

	a(Vector2i(2, 2), Vector2i(-1, -1), Vector2i(-1, -1)) = 
		a(Vector2i(3, 0), Vector2i(6, 3));			

	b = { 17, 0, 15, 3, 0, 5,
		1, 10, 1, 1, 10, 1,
		5, 0, 3, 15, 0, 17,
		18, 1, 20, 1, 22, 1,
		0, 25, 0, 27, 0, 29,
		30, 1, 32, 1, 34, 1 };

	REQUIRE(ranges::equal(a, b));

	SubArray<integer> c(a(Vector2i(0, 0), Vector2i(2, 2)));
	c = 2;

	b = { 2, 2, 15, 3, 0, 5,
		2, 2, 1, 1, 10, 1,
		5, 0, 3, 15, 0, 17,
		18, 1, 20, 1, 22, 1,
		0, 25, 0, 27, 0, 29,
		30, 1, 32, 1, 34, 1 };

	REQUIRE(ranges::equal(a, b));

	f(c, 3);

	b = { 3, 3, 15, 3, 0, 5,
		3, 3, 1, 1, 10, 1,
		5, 0, 3, 15, 0, 17,
		18, 1, 20, 1, 22, 1,
		0, 25, 0, 27, 0, 29,
		30, 1, 32, 1, 34, 1 };

	REQUIRE(ranges::equal(a, b));
}

TEST_CASE("Slice (Array)")
{
	Array<integer> a(Vector2i(6, 6));
	a = { 0, 1, 2, 3, 4, 5,
		6, 7, 8, 9, 10, 11,
		12, 13, 14, 15, 16, 17,
		18, 19, 20, 21, 22, 23,
		24, 25, 26, 27, 28, 29,
		30, 31, 32, 33, 34, 35 };

	{
		Array<integer, 1> b(Vector1i(6));
		b = { 12, 13, 14, 15, 16, 17 };
		SubArray<integer, 1> slice = a().slice(1, 2);
		REQUIRE(ranges::equal(slice, b));
	}

	{
		Array<integer, 1> b(Vector1i(6));
		b = { 3, 9, 15, 21, 27, 33 };
		SubArray<integer, 1> slice = a().slice(0, 3);
		REQUIRE(ranges::equal(slice, b));

		/*
		std::copy(
			slice.begin(), slice.end(), 
			std::ostream_iterator<integer>(std::cout, " "));
		std::cout << std::endl;
		*/
	}
}

