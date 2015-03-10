// Description: Testing for named parameters
// DocumentationOf: named_parameter.h

#include "test_pastelsys.h"

#include <pastel/sys/function/identity_function.h>

#include <string>

using namespace Pastel;

namespace
{

	template <
		typename Set_Optionals = Identity_Function>
		void generateArticle(
		const std::string& title,
		Set_Optionals setOptionals = Set_Optionals())
	{
		struct Optionals
		{
			std::string author = "Anynomous";
			bool interesting = false;
		} p;

		setOptionals(p);

		std::cout << title << " by " << p.author << std::endl;
	}

}

namespace
{

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
			generateArticle("Going bananas");

			generateArticle(
				"Growing beans",
				[](auto& p)
			{
				p.author = "Ben";
			});
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("named_parameter", test);
	}

	CallFunction run(addTest);

}