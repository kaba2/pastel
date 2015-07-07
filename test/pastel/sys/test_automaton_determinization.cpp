// Description: Testing for automaton determinization
// DocumentationOf: automaton_determinization.h

#include "test/test_init.h"

#include "pastel/sys/automaton/automaton_determinization.h"
#include "pastel/sys/automaton/automaton_minimization.h"

#include <boost/range/algorithm/for_each.hpp>

namespace
{

	class Test
	{
	public:
		virtual void run()
		{
			test();
		}

		using Automaton_ = Automaton<integer, integer>;
		using State = Automaton_::State_ConstIterator;
		using Transition = Automaton_::Transition_ConstIterator;

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

			REQUIRE(!automaton.deterministic());

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
			REQUIRE(det.deterministic());

			//std::cout << det << std::endl;

			Automaton_ minimal = minimizeAutomaton(det);
			REQUIRE(minimal.deterministic());

			//std::cout << minimal << std::endl;
		}
	};

	TEST_CASE("determinizeAutomaton", "[determinizeAutomaton]")
	{
	}

}