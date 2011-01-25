// Description: Range algorithm concept

#ifndef PASTEL_RANGEALGORITHM_CONCEPT_H
#define PASTEL_RANGEALGORITHM_CONCEPT_H

#include "pastel/sys/algorithm_concept.h"
#include "pastel/sys/forwardrange_concept.h"

namespace Pastel
{

	class RangeAlgorithm_Concept
		: public Algorithm_Concept
	{
	public:
		//! Applies the algorithm to an iterator range.
		template <typename ForwardRange>
		void operator()(ForwardRange range);
	};

	class RangeAlgorithm2_Concept
		: public Algorithm_Concept
	{
	public:
		//! Applies the algorithm to iterator ranges.
		template <
			typename A_ForwardRange,
			typename B_ForwardRange>
		void operator()(
			A_ForwardRange aRange,
			B_ForwardRange bRange);
	};

	class RangeAlgorithm3_Concept
		: public Algorithm_Concept
	{
	public:
		//! Applies the algorithm to iterator ranges.
		template <
			typename A_ForwardRange,
			typename B_ForwardRange,
			typename C_ForwardRange>
		void operator()(
			A_ForwardRange aRange,
			B_ForwardRange bRange,
			C_ForwardRange cRange);
	};

}

#endif
