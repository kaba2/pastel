// Description: Concepts for ClonePtr

#ifndef PASTELSYS_CLONE_PTR_CONCEPTS_H
#define PASTELSYS_CLONE_PTR_CONCEPTS_H

#include "pastel/sys/pointer/clone_ptr.h"

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	namespace ClonePtr_Concepts
	{

		using Type = UserDefinedType;
		
		class Cloner
		{
		public:
			//! Returns a deep copy of 'that'. 
			Type* operator()(const Type& that) const;
		};

		class Deleter
		{
		public:
			//! Frees the resource in 'that'.
			void operator()(const Type* that) const;
		};

	}

}

#endif
