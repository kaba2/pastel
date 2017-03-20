// Description: Testing template
// Documentation: unit_testing.txt

#include "test/test_init.h"

#include <experimental/generator>
#include <iostream>

class A
{
public:
	explicit A(integer n)
		: n_(n)
	{
	}

	template <typename Type>
	decltype(auto) numberGenerator() const
	{
		for (integer i = 0;i < n_;++i)
		{
			co_yield i;
		}
		co_return;
		std::cout << "WHAAT" << std::endl;
	}

private:
	integer n_;
};

TEST_CASE("Couroutine (Coroutine)")
{
	A a(4);

	for (auto&& i : a.numberGenerator<float>())
	{
		std::cout << i << std::endl;
	}
}
