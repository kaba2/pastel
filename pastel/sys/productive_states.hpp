#ifndef PASTEL_PRODUCTIVE_STATES_HPP
#define PASTEL_PRODUCTIVE_STATES_HPP

#include "pastel/sys/productive_states.h"

#include "pastel/sys/depth_first_traversal.h"

namespace Pastel
{

	template <
		typename Symbol,
		typename StateData,
		typename TransitionData,
		typename State_Reporter,
		typename Mark,
		typename Marked>
	void forEachProductive(
		const Automaton<Symbol, StateData, TransitionData>& automaton,
		const State_Reporter& report,
		const Mark& mark,
		const Marked& marked)
	{
		typedef Automaton<Symbol, StateData, TransitionData>
			Automaton;
		typedef Automaton::State_ConstIterator
			State_ConstIterator;

		if (automaton.finalStates() == 0)
		{
			// Every state is non-productive.
			return;
		}

		if (automaton.finalStates() == automaton.states())
		{
			// Every state is productive.
			for (auto state = automaton.cStateBegin();
				state != automaton.cStateEnd();
				++state)
			{
				report(state);
			}

			return;
		}

		auto forEachSeedVertex = 
			[&](const std::function<void(const State_ConstIterator&)>& visit)
		{
			for (auto state = automaton.cFinalBegin();
				state != automaton.cFinalEnd();
				++state)
			{
				visit(*state);
			}
		};

		auto forEachAdjacent =
			[&](const State_ConstIterator& vertex,
			const std::function<void(const State_ConstIterator&)>& visit)
		{
			for (auto edge = vertex->cIncomingBegin();
				edge != vertex->cIncomingEnd();
				++edge)
			{
				visit(edge->vertex());
			}
		};

		traverseDepthFirst<State_ConstIterator>(
			forEachSeedVertex, forEachAdjacent, report, 
			mark, marked);
	}

}

#endif
