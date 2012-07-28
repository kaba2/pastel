#ifndef PASTEL_AUTOMATON_MINIMIZATION_HPP
#define PASTEL_AUTOMATON_MINIMIZATION_HPP

#include "pastel/sys/automaton_minimization.h"

#include "pastel/sys/unaccepting_traversal.h"
#include "pastel/sys/unreachable_traversal.h"
#include "pastel/sys/refinable_partition.h"
#include "pastel/sys/counting_iterator.h"

#include <unordered_map>

namespace Pastel
{

	template <
		typename Symbol, 
		typename StateData, 
		typename TransitionData>
	Automaton<Symbol, StateData, TransitionData> minimizeAutomaton(
		Automaton<Symbol, StateData, TransitionData> automaton)
	{
		// "Fast brief practical DFA minimization",
		// Antti Valmari, Information Processing Letters,
		// Volume 112, Issue 6, 2012, pp. 213-217.

		typedef Automaton<Symbol, StateData, TransitionData> Automaton;
		typedef typename Automaton::State_ConstIterator
			State_ConstIterator;
		typedef typename Automaton::Transition_ConstIterator
			Transition_ConstIterator;

		typedef RefinablePartition<State_ConstIterator>
			StatePartition;
		typedef typename StatePartition::Set_Iterator
			Block_Iterator;
		typedef typename StatePartition::Element_Iterator
			BlockElement_Iterator;

		typedef RefinablePartition<Transition_ConstIterator>
			TransitionPartition;
		typedef typename TransitionPartition::Set_Iterator
			Cord_Iterator;
		typedef typename TransitionPartition::Element_Iterator
			CordElement_Iterator;

		// Remove unreachable states.
		forEachUnreachable(
			automaton,
			[&](const State_ConstIterator& state)
		{
			automaton.removeState(state);
		});
		
		// Remove unaccepting states.
		forEachUnaccepting(
			automaton,
			[&](const State_ConstIterator& state)
		{
			automaton.removeState(state);
		});

		// These are used for fast conversion of an 
		// automaton element to a partition element.

		std::unordered_map<State_ConstIterator, 
			BlockElement_Iterator, IteratorAddress_Hash> blockElementSet;

		std::unordered_map<Transition_ConstIterator, 
			CordElement_Iterator, IteratorAddress_Hash> cordElementSet;

		// Create the initial partition of states with 
		// a single set containing all the states.
		
		RefinablePartition<State_ConstIterator>
			statePartition;

		Block_Iterator initialBlock = 
			statePartition.addSet();
		
		for (auto state = automaton.cStateBegin();
			state != automaton.cStateEnd();
			++state)
		{
			BlockElement_Iterator element = 
				statePartition.insertOne(initialBlock, state);

			blockElementSet.insert(
				std::make_pair(state, element));
		}

		// Partition the states into final states
		// and non-final states.

		std::for_each(
			automaton.cFinalBegin(),
			automaton.cFinalEnd(),
			[&](const State_ConstIterator& state)
		{
			statePartition.mark(
				blockElementSet[state]);
		});

		statePartition.split();

		// Create the initial partition of transitions
		// based on the symbols.
		RefinablePartition<Transition_ConstIterator>
			transitionPartition;

		std::unordered_map<Symbol, 
			Cord_Iterator> searchSet;
		for (auto transition = automaton.cTransitionBegin();
			transition != automaton.cTransitionEnd();
			++transition)
		{
			// See if we have already created a partition-set
			// for this symbol.
			auto search = searchSet.find(
				transition->symbol());
			if (search == searchSet.end())
			{
				// There is no partition-set for this symbol.
				// Create a new one.
				search = searchSet.insert(
					std::make_pair(
					transition->symbol(),
					transitionPartition.addSet())).first;
			}
			
			Cord_Iterator cord =
				search->second;
			
			// Add the transition into the cord.
			CordElement_Iterator element = 
				transitionPartition.insertOne(
				cord, transition);

			cordElementSet.insert(
				std::make_pair(transition, element));
		}

		// Partition the states into minimal amount of 
		// equivalence classes based on compatibility.

		Block_Iterator block = statePartition.setBegin();

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
					blockElementSet[transition->from()]);
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
						transitionPartition.mark(
							cordElementSet[incidence->edge()]);
					}
				});

				// Make the cords compatible with this block by
				// splitting the incompatible cords.
				transitionPartition.split();

				++block;
			}
		}

		return automaton;
	}

}

#endif
