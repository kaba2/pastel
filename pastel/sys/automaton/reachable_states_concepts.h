#ifndef PASTELSYS_REACHABLE_STATES_CONCEPTS_H
#define PASTELSYS_REACHABLE_STATES_CONCEPTS_H

#include "pastel/sys/automaton/reachable_states.h"

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	namespace ReachableStates_Concepts
	{

		//! Automaton<...>::State_ConstIterator
		using State_ConstIterator = UserDefinedType;

		//! Marks a state as visited.
		class Mark
		{
		public:
			void operator()(const State_ConstIterator& state) const;
		};

		//! Returns whether a state has been visited.
		class Marked
		{
		public:
			void operator()(const State_ConstIterator& state) const;
		};

	}

}

#endif
