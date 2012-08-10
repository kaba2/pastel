// Description: Testing for automaton determinization
// DocumentationOf: automaton_determinization.h

#include "pastelsystest.h"

#include "pastel/sys/regex_automata.h"
#include "pastel/sys/automaton_determinization.h"
#include "pastel/sys/automaton_minimization.h"

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
			// (0 + 1)* 0 (0 + 1)^2.

			Automaton zero = regularSymbol<integer, void, void, 
				Automaton_Concepts::Customization<integer, void, void>>(0);

			Automaton one = regularSymbol<integer, void, void, 
				Automaton_Concepts::Customization<integer, void, void>>(1);

			Automaton zeroOrOne = regularUnion(zero, one);

			Automaton zeroOrOneStar = regularKleeneStar(zeroOrOne);

			Automaton zeroOrOneStarAndZero = 
				regularSequence(zeroOrOneStar, zero);

			Automaton regex = regularSequence(
				zeroOrOneStarAndZero, zeroOrOne);

			std::cout << regex << std::endl;

			typedef AsHashedTree<
				State, IteratorAddress_LessThan,
				IteratorAddress_Hash>::type StateSet;

			Automaton det;

			std::unordered_map<
				const StateSet*, State> stateMap;
			
			auto reportState =
				[&](const StateSet& stateSet,
				bool start)
			{
				stateMap[&stateSet] = det.addState();

				bool allStartStates = true;
				std::for_each(
					stateSet.cbegin(),
					stateSet.cend(),
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

			determinizeAutomaton(regex,
				reportState, reportTransition);

			std::cout << det << std::endl;

			Automaton minimal = minimizeAutomaton(det);

			std::cout << minimal << std::endl;
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
