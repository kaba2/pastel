// Description: Testing for automaton determinization
// DocumentationOf: automaton_determinization.h

#include "test_pastelsys.h"

#include "pastel/sys/automaton_determinization.h"
#include "pastel/sys/automaton_minimization.h"

#include <boost/range/algorithm/for_each.hpp>

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

		typedef Automaton<integer, integer> Automaton_;
		typedef Automaton_::State_ConstIterator State;
		typedef Automaton_::Transition_ConstIterator Transition;

		void test()
		{
			Automaton_ automaton;

			// Let us build the regular language
			// (0 + 1)* 0 (0 + 1)

			State a = automaton.addState(0);
			State b = automaton.addState(1);
			State c = automaton.addState(2);

			automaton.addTransition(a, 0, a);
			automaton.addTransition(a, 1, a);
			automaton.addTransition(a, 0, b);
			automaton.addTransition(b, 0, c);
			automaton.addTransition(b, 1, c);
			
			automaton.addStart(a);
			automaton.addFinal(c);

			TEST_ENSURE(!automaton.deterministic());

			//std::cout << automaton << std::endl;

			using StateSet = Hashed_Set<
				State, IteratorAddress_LessThan, void, void,
				IteratorAddress_Hash>;

			Automaton_ det;

			std::unordered_map<
				const StateSet*, State> stateMap;
			
			auto reportState =
				[&](const StateSet& stateSet,
				bool start)
			{
				stateMap[&stateSet] = det.addState();

				bool allStartStates = true;
				boost::for_each(stateSet.crange(),
					[&](const State& state)
				{
					if (state->final())
					{
						det.addFinal(stateMap[&stateSet]);
					}
				});

				if (start)
				{
					det.addStart(stateMap[&stateSet]);
				}
			};
			
			auto reportTransition =
				[&](const StateSet& fromStateSet,
				const Optional<integer>& symbol,
				const StateSet& toStateSet)
			{
				det.addTransition(
					stateMap[&fromStateSet],
					symbol,
					stateMap[&toStateSet]);
			};

			determinizeAutomaton(automaton,
				reportState, reportTransition);
			TEST_ENSURE(det.deterministic());

			//std::cout << det << std::endl;

			Automaton_ minimal = minimizeAutomaton(det);
			TEST_ENSURE(minimal.deterministic());

			//std::cout << minimal << std::endl;
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
