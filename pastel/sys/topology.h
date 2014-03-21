// Description: Topology enum for shapes
// Documentation: maths.txt

#ifndef PASTELSYS_TOPOLOGY_H
#define PASTELSYS_TOPOLOGY_H

#include <iostream>

namespace Pastel
{

	enum class Topology : integer
	{
		Open,
		Closed
	};

	inline Topology switchTopology(
		Topology topology)
	{
		if (topology == Topology::Closed)
		{
			return Topology::Open;
		}

		return Topology::Closed;
	}

	template <typename Real>
	bool containsPositiveHalfspace(
		const Real& outerMin, Topology outerMinTopology,
		const Real& innerMin, Topology innerMinTopology)
	{
		if (innerMin <= outerMin)
		{
			if (innerMin < outerMin ||
				(innerMinTopology == Topology::Closed &&
				outerMinTopology == Topology::Open))
			{
				return false;
			}
		}
		
		return true;
	}

	template <typename Real>
	bool containsNegativeHalfspace(
		const Real& outerMax, Topology outerMaxTopology,
		const Real& innerMax, Topology innerMaxTopology)
	{
		if (innerMax >= outerMax)
		{
			if (innerMax > outerMax ||
				(innerMaxTopology == Topology::Closed &&
				outerMaxTopology == Topology::Open))
			{
				return false;
			}
		}
		
		return true;
	}

	inline std::ostream& operator<<(
		std::ostream& stream, 
		const Topology& topology)
	{
		switch(topology)
		{
		case Topology::Open:
			std::cout << "open";
			break;
		case Topology::Closed:
			std::cout << "closed";
			break;
		};
		return stream;
	}

}

#endif
