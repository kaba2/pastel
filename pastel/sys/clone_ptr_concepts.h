// Description: Concepts for ClonePtr

#ifndef PASTEL_CLONE_PTR_CONCEPTS_H
#define PASTEL_CLONE_PTR_CONCEPTS_H

#include "pastel/sys/clone_ptr.h"

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	namespace ClonePtr_Concepts
	{

		typedef UserDefinedType Type;
		
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
