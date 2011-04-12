// Description: Topology enum for shapes
// Documentation: maths.txt

#ifndef PASTELSYS_TOPOLOGY_H
#define PASTELSYS_TOPOLOGY_H

namespace Pastel
{

	class Topology
	{
	public:
		enum Enum
		{
			Open,
			Closed
		};
	};

	inline Topology::Enum switchTopology(
		Topology::Enum topology)
	{
		if (topology == Topology::Closed)
		{
			return Topology::Open;
		}

		return Topology::Closed;
	}

	template <typename Real>
	bool containsPositiveHalfspace(
		const Real& outerMin, Topology::Enum outerMinTopology,
		const Real& innerMin, Topology::Enum innerMinTopology)
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
		const Real& outerMax, Topology::Enum outerMaxTopology,
		const Real& innerMax, Topology::Enum innerMaxTopology)
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

}

#endif
