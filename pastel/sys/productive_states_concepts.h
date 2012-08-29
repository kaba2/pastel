#ifndef PASTELSYS_PRODUCTIVE_STATES_CONCEPTS_H
#define PASTELSYS_PRODUCTIVE_STATES_CONCEPTS_H

#include "pastel/sys/productive_states.h"

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	namespace ProductiveStates_Concepts
	{

		//! Automaton<...>::State_ConstIterator
		typedef UserDefinedType State_ConstIterator;

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
