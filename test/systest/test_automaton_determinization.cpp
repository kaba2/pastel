// Description: Testing for automaton determinization
// DocumentationOf: automaton_determinization.h

#include "pastelsystest.h"

#include "pastel/sys/regex_automata.h"
#include "pastel/sys/automaton_determinization.h"

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
			Automaton regex = regularSequence(regularSequence(
				regularSequence(
				regularKleeneStar(zeroOrOne),
				zero), zeroOrOne), zeroOrOne);

			typedef AsHashedTree<State, IteratorAddress_LessThan>::type StateSet;

			typedef std::unordered_map<
				const StateSet*, integer> StateMap;

			StateMap stateMap;

			integer stateId = 0;

			auto reportState =
				[&](const StateSet& stateSet)
			{
				if (!stateMap.count(&stateSet))
				{
					stateMap[&stateSet] = stateId;
					++stateId;
				}
			};
			
			auto reportTransition =
				[&](const StateSet& fromStateSet,
				const Optional<integer>& symbol,
				const StateSet& toStateSet)
			{
				std::cout 
					<< stateMap[&fromStateSet] 
					<< " " << symbol << " " 
					<< stateMap[&toStateSet]
					<< std::endl;
			};

			determinizeAutomaton(regex,
				reportState, reportTransition);
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
