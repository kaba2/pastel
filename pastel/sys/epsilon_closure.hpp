#ifndef PASTEL_EPSILON_CLOSURE_HPP
#define PASTEL_EPSILON_CLOSURE_HPP

#include "pastel/sys/epsilon_closure.h"
#include "pastel/sys/transitive_closure.h"
#include "pastel/sys/hash.h"

#include <unordered_set>
#include <functional>

namespace Pastel
{

	template <
		typename StateSet,
		typename Symbol,
		typename StateData,
		typename TransitionData,
		typename Customization
		typename State_StateSet_Reporter>
	void epsilonClosure(
		const Automaton<Symbol, StateData, TransitionData, Customization>& automaton,
		const State_StateSet_Reporter& report)
	{
		typedef Automaton<Symbol, StateData, TransitionData>
			Automaton;
		typedef Automaton::State_ConstIterator
			State_ConstIterator;

		// The epsilon-set is computed for each state of the automaton.
		auto forEachDomain = 
			[&](const std::function<void(const State_ConstIterator&)>& visit)
		{
			std::for_each(
				automaton.cStateBegin(), 
				automaton.cStateEnd(),
				[&](const State_ConstIterator& state)
			{
				visit(state);
			});
		};

		// The epsilon-set of a state A contains the epsilon-set
		// of a state B if and only if there is an epsilon-transition 
		// from A to B.
		auto forEachRelated =
			[&](const State_ConstIterator& vertex,
			const std::function<void(const State_ConstIterator&)>& visit)
		{
			// This is less efficient than it could be.
			// Here we traverse over all outgoing transitions,
			// while an automaton with the search-customization
			// could visit exactly those transitions with the 
			// epsilon symbol.
			for (auto incidence = vertex->cOutgoingBegin();
				incidence != vertex->cOutgoingEnd();
				++incidence)
			{
				if (incidence->edge.symbol() == Epsilon())
				{
					visit(incidence->vertex());
				}
			}
		};

		// Each state contributes itself to an epsilon-set,
		// if referenced by an epsilon-transition.
		auto directEpsilonSet =
			[&](const State_ConstIterator& state)
			-> StateSet
		{
			StateSet stateSet;
			stateSet.insert(state);
			return stateSet;
		};

		// The epsilon-sets are combined by the set-union.
		auto unionOp =
			[&](StateSet&& left, const StateSet& right)
			-> StateSet
		{
			std::for_each(right.cbegin(), right.cend(),
				[&](const State_ConstIterator& state)
			{
				left.insert(state);
			});

			return left;
		};

		// Compute the epsilon-sets by reflexive-transitive 
		// functional closure.
		transitiveClosure<State_ConstIterator>(
			StateSet(), directEpsilonSet, unionOp, 
			forEachRelated, forEachDomain,
			report, true, IteratorAddress_Hash());
	}


}

#endif
