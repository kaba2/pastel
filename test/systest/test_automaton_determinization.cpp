// Description: Testing for automaton determinization
// DocumentationOf: automaton_determinization.h

#include "pastelsystest.h"

#include "pastel/sys/regex_automata.h"

using namespace Pastel;
using namespace std;

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

		typedef Automaton<integer> Automaton;
		typedef Automaton::State_ConstIterator State;
		typedef Automaton::Transition_ConstIterator Transition;

		void test()
		{
			Automaton automaton;

			// Let us build the regular language
			// (0 + 1)* 0 (0 + 1)^n.

			Automaton zero = regularSymbol<integer, void, void, 
				Automaton_Concepts::Customization<integer, void, void>>(0);

			Automaton one = regularSymbol<integer, void, void, 
				Automaton_Concepts::Customization<integer, void, void>>(1);

			Automaton zeroOrOne = regularUnion(zero, one);
			/*
			Automaton regex = regularSequence(regularSequence(
				regularSequence(
				regularKleeneStar(zeroOrOne),
				zero), zeroOrOne), zeroOrOne);
			*/
			
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("determinizeAutomaton", test);
	}

	CallFunction run(addTest);

}
