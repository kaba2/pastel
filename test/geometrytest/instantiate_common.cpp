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

		template class KdTree<1, TestReal, ObjectPolicy<1> >;

		template class KdTree<2, TestReal, ObjectPolicy<2> >;

		template class KdTree<3, TestReal, ObjectPolicy<3> >;

		template class KdTree<4, TestReal, ObjectPolicy<4> >;

		template class KdTree<5, TestReal, ObjectPolicy<5> >;

	}

}
