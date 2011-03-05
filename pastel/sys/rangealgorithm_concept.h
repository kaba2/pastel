// Description: Range algorithm concept

#ifndef PASTEL_RANGEALGORITHM_CONCEPT_H
#define PASTEL_RANGEALGORITHM_CONCEPT_H

#include "pastel/sys/algorithm_concept.h"
#include "pastel/sys/forwardrange_concept.h"

namespace Pastel
{

	namespace RangeAlgorithm_Concept
	{

		class RangeAlgorithm
			: public Algorithm_Concept::Algorithm
		{
		public:
			//! Applies the algorithm to an iterator range.
			template <typename ForwardRange>
			void operator()(ForwardRange range);
		};

	}

	namespace RangeAlgorithm2_Concept
	{

		class RangeAlgorithm2
			: public Algorithm_Concept::Algorithm
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

	}

	namespace RangeAlgorithm3_Concept
	{

		class RangeAlgorithm3
			: public Algorithm_Concept::Algorithm
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

}

#endif
