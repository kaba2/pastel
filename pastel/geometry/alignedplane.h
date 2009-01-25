/*!
\file
\brief A class for an axis-aligned plane.
*/

#ifndef PASTELGEOMETRY_ALIGNEDPLANE_H
#define PASTELGEOMETRY_ALIGNEDPLANE_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! An axis-aligned plane.

	/*!
	An axis-aligned plane can be described by an
	axis and a position on that axis. The plane
	is orthogonal to the given axis.
	The axis is described by an index
	into the sequence of natural basis vectors
	e = ((1, 0, 0, ...), (0, 1, 0, ...), ...).
	*/

	template <int N, typename Real>
	class AlignedPlane
	{
	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		//! Constructs an x-orthogonal plane at origin.
		AlignedPlane();

		//! Constructs an 'axis'-orthogonal plane at 'position'.
		AlignedPlane(const Real& position, integer axis);

		// Used for concept checking.
		~AlignedPlane();

		//! Swaps two AlignedPlane's.
		void swap(AlignedPlane<N, Real>& that);

		//! Sets the position of the plane on the axis.
		void setPosition(const Real& position);

		//! Returns the position of the plane on the axis.
		const Real& position() const;

		//! Sets the index of the orthogonal axis of the plane.
		void setAxis(integer axis);

		//! Returns the index of the orthogonal axis of the plane.
		integer axis() const;

	private:
		Real position_;
		integer axis_;
	};

	typedef AlignedPlane<2, real> AlignedPlane2;
	typedef AlignedPlane<3, real> AlignedPlane3;
	typedef AlignedPlane<4, real> AlignedPlane4;

}

#include "pastel/geometry/alignedplane.hpp"

#endif
