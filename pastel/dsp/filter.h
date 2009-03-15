/*!
\file
\brief A reconstruction filter.
*/

#ifndef PASTELDSP_FILTER_H
#define PASTELDSP_FILTER_H

#include "pastel/dsp/dsplibrary.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/countedptr.h"

#include <string>

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

	class PASTELDSP Filter
		: public ReferenceCounted
	{
	public:
		// Using default constructor.
		// Using default copy constructor.
		// Using default assignment.

		Filter(real radius,
			const std::string& name);
		virtual ~Filter();

		//! Evaluates the filter inside the radius.
		/*!
		Preconditions:
		-radius <= position <= radius

		This function performs faster because
		it can leave out range checking.
		*/
		virtual real evaluateInRange(real position) const = 0;

		void setRadius(real radius);
		real radius() const;

		real evaluate(real position) const;
		const std::string& name() const;

	private:
		// Prohibited
		Filter(const Filter& that);
		// Prohibited
		Filter& operator=(const Filter& that);

		virtual void onSetRadius();

		real radius_;
		std::string name_;
	};

	typedef CountedPtr<Filter> FilterRef;
	typedef CountedPtr<const Filter> ConstFilterRef;

}

#endif
