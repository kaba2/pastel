#ifndef PASTEL_AUTOMATON_CLOSURE_HPP
#define PASTEL_AUTOMATON_CLOSURE_HPP

#include "pastel/sys/automaton_closure.h"
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
		typename Customization,
		typename IsClosureSymbol,
		typename Insert_State,
		typename Closure_Reporter>
	void automatonClosure(
		const Automaton<Symbol, StateData, TransitionData, Customization>& automaton,
		const IsClosureSymbol& isClosureSymbol,
		const Insert_State& insert,
		const Closure_Reporter& report)
	{
		typedef Automaton<Symbol, StateData, TransitionData>
			Automaton;
		typedef Automaton::State_ConstIterator
			State_ConstIterator;

		// The closure-set is computed for each state of the automaton.
		auto forEachDomain = 
			[&](const std::function<void(const State_ConstIterator&)>& visit)
		{
			for (auto state = automaton.cStateBegin();
				state != automaton.cStateEnd();
				++state)
			{
				visit(state);
			}
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
				if (isClosureSymbol(incidence->edge()->symbol()))
				{
					visit(incidence->vertex());
				}
			}
		};

		// Each state contributes itself to an closure-set,
		// if referenced by a transition with a closure symbol.
		auto directSet =
			[&](const State_ConstIterator& state)
			-> StateSet
		{
			StateSet stateSet;
			insert(state, stateSet);
			return stateSet;
		};

		// The closure-sets are combined by the set-union.
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

		// Compute the closure-sets by reflexive-transitive 
		// functional closure.
		transitiveClosure<State_ConstIterator, StateSet>(
			StateSet(), directSet, unionOp, 
			forEachRelated, forEachDomain,
			report, true, IteratorAddress_Hash());
	}

	template <
		typename StateSet,
		typename Symbol,
		typename StateData,
		typename TransitionData,
		typename Customization,
		typename Insert_State,
		typename Closure_Reporter>
	void epsilonClosure(
		const Automaton<Symbol, StateData, TransitionData, Customization>& automaton,
		const Insert_State& insert,
		const Closure_Reporter& report)
	{
		auto isClosureSymbol =
			[&](const Optional<Symbol>& symbol)
		{
			return symbol.empty();
		};

		return automatonClosure<StateSet>(
			automaton, isClosureSymbol, 
			insert, report);
	}

}

#endif
