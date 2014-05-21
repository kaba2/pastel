// Description: Down filter concept
// Documentation: downfilter.txt

#ifndef PASTELSYS_DOWNFILTER_CONCEPT_H
#define PASTELSYS_DOWNFILTER_CONCEPT_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	namespace DownFilter_Concept
	{

		class DownFilter
		{
		private:
			using Element = UserDefinedType;

		public:
			//! Returns whether an element is marked.
			bool element(const Element& that) const
			{
				return true;
			}

			//! Returns whether a down-set contains a marked element.
			/*!
			The down-set of 'that' is the set of elements
			which are <= 'that' in the pre-order over which
			the down-filter is defined.
			*/
			bool downSet(const Element& that) const
			{
				return true;
			}
		};

	}

}

#endif
