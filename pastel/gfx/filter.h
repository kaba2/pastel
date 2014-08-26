// Description: Filter class
// Detail: Reconstruction filter abstraction
// Documentation: filters.txt

#ifndef PASTELGFX_FILTER_H
#define PASTELGFX_FILTER_H

#include "pastel/gfx/gfxlibrary.h"

#include "pastel/sys/mytypes.h"

#include <memory>
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
	However, this leads to too long names. I decided
	to leave this semantic information for the documentation.
	*/

	class Filter
	{
	public:
		// Using default constructor.
		// Using default copy constructor.
		// Using default assignment.

		Filter(real radius,
			const std::string& name)
		: radius_(radius)
		, name_(name)
		{
			ENSURE_OP(radius, >, 0);
		}

		virtual ~Filter()
		{
		}

		//! Evaluates the filter inside the radius.
		/*!
		Preconditions:
		-radius <= position <= radius

		This function performs faster because
		it can leave out range checking.
		*/
		virtual real evaluateInRange(real position) const = 0;

		void setRadius(real radius)
		{
			ENSURE_OP(radius, >, 0);
			radius_ = radius;
			onSetRadius();
		}
		
		real radius() const
		{
			return radius_;
		}

		real evaluate(real x) const
		{
			if (x < -radius_ || x > radius_)
			{
				return 0;
			}

			return evaluateInRange(x);
		}

		const std::string& name() const
		{
			return name_;
		}

	private:
		Filter(const Filter& that) = delete;
		Filter& operator=(const Filter& that) = delete;

		virtual void onSetRadius()
		{
		}

		real radius_;
		std::string name_;
	};

	using FilterPtr = std::shared_ptr<Filter>;
	using ConstFilterPtr = std::shared_ptr<const Filter>;

}

#endif
