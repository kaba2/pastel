#ifndef PASTELSYS_AUTOMATON_CLOSURE_HPP
#define PASTELSYS_AUTOMATON_CLOSURE_HPP

#include "pastel/sys/automaton/automaton_closure.h"
#include "pastel/sys/transitive_closure/transitive_closure.h"
#include "pastel/sys/hashing/iteratoraddress_hash.h"

#include <unordered_set>
#include <functional>

namespace Pastel
{

	template <
		typename Symbol,
		typename StateData,
		typename TransitionData,
		typename Customization,
		typename StateSet,
		typename ForEachRelated,
		typename Insert_State,
		typename Closure_Output>
	void automatonClosure(
		const Automaton<Symbol, StateData, TransitionData, Customization>& automaton,
		const StateSet& emptySet,
		const ForEachRelated& forEachRelated,
		const Insert_State& insert,
		const Closure_Output& report)
	{
		typedef Automaton<Symbol, StateData, TransitionData, Customization>
			Automaton;
		typedef typename Automaton::State_ConstIterator
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

		// Each state contributes itself to an closure-set,
		// if referenced by a transition with a closure symbol.
		auto directSet =
			[&](const State_ConstIterator& state)
			-> StateSet
		{
			StateSet stateSet(emptySet);
			insert(state, stateSet);
			return stateSet;
		};

		// The closure-sets are combined by the set-union.
		auto unionOp =
			[&](StateSet&& left, const StateSet& right)
			-> StateSet
		{
			for (const auto& state : right)
			{
				insert(state, left);
			}

			return left;
		};

		// Compute the closure-sets by reflexive-transitive 
		// functional closure.
		transitiveClosure<State_ConstIterator, StateSet>(
			emptySet, forEachDomain, forEachRelated, directSet, unionOp, 
			report, true, IteratorAddress_Hash());
	}

	template <
		typename Symbol,
		typename StateData,
		typename TransitionData,
		typename Customization,
		typename StateSet,
		typename Insert_State,
		typename Closure_Output>
	void epsilonClosure(
		const Automaton<Symbol, StateData, TransitionData, Customization>& automaton,
		const StateSet& emptySet,
		const Insert_State& insert,
		const Closure_Output& report)
	{
		typedef Automaton<Symbol, StateData, TransitionData, Customization>
			Automaton;
		typedef typename Automaton::State_ConstIterator
			State_ConstIterator;
		typedef typename Automaton::Transition_ConstIterator
			Transition_ConstIterator;

		using BranchMap = typename Automaton::BranchMap;

		auto forEachRelated =
			[&](const State_ConstIterator& state, 
			const std::function<void(const State_ConstIterator&)>& visit)
		{
			auto branchRange = automaton.cBranchRange(state, Epsilon());
			
			std::for_each(
				branchRange.begin(),
				branchRange.end(),
				[&](const Transition_ConstIterator& transition)
			{
				visit(transition->to());
			});
			
			/*for (auto branch = branchRange.begin();
				branch != branchRange.end();
				++branch)
			{
				visit((*branch)->to());
			}*/
		};

		return automatonClosure(
			automaton, emptySet, forEachRelated, 
			insert, report);
	}

}

#endif
