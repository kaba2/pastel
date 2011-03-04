// Description: Testing for insertion sort
// DocumentationOf: insertionsort.h

#include "pastelsystest.h"

#include "pastel/sys/insertionsort.h"
#include "pastel/sys/log.h"

#include <iostream>

using namespace Pastel;

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
		std::cout << "Swapped.";
		left.swap(right);
	}

	void testInsertionSort()
	{
		const integer n = 100;

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
			REPORT(v[i] != i);
		}
	}

	void addTest()
	{
		sysTestList().add("insertionSort", testInsertionSort);
	}

	CallFunction run(addTest);

}
