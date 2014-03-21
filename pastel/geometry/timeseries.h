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
		typedef typename Args::PointRep PointRep;
		typedef typename PointRep::Real Real;
		typedef typename PointRep::Point Point;
		PASTEL_CONSTEXPR int N = PointRep::N;

	private:
		class Node;
		typedef std::unique_ptr<Node> NodePtr;

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
