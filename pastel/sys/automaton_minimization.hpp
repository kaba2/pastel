#ifndef PASTEL_AUTOMATON_MINIMIZATION_HPP
#define PASTEL_AUTOMATON_MINIMIZATION_HPP

#include "pastel/sys/automaton_minimization.h"

#include "pastel/sys/counting_iterator.h"
#include "pastel/sys/productive_states.h"
#include "pastel/sys/reachable_states.h"
#include "pastel/sys/refinable_partition.h"

#include <unordered_map>
#include <unordered_set>

namespace Pastel
{

	template <
		typename Symbol, 
		typename StateData, 
		typename TransitionData>
	Automaton<Symbol, StateData, TransitionData> minimizeAutomaton(
		const Automaton<Symbol, StateData, TransitionData>& automaton)
	{
		// "Fast brief practical DFA minimization",
		// Antti Valmari, Information Processing Letters,
		// Volume 112, Issue 6, 2012, pp. 213-217.

		typedef Automaton<Symbol, StateData, TransitionData> Automaton;
		typedef typename Automaton::State_ConstIterator
			State_ConstIterator;
		typedef typename Automaton::Transition_ConstIterator
			Transition_ConstIterator;

		typedef RefinablePartition<State_ConstIterator, State_ConstIterator>
			StatePartition;
		typedef typename StatePartition::Set_Iterator
			Block_Iterator;
		typedef typename StatePartition::Set_ConstIterator
			Block_ConstIterator;
		typedef typename StatePartition::Element_Iterator
			BlockElement_Iterator;
		typedef typename StatePartition::Element_ConstIterator
			BlockElement_ConstIterator;

		typedef RefinablePartition<Transition_ConstIterator>
			TransitionPartition;
		typedef typename TransitionPartition::Set_Iterator
			Cord_Iterator;
		typedef typename TransitionPartition::Set_ConstIterator
			Cord_ConstIterator;
		typedef typename TransitionPartition::Element_Iterator
			CordElement_Iterator;
		typedef typename TransitionPartition::Element_ConstIterator
			CordElement_ConstIterator;

		typedef std::unordered_set<State_ConstIterator, IteratorAddress_Hash>
			StateSet;

		// The automaton has to be deterministic.
		ENSURE_OP(automaton.startStates(), <=, 1);

		// This will contain the minimal automaton.
		Automaton minimal;

		if (automaton.startStates() == 0)
		{
			// If there are no start states, then the minimal
			// automaton is the empty automaton (the automaton
			// recognizes the empty language).
			return minimal;			
		}

		// A state is relevant if it can be reached from the
		// start state (reachability) and can reach a final 
		// state (productivity).
		StateSet relevantSet;
		
		// Compute the states reachable from the start symbol.

		StateSet reachableSet;

		auto markReachable =
			[&](const State_ConstIterator& state)
		{
			reachableSet.insert(state);
		};

		auto markedReachable =
			[&](const State_ConstIterator& state) -> bool
		{
			return reachableSet.count(state);
		};

		forEachReachable(
			automaton,
			markReachable,
			markedReachable);

		// Compute the states that can reach a final state.

		StateSet productiveSet;

		auto markProductive =
			[&](const State_ConstIterator& state) 
		{
			productiveSet.insert(state);

			if (reachableSet.count(state))
			{
				// Make a state relevant if it is both
				// reachable and productive.
				relevantSet.insert(state);
			}
		};

		auto markedProductive =
			[&](const State_ConstIterator& state) -> bool
		{
			return productiveSet.count(state);
		};

		forEachProductive(
			automaton,
			markProductive,
			markedProductive);

		// These are used for fast conversion of an 
		// automaton element to a partition element.

		std::unordered_map<State_ConstIterator, 
			BlockElement_Iterator, IteratorAddress_Hash> stateToElement;

		std::unordered_map<Transition_ConstIterator, 
			CordElement_Iterator, IteratorAddress_Hash> transitionToElement;

		// Create the initial partition of states with 
		// a single set containing all the relevant states.
		
		StatePartition statePartition;

		Block_Iterator initialBlock = 
			statePartition.addSet();
		
		for (auto state = automaton.cStateBegin();
			state != automaton.cStateEnd();
			++state)
		{
			if (!relevantSet.count(state))
			{
				// Ignore irrelevant states.
				continue;
			}

			BlockElement_Iterator element = 
				statePartition.insertOne(initialBlock, state);

			stateToElement.insert(
				std::make_pair(state, element));
		}

		// Partition the states into final states
		// and non-final states.

		std::for_each(
			automaton.cFinalBegin(),
			automaton.cFinalEnd(),
			[&](const State_ConstIterator& state)
		{
			if (relevantSet.count(state))
			{
				ASSERT(stateToElement.count(state));
				statePartition.mark(
					stateToElement[state]);
			}
		});

		statePartition.split();

		// Create the initial partition of transitions
		// based on the symbols.
		TransitionPartition transitionPartition;

		std::unordered_map<Symbol, Cord_Iterator> searchSet;
		for (auto transition = automaton.cTransitionBegin();
			transition != automaton.cTransitionEnd();
			++transition)
		{
			Optional<Symbol> symbol = transition->symbol();
			// The automaton should be deterministic.
			ENSURE(symbol != Epsilon());

			if (!relevantSet.count(transition->from()) ||
				!relevantSet.count(transition->to()))
			{
				// Only consider the transition if both of its
				// states are relevant.
				continue;
			}

			// See if we have already created a partition-set
			// for this symbol.
			auto search = searchSet.find(symbol);
			if (search == searchSet.end())
			{
				// There is no partition-set for this symbol.
				// Create a new one.
				search = searchSet.insert(
					std::make_pair(
					symbol,
					transitionPartition.addSet())).first;
			}
			
			Cord_Iterator cord =
				search->second;
			
			// Add the transition into the cord.
			CordElement_Iterator element = 
				transitionPartition.insertOne(
				cord, transition);

			transitionToElement.insert(
				std::make_pair(transition, element));
		}

		// Partition the states into minimal amount of 
		// equivalence classes based on compatibility.

		Block_Iterator block = statePartition.setBegin();

		// Skip partitioning the cords by the first block.
		// Note that the state-partition always contains at 
		// least one block.
		++block;

		// As long as there are cords for which we haven't yet
		// checked compatibility with blocks (note that this
		// set grows dynamically as the partition gets finer)...
		for (Cord_Iterator cord = transitionPartition.setBegin();
			cord != transitionPartition.setEnd();
			++cord)
		{
			// Find the incompatibilities between blocks and 
			// this cord. A block is compatible with a cord 
			// if and only if either all or none of the states
			// in the block have a transition in the cord.
			std::for_each(
				cord->begin(), cord->end(),
				[&](const CordElement_Iterator& element)
			{
				Transition_ConstIterator transition =
					*element;

				statePartition.mark(
					stateToElement[transition->from()]);
			});

			// Make the blocks compatible with this cord
			// by splitting the incompatible blocks.
			Block_Iterator firstNewBlock = 
				statePartition.split();

			if (block == statePartition.setEnd())
			{
				// If we exhausted the blocks in the previous
				// iteration, then continue with the newly
				// created blocks.

				block = firstNewBlock;
			}

			// Check the currently available cords for 
			// incompatibilities with currently available
			// blocks.
			while(block != statePartition.setEnd())
			{
				// Find the incompatibilities between cords 
				// and this block. A cord is compatible with
				// a block if and only if either all or none 
				// of the transitions in the cord end-up in
				// the block.
				std::for_each(
					block->begin(), block->end(),
					[&](const BlockElement_Iterator& element)
				{
					State_ConstIterator state =
						*element;

					for (auto incidence = state->cIncomingBegin();
						incidence != state->cIncomingEnd();
						++incidence)
					{
						Transition_ConstIterator transition =
							incidence->edge();

						// Ignore irrelevant transitions.
						if (relevantSet.count(transition->from()) &&
							relevantSet.count(transition->to()))
						{
							transitionPartition.mark(
								transitionToElement[transition]);
						}
					}
				});

				// Make the cords compatible with this block by
				// splitting the incompatible cords.
				transitionPartition.split();

				++block;
			}
		}

		// Create the states of the minimal automaton.
		for (auto block = statePartition.setBegin();
			block != statePartition.setEnd();
			++block)
		{
			ASSERT_OP(block->elements(), >, 0);

			// We will store the corresponding minimal 
			// automaton state in the block data.
			*block = minimal.addState();

			// Pick any state from the block.
			State_ConstIterator state =
				**(block->begin());
			if (state->final())
			{
				// If one of the states in the block is
				// final, they all are. Therefore the
				// minimal automaton state is final.
				minimal.addFinal(*block);
			}
		}
		
		// Create the transitions of the minimal automaton.
		for (auto block = statePartition.setBegin();
			block != statePartition.setEnd();
			++block)
		{
			ASSERT_OP(block->elements(), >, 0);

			// Pick any state from the block...
			State_ConstIterator state =
				**(block->begin());

			// ... and report its transitions.
			// The transitions are the same for all
			// states in the block.
			for (auto incidence = state->cOutgoingBegin();
				incidence != state->cOutgoingEnd();
				++incidence)
			{
				Transition_ConstIterator transition =
					incidence->edge();

				// Only report relevant edges.
				if (relevantSet.count(transition->from()) &&
					relevantSet.count(transition->to()))
				{
					minimal.addTransition(
						*(stateToElement[transition->from()]->set()), 
						transition->symbol(), 
						*(stateToElement[transition->to()]->set()));
				}
			}
		}

		// Set the start-state of the minimal automaton.
		State_ConstIterator startState = 
			*automaton.cStartBegin();
		if (relevantSet.count(startState))
		{
			minimal.addStart(
				*(stateToElement[startState]->set()));
		}

		return minimal;
	}

}

#endif
