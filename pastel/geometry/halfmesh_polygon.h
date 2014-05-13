// Description: Half-edge structure polygon

#ifndef PASTELGEOMETRY_HALFMESH_POLYGON_H
#define PASTELGEOMETRY_HALFMESH_POLYGON_H

#include "pastel/geometry/halfmesh.h"

namespace Pastel
{
	
	namespace HalfMesh_
	{

		template <typename Settings>
		class Polygon
			: boost::less_than_comparable<Polygon<Settings>
			, boost::equality_comparable<Polygon<Settings>
			> >
			, public HalfMesh_Fwd<Settings>::PolygonData_Class
		{
		public:
			using Fwd = HalfMesh_Fwd<Settings>;
			PASTEL_FWD(PolygonData_Class);
			PASTEL_FWD(Half_Iterator);
			PASTEL_FWD(Half_ConstIterator);

			template <typename, template <typename> class>
			friend class HalfMesh;

			Polygon() = default;

			operator PolygonData_Class&()
			{
				return *this;
			}

			operator const PolygonData_Class&() const
			{
				return *this;
			}

			Half_Iterator half()
			{
				return half_;
			}

			Half_ConstIterator half() const
			{
				return half_;
			}

		private:
			Polygon(const Polygon&) = delete;
			Polygon(Polygon&&) = delete;
			Polygon& operator=(Polygon) = delete;
			
			Half_Iterator half_;
		};

	}

}

#endif
