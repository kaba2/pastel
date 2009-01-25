#ifndef PASTELGEOMETRY_GRID_HPP
#define PASTELGEOMETRY_GRID_HPP

#include "pastel/geometry/grid.h"

#include "pastel/sys/ensure.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/view_visit.h"
#include "pastel/sys/arrayview.h"
#include "pastel/sys/subview.h"
#include "pastel/sys/rectangle_tools.h"

namespace Pastel
{

	template <int N, typename Real, typename Object>
	Grid<N, Real, Object>::Grid()
		: bound_()
		, inverseBucketDimensions_()
		, cellArray_()
		, data_()
	{
	}

	template <int N, typename Real, typename Object>
	Grid<N, Real, Object>::Grid(
		const Grid& that)
		: data_()
		, cellArray_()
		, bound_(that.bound_)
		, inverseBucketDimensions_(that.inverseBucketDimensions_)
	{
		cellArray_.setExtent(
			that.extent()
			Cell(data_.end(), data_.end()));
		insert(that.begin(), that.end());
	}

	template <int N, typename Real, typename Object>
	Grid<N, Real, Object>::Grid(
		const AlignedBox<N, Real>& bound,
		const Vector<N, integer>& extent)
		: data_()
		, cellArray_()
		, bound_()
		, inverseBucketDimensions_()
	{
		ENSURE(allGreaterEqual(extent, 0));

		cellArray_.setExtent(extent,
			Cell(data_.end(), data_.end()));
	}

    template <int N, typename Real, typename Object>
	Grid<N, Real, Object>& Grid<N, Real, Object>::operator=(
		const Grid& that)
	{
		Grid copy(that);
		swap(copy);
		return *this;
	}

	template <int N, typename Real, typename Object>
	void Grid<N, Real, Object>::swap(Grid& that)
	{
		data_.swap(that.data_);
		cellArray_.swap(that.cellArray_);
		bound_.swap(that.bound_);
		inverseBucketDimensions_.swap(that.inverseBucketDimensions_);
	}

	template <int N, typename Real, typename Object>
	void Grid<N, Real, Object>::clear()
	{
		data_.clear();
        cellArray_.clear();
		bound_ = AlignedBox<N, Real>();
		inverseBucketDimensions_.set(0);
	}

	template <int N, typename Real, typename Object>
	integer Grid<N, Real, Object>::size() const
	{
		return data_.size();
	}

	template <int N, typename Real, typename Object>
	const Vector<N, integer>& Grid<N, Real, Object>::extent() const
	{
		return cellArray_.extent();
	}

	template <int N, typename Real, typename Object>
	const AlignedBox<N, Real>& Grid<N, Real, Object>::bound() const
	{
		return bound_;
	}

	template <int N, typename Real, typename Object>
	template <typename BoundFunctor>
	void Grid<N, Real, Object>::insert(
		const Object& object, 
		const BoundFunctor& boundFunctor)
	{
		insert(object, boundFunctor(object));
	}

	template <int N, typename Real, typename Object>
	void Grid<N, Real, Object>::insert(
		const Object& object, 
		const AlignedBox<N, Real>& objectBound)
	{
		const Rectangle<N> objectRegion = 
			nodesAt(objectBound);

		InsertFunctor insertFunctor(data_, object);
		visit(subView(arrayView(cellArray_), objectRegion), insertFunctor);
	}

	template <int N, typename Real, typename Object>
	template <typename InputIterator, typename BoundFunctor>
	void Grid<N, Real, Object>::insert(
		const InputIterator& from,
		const InputIterator& to,
		const BoundFunctor& boundFunctor)
	{
		InputIterator iter(from);
		while(iter != to)
		{
			insert(*iter, boundFunctor);
			++iter;
		}
	}

	template <int N, typename Real, typename Object>
	Point<N, integer> Grid<N, Real, Object>::nodeAt(const Point<N, Real>& point) const
	{
		const Vector<N, Real> delta(
			point - bound_.min());

		return Point<N, integer>(
			evaluate(floor(delta * inverseBucketDimensions_)));
	}

	template <int N, typename Real, typename Object>
	Rectangle<N> Grid<N, Real, Object>::nodesAt(
		const AlignedBox<N, Real>& region) const
	{
		const Rectangle<N> gridRegion(
			nodeAt(region.min()),
			nodeAt(region.max()) + 1);

		const Rectangle<N> validGridRegion(
			Point<N, integer>(0),
			Point<N, integer>(extent()));

		Rectangle<N> visibleRegion;
		intersect(gridRegion, validGridRegion, visibleRegion);

		return visibleRegion;
	}

	template <int N, typename Real, typename Object>
	typename Grid<N, Real, Object>::ConstObjectIterator
		Grid<N, Real, Object>::begin() const
	{
		return data_.begin();
	}

	template <int N, typename Real, typename Object>
	typename Grid<N, Real, Object>::ConstObjectIterator
		Grid<N, Real, Object>::end() const
	{
		return data_.end();
	}

	template <int N, typename Real, typename Object>
	typename Grid<N, Real, Object>::ConstObjectIterator
		Grid<N, Real, Object>::begin(const Point<N, integer>& index) const
	{
		if (anyLess(index, 0) ||
			anyGreaterEqual(asVector(index), extent()))
        {
            return data_.end();
        }

		const Cell& bucket = cellArray_(index);
		return bucket.first_;
	}

	template <int N, typename Real, typename Object>
	typename Grid<N, Real, Object>::ConstObjectIterator
		Grid<N, Real, Object>::end(const Point<N, integer>& index) const
	{
		if (anyLess(index, 0) ||
			anyGreaterEqual(asVector(index), extent()))
        {
            return data_.end();
        }

		const Cell& bucket = cellArray_(index);
		if (bucket.count_ == 0)
		{
			return data_.end();
		}

		ConstObjectIterator result(bucket.last_);
		++result;

		return result;
	}

}

#endif
