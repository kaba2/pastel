// Description: Bounded array concepts

#ifndef PASTELSYS_BOUNDED_ARRAY_CONCEPTS_H
#define PASTELSYS_BOUNDED_ARRAY_CONCEPTS_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	namespace BoundedArray_Concepts
	{

		class Settings
		{
		public:
			//! The type of the elements.
			using Element = UserDefinedType;

			//! The bound for the array size.
			/*!
			Precondition:
			N >= 0
			*/
			static PASTEL_CONSTEXPR integer N = UserDefinedInteger;
		};

		template <typename Settings>
		class Customization
		{
		public:
			//! Called after construction.
			void onConstruct() {}

			//! Called at the beginning of swap().
			void swap(Customization& that) {}

			//! Called at the beginning of clear().
			void onClear() {}

			//! Called at the end of emplaceBack().
			void onEmplaceBack() {}

			//! Called at the beginning of popBack().
			void onPopBack() {}
		};

	}	

}

#endif
