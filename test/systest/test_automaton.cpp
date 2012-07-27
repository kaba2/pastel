// Description: Testing for Automaton
// DocumentationOf: automaton.h

#include "pastelsystest.h"

#include <pastel/sys/automaton.h>

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
			testSimple();
		}

		void testSimple()
		{
			typedef Automaton<integer, void, void> Automaton;
			typedef Automaton::State_Iterator State;
			typedef Automaton::Transition_Iterator Transition;

			Automaton automaton;
			State a = automaton.addState();
			State b = automaton.addState();
			State c = automaton.addState();

			Transition A = automaton.addTransition(
				a, 0, b);
			Transition B = automaton.addTransition(
				b, 0, c);
			automaton.setStartState(a);
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("Automaton", test);
	}

	CallFunction run(addTest);

}
