// Description: Testing for Automaton
// DocumentationOf: automaton.h

#include "pastelsystest.h"

#include <pastel/sys/automaton.h>
#include <pastel/sys/unreachable_traversal.h>
#include <pastel/sys/unaccepting_traversal.h>

#include <algorithm>

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
			typedef Automaton<integer> Automaton;
			typedef Automaton::State_ConstIterator State_ConstIterator;
			typedef Automaton::State_Iterator State_Iterator;
			typedef Automaton::Transition_Iterator Transition;

			Automaton automaton;
			State_ConstIterator a = automaton.addState();
			State_ConstIterator b = automaton.addState();
			State_ConstIterator c = automaton.addState();
			State_ConstIterator d = automaton.addState();
			State_ConstIterator e = automaton.addState();

			automaton.addTransition(
				a, 0, b);
			automaton.addTransition(
				b, 0, c);
			automaton.addTransition(
				c, 0, a);
			automaton.addTransition(
				a, 0, c);
			automaton.addTransition(
				d, 0, a);
			automaton.addTransition(
				d, 1, b);
			automaton.addTransition(
				d, 2, c);
			
			automaton.setStartState(a);
			automaton.addFinal(b);
			automaton.addFinal(c);

			std::vector<State_ConstIterator> unreachableSet;
			forEachUnreachable(
				automaton,
				[&](const State_ConstIterator& state)
			{
				unreachableSet.push_back(state);
			});

			{
				std::sort(
					std::begin(unreachableSet), 
					std::end(unreachableSet), 
					Iterator_Less());

				State_ConstIterator correctSet[] = {d, e};
				std::sort(
					std::begin(correctSet), 
					std::end(correctSet), 
					Iterator_Less());

				TEST_ENSURE_OP(unreachableSet.size(), ==, 2);
				if (unreachableSet.size() == 2)
				{
					TEST_ENSURE(
						std::equal(
						std::begin(unreachableSet),
						std::end(unreachableSet),
						std::begin(correctSet)));
				}
			}

			std::vector<State_ConstIterator> unacceptingSet;
			forEachUnaccepting(
				automaton,
				[&](const State_ConstIterator& state)
			{
				unacceptingSet.push_back(state);
			});

			{
				std::sort(
					std::begin(unacceptingSet), 
					std::end(unacceptingSet), 
					Iterator_Less());

				State_ConstIterator correctSet[] = {e};
				std::sort(
					std::begin(correctSet), 
					std::end(correctSet), 
					Iterator_Less());

				TEST_ENSURE_OP(unacceptingSet.size(), ==, 1);
				if (unacceptingSet.size() == 1)
				{
					TEST_ENSURE(
						std::equal(
						std::begin(unacceptingSet),
						std::end(unacceptingSet),
						std::begin(correctSet)));
				}
			}
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
