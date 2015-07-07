// Description: Testing for insertion sort
// DocumentationOf: insertion_sort.h

#include "test/test_init.h"

#include "pastel/sys/sequence/insertion_sort.h"

namespace
{

	class A
	{
	public:
		A()
			: data_(0)
		{
		}

		explicit A(integer data)
			: data_(data)
		{
		}

		void swap(A& that)
		{
			std::swap(data_, that.data_);
		}

	private:
		integer data_;
	};

	void swap(A& left, A& right)
	{
		left.swap(right);
	}

}

TEST_CASE("InsertionSort (InsertionSort)")
{
	integer n = 100;

	std::vector<integer> v;
	v.reserve(n);
	for (integer i = 0;i < n;++i)
	{
		v.push_back(i);
	}

	std::random_shuffle(v.begin(), v.end());

	insertionSort(v.begin(), v.end());

	for (integer i = 0;i < n;++i)
	{
		REQUIRE(v[i] == i);
	}
}
