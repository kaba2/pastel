#ifndef PASTELGEOMETRY_POINTKDTREE_INVARIANTS_HPP
#define PASTELGEOMETRY_POINTKDTREE_INVARIANTS_HPP

#include "pastel/geometry/pointkdtree/pointkdtree_invariants.h"

namespace Pastel
{

	namespace PointKdTree_
	{

		template <
			typename Settings, template <typename> class Customization,
			typename Locator = typename Settings::Locator,
			typename Real = typename Locator::Real,
			integer N = Locator::N>
		bool testInvariants(
			const PointKdTree<Settings, Customization>& tree,
			const typename PointKdTree<Settings, Customization>::Cursor& cursor,
			const AlignedBox<Real, N>& bound)
		{
			using Tree = PointKdTree<Settings, Customization>;
			using Fwd = Tree;
			PASTEL_FWD(Cursor);
			PASTEL_FWD(Point_ConstIterator);

			// The number of points in a node reported by 'cursor.points()'
			// must equal the distance between 'cursor.begin()' and 'cursor.end()'.
			if (REPORT_OP((std::distance(cursor.begin(), cursor.end())), !=, cursor.points()))
			{
				return false;
			}

			if (REPORT_OP(cursor.points(), <, 0))
			{
				return false;
			}

			// If a node is empty, then 'cursor.begin()' and 'cursor.end()' must
			// equal 'tree.end()'.
			if (REPORT(cursor.empty() && (cursor.begin() != tree.end() ||
				cursor.end() != tree.end())))
			{
				return false;
			}

			if (cursor.leaf())
			{
				// In a leaf node, the contained points must have that node
				// as the associated node.
				Point_ConstIterator iter = cursor.begin();
				Point_ConstIterator iterEnd = cursor.end();
				while(iter != iterEnd)
				{
					Cursor leaf = iter->leaf();
					if (REPORT(leaf != cursor))
					{
						return false;
					}
					++iter;
				}
			}
			else
			{
				if (REPORT(cursor.splitPosition() < bound.min()[cursor.splitAxis()]))
				{
					return false;
				}

				if (REPORT(cursor.splitPosition() > bound.max()[cursor.splitAxis()]))
				{
					return false;
				}

				/*
				if (REPORT(cursor.min() >= bound.min()[cursor.splitAxis()]))
				{
					return false;
				}

				if (REPORT(cursor.max() <= bound.max()[cursor.splitAxis()]))
				{
					return false;
				}
				*/

				if (REPORT(cursor.left().points() + cursor.right().points() !=
					cursor.points()))
				{
					return false;
				}

				if (REPORT(cursor.left().parent() != cursor))
				{
					return false;
				}

				if (REPORT(cursor.right().parent() != cursor))
				{
					return false;
				}

				if (!cursor.empty())
				{
					Point_ConstIterator begin = 
						!cursor.left().empty() ? cursor.left().begin() : cursor.right().begin();
					Point_ConstIterator end =
						!cursor.right().empty() ? cursor.right().end() : cursor.left().end();
					if (REPORT(cursor.begin() != begin || cursor.end() != end))
					{
						return false;
					}
				}

				AlignedBox<Real, N> rightBound(bound);
				rightBound.min()[cursor.splitAxis()] = cursor.splitPosition();

				if (!testInvariants(tree, cursor.right(), rightBound))
				{
					return false;
				}

				AlignedBox<Real, N> leftBound(bound);
				leftBound.max()[cursor.splitAxis()] = cursor.splitPosition();

				if (!testInvariants(tree, cursor.left(), leftBound))
				{
					return false;
				}
			}

			return true;
		}

	}

	template <typename Settings, template <typename> class Customization>
	bool testInvariants(const PointKdTree<Settings, Customization>& tree)
	{
		using Fwd = Settings;
		PASTEL_FWD(Locator);
		using Real = typename Locator::Real;
		static constexpr integer N = Locator::N;

		AlignedBox<Real, N> bound(
			Vector<Real, N>(ofDimension(tree.n()), -infinity<Real>()),
			Vector<Real, N>(ofDimension(tree.n()), infinity<Real>()));;
		
		return PointKdTree_::testInvariants(tree, tree.root(), bound);
	}

}

#endif
