/*!
\file
\brief Functions for convolution.
*/

#ifndef PASTELGFX_FILTER_H
#define PASTELGFX_FILTER_H

#include "pastel/gfx/gfxlibrary.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/countedptr.h"

namespace Pastel
{

	//! A reconstruction filter.
	/*!
	This class represents different kinds of 1-dimensional
	reconstruction filters. The usage of this class is extended
	to higher dimensions by giving it a radial distance instead 
	(for radially symmetric filters) or using it for each
	dimension separately (for separable filters). This class
	would be more properly named a 'ReconstructionFilter'. 
	However, this leads to too long names and thus I decided
	to leave this semantic information in the documentation.
	*/

	class PASTELGFX Filter
		: public ReferenceCounted
	{
	public:
		// Using default constructor.
		// Using default copy constructor.
		// Using default assignment.

		virtual ~Filter();

		virtual real operator()(real position) const = 0;
		virtual real radius() const = 0;
	};

	typedef CountedPtr<Filter> FilterRef;
	typedef CountedPtr<const Filter> ConstFilterRef;

}

#endif
