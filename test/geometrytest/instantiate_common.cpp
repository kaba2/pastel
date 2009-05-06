#include "pastel/sys/mytypes.h"

namespace
{

	typedef Pastel::real TestReal;

}

#include "pastel/geometry/contains_alignedbox_alignedbox.h"

namespace Pastel
{

	template
		bool contains(
		const AlignedBox<1, TestReal>& outer,
		const AlignedBox<1, TestReal>& inner);

	template
		bool contains(
		const AlignedBox<2, TestReal>& outer,
		const AlignedBox<2, TestReal>& inner);

	template
		bool contains(
		const AlignedBox<3, TestReal>& outer,
		const AlignedBox<3, TestReal>& inner);

	template
		bool contains(
		const AlignedBox<4, TestReal>& outer,
		const AlignedBox<4, TestReal>& inner);

	template
		bool contains(
		const AlignedBox<5, TestReal>& outer,
		const AlignedBox<5, TestReal>& inner);

}

#include "pastel/geometry/bihtree.h"

namespace Pastel
{

	namespace
	{

		template class BihTree<1, TestReal, int>;

		template class BihTree<2, TestReal, int>;

		template class BihTree<3, TestReal, int>;

		template class BihTree<4, TestReal, int>;

		template class BihTree<5, TestReal, int>;

	}

}

#include "pastel/geometry/kdtree.h"

namespace Pastel
{

	namespace
	{

		template <int N>
		class ObjectPolicy
		{
		public:
			typedef int Object;
			typedef TrueType UseBounds;

			Tuple<2, TestReal> bound(
				const Object& object, integer index) const
			{
				return Tuple<2, TestReal>();
			}

			AlignedBox<N, TestReal> bound(
				const Object& object) const
			{
				return AlignedBox<N, TestReal>();
			}
		};

		template class KdTree<1, TestReal, ObjectPolicy<1> >;

		template class KdTree<2, TestReal, ObjectPolicy<2> >;

		template class KdTree<3, TestReal, ObjectPolicy<3> >;

		template class KdTree<4, TestReal, ObjectPolicy<4> >;

		template class KdTree<5, TestReal, ObjectPolicy<5> >;

	}

}
