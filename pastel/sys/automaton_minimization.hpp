#ifndef PASTEL_AUTOMATON_MINIMIZATION_HPP
#define PASTEL_AUTOMATON_MINIMIZATION_HPP

#include "pastel/sys/automaton_minimization.h"

#include "pastel/sys/unaccepting_traversal.h"
#include "pastel/sys/unreachable_traversal.h"
#include "pastel/sys/refinable_partition.h"

namespace Pastel
{

	template <typename State, typename Symbol>
	Automaton<State, Symbol> minimize(
		Automaton<State, Symbol> automaton)
	{
		// "Fast brief practical DFA minimization",
		// Antti Valmari, Information Processing Letters,
		// Volume 112, Issue 6, 2012, pp. 213-217.

		typedef Automaton<State, Symbol> Automaton;
		typedef typename Automaton::State_ConstIterator
			State_ConstIterator;
		typedef typename Automaton::Transition_ConstIterator
			Transition_ConstIterator;

		typedef RefinablePartition<State_ConstIterator>
			StatePartition;
		typedef typename StatePartition::Set_Iterator
			Block_Iterator;

		typedef RefinablePartition<Transition_ConstIterator>
			TransitionPartition;
		typedef typename StatePartition::Set_Iterator
			Cord_Iterator;

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

		// Create the initial partition of states with 
		// a single set containing all the states.
		
		RefinablePartition<State_ConstIterator>
			statePartition;
		
		statePartition.insert(
			statePartition.addSet(), 
			countingIterator(automaton.cStateBegin()),
			countingIterator(automaton.cStateEnd()));

		// Partition the states into final states
		// and non-final states.

		std::for_each(
			automaton.cFinalBegin(),
			automaton.cFinalEnd(),
			[&](const State_ConstIterator& state)
		{
			statePartition.mark(state);
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
					transitionPartition.addSet()));
			}
			
			Cord_Iterator cord =
				search->second;
			
			// Add the transition into the cord.
			transitionPartition.insertOne(
				cord, transition);
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
				[&](const Transition_ConstIterator& transition)
			{
				statePartition.mark(transition->from());
			};

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
					[&](const State_ConstIterator& state)
				{
					for (auto incidence = state->cIncomingBegin();
						incidence != state->cIncomingEnd();
						++incidence)
					{
						transitionPartition.mark(
							incidence->edge());
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
