#ifndef PASTEL_FLATSIMPLEXMESH_H
#define PASTEL_FLATSIMPLEXMESH_H

#include "pastel/geometry/simplexmesh.h"

namespace Pastel
{

	namespace FlatSimplexMesh_Detail
	{

		template <int N, typename Real, typename DataPolicy>
		class GeometryDataPolicy
			: public DataPolicy
		{
		protected:
			typedef typename DataPolicy::VertexData NewVertexData;
			typedef typename DataPolicy::SimplexData NewSimplexData;

			class VertexData
				: private PossiblyEmptyMember<NewVertexData>
			{
			private:
				typedef PossiblyEmptyMember<NewVertexData> Base;

			public:
				friend class GeometryDataPolicy<N, Real, DataPolicy>;

				VertexData()
					: position_()
				{
				}

				Point<N, Real>& position()
				{
					return position_;
				}

				const Point<N, Real>& position() const
				{
					return position_;
				}

				NewVertexData& data()
				{
					return *Base::data();
				}

				const NewVertexData& data() const
				{
					return *Base::data();
				}

			private:
				Point<N, Real> position_;
			};

			class SimplexData
				: private PossiblyEmptyMember<NewSimplexData>
			{
			private:
				typedef PossiblyEmptyMember<NewSimplexData> Base;

			public:
				friend class GeometryDataPolicy<N, Real, DataPolicy>;

				NewSimplexData& data()
				{
					return *Base::data();
				}

				const NewSimplexData& data() const
				{
					return *Base::data();
				}
			};

			void swap(GeometryDataPolicy& that)
			{
			}

			void clear()
			{
			}

			void constructVertex(VertexData* vertex)
			{
				DataPolicy::constructVertex(
					&vertex->data());
			}

			void constructSimplex(SimplexData* simplex)
			{
				DataPolicy::constructSimplex(
					&simplex->data());
			}

			void destructVertex(VertexData* vertex)
			{
				DataPolicy::destructVertex(
					&vertex->data());
			}

			void destructSimplex(SimplexData* simplex)
			{
				DataPolicy::destructSimplex(
					&simplex->data());
			}
		};

	}

	/*!
	DataPolicy:
	See the documentation for SimplexMesh's DataPolicy concept.
	*/

	template <int N, typename Real, typename DataPolicy = SimplexMesh_EmptyDataPolicy>
	class FlatSimplexMesh
		: public SimplexMesh<N, FlatSimplexMesh_Detail::GeometryDataPolicy<N, Real, DataPolicy> >
	{
	};

}

#endif
