// Description: Testing for Automaton
// DocumentationOf: automaton.h

#include "test/test_init.h"

#include <pastel/sys/automaton.h>
#include <pastel/sys/automaton/reachable_states.h>
#include <pastel/sys/automaton/productive_states.h>
#include <pastel/sys/automaton/automaton_minimization.h>
#include <pastel/sys/hashing/iteratoraddress_hash.h>

#include <algorithm>
#include <unordered_set>

namespace
{

	class Test
	{
	public:
		virtual void run()
		{
			testSimple();
		}

		void testSimple()
		{
			using Automaton = Automaton<integer>;
			using State_ConstIterator = Automaton::State_ConstIterator;
			using State_Iterator = Automaton::State_Iterator;
			using Transition = Automaton::Transition_Iterator;

			Automaton automaton;

			State_ConstIterator a = automaton.addState();
			State_ConstIterator b = automaton.addState();
			State_ConstIterator c = automaton.addState();
			State_ConstIterator d = automaton.addState();
			State_ConstIterator e = automaton.addState();
			unused(e);

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
					IteratorAddress_LessThan());

				State_ConstIterator correctSet[] = {a, b, c};
				std::sort(
					std::begin(correctSet), 
					std::end(correctSet), 
					IteratorAddress_LessThan());

				REQUIRE(reachableSet.size() == 3);
				if (reachableSet.size() == 3)
				{
					REQUIRE(
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
					IteratorAddress_LessThan());

				State_ConstIterator correctSet[] = {a, b, c, d};
				std::sort(
					std::begin(correctSet), 
					std::end(correctSet), 
					IteratorAddress_LessThan());

				REQUIRE(productiveSet.size() == 4);
				if (productiveSet.size() == 4)
				{
					REQUIRE(
						std::equal(
						std::begin(productiveSet),
						std::end(productiveSet),
						std::begin(correctSet)));
				}
			}
			*/
		}
	};

	TEST_CASE("Automaton", "[Automaton]")
	{
	}

}
