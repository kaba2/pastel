// Description: Half-edge structure polygon

#ifndef PASTELGEOMETRY_HALFMESH_POLYGON_H
#define PASTELGEOMETRY_HALFMESH_POLYGON_H

#include "pastel/geometry/halfmesh/halfmesh_fwd.h"

namespace Pastel
{
	
	namespace HalfMesh_
	{

		template <typename Settings>
		class Polygon
			: boost::less_than_comparable<Polygon<Settings>
			, boost::equality_comparable<Polygon<Settings>
			> >
		{
		public:
			using Fwd = HalfMesh_Fwd<Settings>;
			PASTEL_FWD(PolygonData);
			PASTEL_FWD(Half_Iterator);
			PASTEL_FWD(Half_ConstIterator);

			template <typename... Type>
			Polygon(Type&&... data)
			: data_(std::forward<Type>(data)...)
			{
			}

			Polygon(const Polygon& that)
			: data_(that.data_) 
			{
			}

			Polygon(Polygon&& that)
			: data_(std::move(that.data_))
			{
			}

			PolygonData& data()
			{
				return data_;
			}

			const PolygonData& data() const
			{
				return data_;
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
			template <typename, template <typename> class>
			friend class Pastel::HalfMesh;
			
			BOOST_ATTRIBUTE_NO_UNIQUE_ADDRESS
			PolygonData data_;
			Half_Iterator half_;
		};

	}

}

#endif
