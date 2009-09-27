// Description: All-nearest-neighbors counting in 1D
// Detail: Implements a very fast algorithm that works only in 1D.

#ifndef PASTEL_COUNT_ALL_NEIGHBORS_1D_H
#define PASTEL_COUNT_ALL_NEIGHBORS_1D_H

#include "pastel/sys/vector.h"

#include <vector>

namespace Pastel
{

	//! Counts the number of neighbors for all query points.
	/*!
	pointSet:
	A set of points.

	indexBegin, indexEnd:
	A sequence of n integers denoting the indices of query points
	in 'pointSet' for which the neighbors are counted for.
	Note: The special iterator CountingIterator is memory-
	efficient if the index set consists of an integer interval.
	See 'pastel/sys/iterators.txt'. The iterators must be
	random-access iterators.
	
	maxDistanceBegin:
	A sequence of n reals denoting the maximum distance inside
	which points are considered neighbors. Note:
	the special iterator ConstantIterator is memory-efficient 
	if the maximum distance is the same for all query points.
	See 'pastel/sys/iterators.txt'. The iterator must be a
	random-access iterator.
	
	normBijection:
	Defines the distance that is used. 
	See 'pastel/math/normbijection.txt'.

	neighborsBegin (output):
	A sequence of n integers, the i:th member denoting the number 
	of neighbors 'pointSet[indexSetBegin[i]]' has. The iterator 
	must be a random-access iterator and writable.
	*/
	template <typename Real, int N, typename NormBijection,
	typename ConstIndexIterator, typename ConstDistanceIterator,
	typename CountIterator>
	void countAllNeighbors1d(
		const std::vector<Vector<Real, N> >& pointSet,
		const ConstIndexIterator& indexBegin,
		const ConstIndexIterator& indexEnd,
		const ConstDistanceIterator& maxDistanceBegin,
		const NormBijection& normBijection,
		const CountIterator& neighborsBegin);

}

#include "pastel/geometry/count_all_neighbors_1d.hpp"

#endif
