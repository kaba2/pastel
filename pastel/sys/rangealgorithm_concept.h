// Description: Range algorithm concept

#ifndef PASTEL_RANGEALGORITHM_CONCEPT_H
#define PASTEL_RANGEALGORITHM_CONCEPT_H

#include "pastel/sys/algorithm_concept.h"
#include "pastel/sys/iteratorrange.h"

namespace Pastel
{

	class RangeAlgorithm_Concept
		: public Algorithm_Concept
	{
	public:
		//! Applies the algorithm to an iterator range.
		template <typename Iterator>
		void operator()(const ForwardRange<Iterator>& range);
	};

	class RangeAlgorithm2_Concept
		: public Algorithm_Concept
	{
	public:
		//! Applies the algorithm to iterator ranges.
		template <
			typename Iterator_A,
			typename Iterator_B>
		void operator()(
			const ForwardRange<Iterator_A>& aRange,
			const ForwardRange<Iterator_B>& bRange);
	};

	class RangeAlgorithm3_Concept
		: public Algorithm_Concept
	{
	public:
		//! Applies the algorithm to iterator ranges.
		template <
			typename Iterator_A,
			typename Iterator_B,
			typename Iterator_C>
		void operator()(
			const ForwardRange<Iterator_A>& aRange,
			const ForwardRange<Iterator_B>& bRange,
			const ForwardRange<Iterator_C>& cRange);
	};

}

#endif
