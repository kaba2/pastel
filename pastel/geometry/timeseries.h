// Description: Time series

#ifndef PASTELGEOMETRY_TIMESERIES_H
#define PASTELGEOMETRY_TIMESERIES_H

#include "pastel/geometry/timeseries_concepts.h"
#include "pastel/geometry/pointkdtree.h"

#include <memory>

namespace Pastel
{

	template <typename Args>
	class TimeSeries
	{
	public:
		using PointRep = typename Args::PointRep;
		using Real = typename PointRep::Real;
		using Point = typename PointRep::Point;
		static PASTEL_CONSTEXPR int N = PointRep::N;

	private:
		class Node;
		using NodePtr = std::unique_ptr<Node>;

		class Node
		{
		public:
			integer tSplit_;
			NodePtr left_;
			NodePtr right_;
		};
		
		NodePtr root_;
		integer tBegin_;
	};

}

#endif
