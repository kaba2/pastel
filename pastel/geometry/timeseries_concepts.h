// Description: Concepts for time series'

#ifndef PASTELGEOMETRY_TIMESERIES_CONCEPTS_H
#define PASTELGEOMETRY_TIMESERIES_CONCEPTS_H

#include "pastel/geometry/timeseries.h"

#include "pastel/sys/locator_concept.h"

namespace Pastel
{

	namespace TimeSeries_Concepts
	{

		class Args
		{
		public:
			typedef Locator_Concept Locator;
		};

	}

}

#endif
