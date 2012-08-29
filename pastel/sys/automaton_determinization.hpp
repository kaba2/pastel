#ifndef PASTELSYS_AUTOMATON_DETERMINIZATION_HPP
#define PASTELSYS_AUTOMATON_DETERMINIZATION_HPP

#include "pastel/sys/automaton_determinization.h"
#include "pastel/sys/automaton_closure.h"
#include "pastel/sys/hashed_tree.h"
#include "pastel/sys/iteratoraddress_hash.h"
#include "pastel/sys/dereferenced_hash.h"

#include <list>
#include <unordered_set>

namespace Pastel
{

	template <
		typename Symbol, 
		typename StateData, 
		typename TransitionData,
		typename Customization,
		typename State_Reporter,
		typename Transition_Reporter>
	void determinizeAutomaton(
		const Automaton<Symbol, StateData, TransitionData, Customization>& automaton,
		const State_Reporter& reportState,
		const Transition_Reporter& reportTransition)
	{
		typedef Automaton<Symbol, StateData, TransitionData, Customization> 
			Automaton;
		typedef typename Automaton::State_ConstIterator
			State_ConstIterator;

		// A state-set is a set automaton states.
		// The important thing about the set-type
		// is that it can be hashed in constant time
		// and so associatively searched efficiently 
		// in a hash table.
		typedef typename AsHashedTree<
			State_ConstIterator, 
			IteratorAddress_LessThan,
			IteratorAddress_Hash>::type StateSet;

		if (automaton.states() == 0)
		{
			return;
		}

		// Compute the epsilon-closure for each state.
		typedef std::unordered_map<State_ConstIterator, 
			StateSet, IteratorAddress_Hash> ClosureMap;
		ClosureMap closureMap;
		{
			auto reportEpsilon =
				[&](const State_ConstIterator& state,
				StateSet&& stateSet)
			{
				closureMap.emplace(
					std::make_pair(state, std::move(stateSet)));
			};

			auto insertEpsilon =
				[&](const State_ConstIterator& state,
				StateSet& stateSet)
			{
				stateSet.insert(state);
			};

			epsilonClosure(
				automaton, StateSet(), 
				insertEpsilon, reportEpsilon);
		}

		// This set contains the state-sets that are
		// waiting to be examined. At the the beginning
		// it only contains the start state-set.
		typedef std::list<StateSet> WorkSet;
		typedef typename WorkSet::iterator Work_Iterator;
		WorkSet workSet;

		// When a state-set from workSet is started to
		// be examined, it is spliced into this set;
		// this preserves the memory address of the
		// state-set.
		WorkSet readySet;

		// This set records the set of state-sets that we have already
		// generated. By using a pointer to a state-set, we avoid making 
		// a copy of the state-set. This is possible since we always 
		// preserve the memory address of the state-sets. The mapped-value
		// gives an iterator to the 'readySet', so that we can guarantee
		// that when we report the user the same state-set (as part of
		// transition), then it always has the same memory address.
		typedef std::unordered_set<const StateSet*, 
			Dereferenced_Hash, Dereferenced_EqualTo> ExistingSet;
		ExistingSet existingSet;

		// Create the start state-set.
		{
			StateSet startStateSet;

			// Add all the states in the epsilon closure
			// of the start states into the 'startStateSet'.
			std::for_each(
				automaton.cStartBegin(),
				automaton.cStartEnd(),
				[&](const State_ConstIterator& state)
			{
				const StateSet& closure =
					closureMap[state];
				startStateSet.insertMany(
					closure.cbegin(),
					closure.cend());
			});

			workSet.emplace_back(std::move(startStateSet));
			existingSet.emplace(&workSet.back());

			// Report the start state-set.
			reportState(
				(const StateSet&)workSet.back(), true);
		}

		while(!workSet.empty())
		{
			// Move the state-set from the work-set to
			// the ready-set. Note that we preserve the
			// memory address of the state-set.
			readySet.splice(
				readySet.cend(),
				workSet,
				workSet.cbegin());

			const StateSet& stateSet = readySet.back();
			ASSERT(existingSet.count(&stateSet));

			// This set will contain, for each symbol,
			// the states that can be reached from the
			// current state-set by following transitions
			// with this symbol. Note this set will not
			// contain epsilons, so using Symbol is ok.
			typedef std::unordered_map<
				Symbol, StateSet> SymbolStateSetMap;
			SymbolStateSetMap symbolStateSetMap;

			// Find out the state-sets which are produced
			// by following each possible symbol for the
			// states in the state-set.
			std::for_each(
				stateSet.cbegin(), stateSet.cend(),
				[&](const State_ConstIterator& state)
			{
				for (auto incidence = state->cOutgoingBegin();
					incidence != state->cOutgoingEnd();
					++incidence)
				{
					const Optional<Symbol>& symbol = 
						incidence->edge()->symbol();

					if (symbol.empty())
					{
						// Do not follow epsilon-transitions.
						continue;
					}

					StateSet& toStateSet = 
						symbolStateSetMap[symbol];
					
					// Add all the states in the epsilon closure
					// of 'state' into the 'toStateSet'.
					const StateSet& closure =
						closureMap[incidence->vertex()];
					toStateSet.insertMany(
						closure.cbegin(),
						closure.cend());
				}
			});

			// Report transitions between state-sets, and report
			// new state-sets.
			for (auto symbolStateSet = symbolStateSetMap.begin();
				symbolStateSet != symbolStateSetMap.end();
				++symbolStateSet)
			{
				const Symbol& symbol =
					symbolStateSet->first;

				StateSet& newStateSet =
					symbolStateSet->second;

				auto existing = existingSet.find(&newStateSet);
				if (existing == existingSet.cend())
				{
					// This state-set is a new one.

					// Store the state-set in the work-set.
					workSet.emplace_back(std::move(newStateSet));
					existingSet.emplace(&workSet.back());
					
					// Report the new state-set.
					reportState(
						(const StateSet&)workSet.back(), false);
					
					// Report the transition which lead
					// into the new state-set.
					reportTransition(
						(const StateSet&)stateSet, symbol, 
						(const StateSet&)workSet.back());
				}
				else
				{
					// The state-set already exists.

					// Report the transition which lead
					// into the state-set.
					reportTransition(
						(const StateSet&)stateSet, symbol, 
						(const StateSet&)**existing);
				}
			}
		}
	}

}

#endif
