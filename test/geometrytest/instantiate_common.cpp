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
		const AlignedBox<TestReal, 1>& outer,
		const AlignedBox<TestReal, 1>& inner);

	template
		bool contains(
		const AlignedBox<TestReal, 2>& outer,
		const AlignedBox<TestReal, 2>& inner);

	template
		bool contains(
		const AlignedBox<TestReal, 3>& outer,
		const AlignedBox<TestReal, 3>& inner);

	template
		bool contains(
		const AlignedBox<TestReal, 4>& outer,
		const AlignedBox<TestReal, 4>& inner);

	template
		bool contains(
		const AlignedBox<TestReal, 5>& outer,
		const AlignedBox<TestReal, 5>& inner);

}

#include "pastel/geometry/bihtree.h"

namespace Pastel
{

	namespace
	{

		template class BihTree<TestReal, 1, int>;

		template class BihTree<TestReal, 2, int>;

		template class BihTree<TestReal, 3, int>;

		template class BihTree<TestReal, 4, int>;

		template class BihTree<TestReal, 5, int>;

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

			Tuple<TestReal, 2> bound(
				const Object& object, integer index) const
			{
				return Tuple<TestReal, 2>();
			}

			AlignedBox<TestReal, N> bound(
				const Object& object) const
			{
				return AlignedBox<TestReal, N>();
			}
		};

		template class KdTree<TestReal, 1, ObjectPolicy<1> >;

		template class KdTree<TestReal, 2, ObjectPolicy<2> >;

		template class KdTree<TestReal, 3, ObjectPolicy<3> >;

		template class KdTree<TestReal, 4, ObjectPolicy<4> >;

		template class KdTree<TestReal, 5, ObjectPolicy<5> >;

	}

}
