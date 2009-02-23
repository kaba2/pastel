#ifndef PASTELGEOMETRY_GRID_H
#define PASTELGEOMETRY_GRID_H

#include "pastel/sys/fastlist.h"
#include "pastel/sys/array.h"
#include "pastel/sys/keyvalue.h"
#include "pastel/sys/poolallocator.h"
#include "pastel/sys/rectangle.h"

#include "pastel/geometry/alignedbox.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	template <int N, typename Real, typename Object>
	class Grid
	{
	private:
		typedef FastList<Object, PoolAllocator> ObjectContainer;
		typedef typename ObjectContainer::iterator ObjectIterator;

		class Cell;

	public:
		enum
		{
			N_ = N
		};
		typedef Real Real_;
		typedef Object Object_;

		typedef typename ObjectContainer::const_iterator ConstObjectIterator;
		typedef typename Array<N, Cell>::ConstCursor ConstCellCursor;

		// Using default destructor.

		//! Constructs an empty grid.
		/*!
		Exception safety: strong
		Time complexity: constant
		*/
		Grid();

		//! Constructs a copy of another grid.
		/*!
		Exception safety: strong
		Time complexity: linear
		*/
		Grid(const Grid& that);

		//! Constructs a grid with the given dimensions.
		/*!
		Exception safety: strong
		Time complexity: constant
		*/
		Grid(
			const AlignedBox<N, Real>& bound,
			const Vector<N, integer>& extent);

		//! Copies from a another grid.
		/*!
		Exception safety: strong
		Time complexity: linear
		*/
		Grid<N, Real, Object>& operator=(const Grid& that);

		//! Swaps two grids.
		/*!
		Exception safety: nothrow
		Time complexity: constant
		*/
		void swap(Grid& that);

		//! Removes all points from the grid.
		/*!
		Exception safety: nothrow
		Time complexity: constant
		*/
		void clear();

		//! Returns the number of points in the grid.
		/*!
		Exception safety: nothrow
		Time complexity: constant
		*/
		integer size() const;

		//! Returns the extent of the underlying grid.
		/*!
		Exception safety: nothrow
		Time complexity: constant
		*/
		const Vector<N, integer>& extent() const;

		//! Returns the bounding box of the grid.
		/*!
		Exception safety: nothrow
		Time complexity: constant
		*/
		const AlignedBox<N, Real>& bound() const;

		//! Inserts an object into the grid.
		/*!
		Exception safety: strong
		Time complexity: constant
		*/
		template <typename BoundFunctor>
		void insert(
			const Object& object,
			const BoundFunctor& boundFunctor);

		//! Inserts an object into the grid.
		/*!
		Exception safety: strong
		Time complexity: constant
		*/
		void insert(
			const Object& object,
			const AlignedBox<N, Real>& objectBound);

		//! Inserts the points in the grid.
		/*!
		Exception safety: basic
		Time complexity: constant
		*/
		template <typename InputIterator, typename BoundFunctor>
		void insert(
			const InputIterator& from,
			const InputIterator& to,
			const BoundFunctor& boundFunctor);

		//! Returns the coordinates of the containing grid voxel.
		/*!
		Exception safety: nothrow
		Time complexity: constant
		*/
		Point<N, integer> nodeAt(const Point<N, Real>& point) const;

		Rectangle<N> nodesAt(const AlignedBox<N, Real>& region) const;

		ConstObjectIterator begin() const;
		ConstObjectIterator end() const;

		ConstObjectIterator begin(const Point<N, integer>& index) const;
		ConstObjectIterator end(const Point<N, integer>& index) const;

	private:
		class Cell
		{
		public:
			friend class InsertFunctor;

			Cell()
				: first_()
				, last_()
				, count_(0)
			{
			}

			/*
			Cell(const ObjectIterator& begin,
				const ObjectIterator& end)
				: begin_(begin)
				, end_(end)
				, count_(0)
			{
			}
			*/

			ObjectIterator first_;
			ObjectIterator last_;
			integer count_;
		};

		class InsertFunctor
		{
		public:
			explicit InsertFunctor(
				ObjectContainer& data,
				Object const& object)
				: data_(data)
				, object_(object)
			{
			}

			void operator()(Cell& cell) const
			{
				ObjectIterator end_ = cell.last_;

				if (cell.count_ == 0)
				{
					cell.last_ = data_.insert(end_, object);
					cell.first_ = cell.last_;
				}
				else
				{
					++end_;
					cell.last_ = data_.insert(end_, object);
				}

				++cell.count_;
			}

		private:
			ObjectContainer& data_;
			Object const& object_;
		};

		ObjectContainer data_;
		Array<N, Cell> cellArray_;
		AlignedBox<N, Real> bound_;
		Vector<N, Real> inverseBucketDimensions_;
	};

}

#include "pastel/geometry/grid.hpp"

#endif
