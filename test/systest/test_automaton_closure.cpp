// Description: Testing for automaton closure
// Documentation: unit_testing.txt

#include "test_pastelsys.h"

#include "pastel/sys/automaton_closure.h"
#include "pastel/sys/sequence_algorithms.h"
#include "pastel/sys/predicates.h"

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

		void test()
		{
			typedef Automaton<integer> Automaton;
			typedef Automaton::State_ConstIterator
				State;

			Automaton automaton;
			
			State a = automaton.addState();
			State b = automaton.addState();
			State c = automaton.addState();
			State d = automaton.addState();
			State e = automaton.addState();

			automaton.addTransition(
				a, Epsilon(), b);

			automaton.addTransition(
				b, Epsilon(), c);

			automaton.addTransition(
				c, Epsilon(), a);

			automaton.addTransition(
				d, Epsilon(), c);

			automaton.addTransition(
				d, Epsilon(), e);

			typedef std::set<State, IteratorAddress_LessThan> StateSet;
			std::unordered_map<State, StateSet, IteratorAddress_Hash> closureMap;

			auto report =
				[&](const State& state,
				StateSet&& stateSet)
			{
				closureMap.insert(
					std::make_pair(state, std::move(stateSet)));
			};

			auto insert =
				[&](const State& state,
				StateSet& stateSet)
			{
				stateSet.insert(state);
			};

			epsilonClosure(automaton, StateSet(), insert, report);
			
			{
				State correctClosure[] = {a, b, c};
				std::sort(
					std::begin(correctClosure), 
					std::end(correctClosure),
					IteratorAddress_LessThan());
				
				TEST_ENSURE(boost::equal(range(correctClosure), closureMap[a]));
				TEST_ENSURE(boost::equal(range(correctClosure), closureMap[b]));
				TEST_ENSURE(boost::equal(range(correctClosure), closureMap[c]));
			}

			{
				State correctClosure[] = {a, b, c, d, e};
				std::sort(
					std::begin(correctClosure), 
					std::end(correctClosure),
					IteratorAddress_LessThan());
				
				TEST_ENSURE(boost::equal(range(correctClosure), closureMap[d]));
			}

			{
				State correctClosure[] = {e};
				std::sort(
					std::begin(correctClosure), 
					std::end(correctClosure),
					IteratorAddress_LessThan());
				
				TEST_ENSURE(boost::equal(range(correctClosure), closureMap[e]));
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
		testRunner().add("automatonClosure", test);
	}

	CallFunction run(addTest);

}
