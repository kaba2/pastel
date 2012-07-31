// Description: Testing for Automaton
// DocumentationOf: automaton.h

#include "pastelsystest.h"

#include <pastel/sys/automaton.h>
#include <pastel/sys/reachable_states.h>
#include <pastel/sys/productive_states.h>
#include <pastel/sys/automaton_minimization.h>

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

		typedef Automaton<integer> Automaton;
		typedef Automaton::State_ConstIterator State_ConstIterator;
		typedef Automaton::State_Iterator State_Iterator;
		typedef Automaton::Transition_Iterator Transition;

		void testSimple()
		{
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
			
			automaton.addStart(a);
			automaton.addFinal(b);
			automaton.addFinal(c);

			std::unordered_set<State_ConstIterator, IteratorAddress_Hash> reachableSet;

			auto markReachable =
				[&](const State_ConstIterator& state)
			{
				reachableSet.insert(state);
			};

			auto markedReachable =
				[&](const State_ConstIterator& state) -> bool
			{
				return reachableSet.count(state);
			};

			forEachReachable(
				automaton,
				markReachable,
				markedReachable);

			/*
			{
				std::sort(
					std::begin(reachableSet), 
					std::end(reachableSet), 
					Iterator_Less());

				State_ConstIterator correctSet[] = {a, b, c};
				std::sort(
					std::begin(correctSet), 
					std::end(correctSet), 
					Iterator_Less());

				TEST_ENSURE_OP(reachableSet.size(), ==, 3);
				if (reachableSet.size() == 3)
				{
					TEST_ENSURE(
						std::equal(
						std::begin(reachableSet),
						std::end(reachableSet),
						std::begin(correctSet)));
				}
			}
			*/

			std::unordered_set<State_ConstIterator, IteratorAddress_Hash> productiveSet;

			auto markProductive =
				[&](const State_ConstIterator& state)
			{
				productiveSet.insert(state);
			};

			auto markedProductive =
				[&](const State_ConstIterator& state) -> bool
			{
				return productiveSet.count(state);
			};

			forEachProductive(
				automaton,
				markProductive,
				markedProductive);

			/*
			{
				std::sort(
					std::begin(productiveSet), 
					std::end(productiveSet), 
					Iterator_Less());

				State_ConstIterator correctSet[] = {a, b, c, d};
				std::sort(
					std::begin(correctSet), 
					std::end(correctSet), 
					Iterator_Less());

				TEST_ENSURE_OP(productiveSet.size(), ==, 4);
				if (productiveSet.size() == 4)
				{
					TEST_ENSURE(
						std::equal(
						std::begin(productiveSet),
						std::end(productiveSet),
						std::begin(correctSet)));
				}
			}
			*/

			automaton = minimizeAutomaton(
				std::move(automaton));
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
