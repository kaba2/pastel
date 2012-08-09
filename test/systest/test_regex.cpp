// Description: Testing template
// Documentation: unit_testing.txt

#include "pastelsystest.h"

#include "pastel/sys/regex_automata.h"

#include <boost/proto/proto.hpp>

namespace Pastel
{

	namespace proto = boost::proto;

	template <typename Symbol>
	auto regexSymbol(Symbol symbol)
	-> proto::literal<Symbol>
	{
		return proto::lit(symbol);
	}

}

using namespace Pastel;

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
			test();
		}

		void test()
		{
			//(regexSymbol(0) | regexSymbol(1))* << regexSymbol(0) << (regexSymbol(0) | regexSymbol(1));
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("Regex", test);
	}

	CallFunction run(addTest);

}
