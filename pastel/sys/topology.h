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

}

#endif
