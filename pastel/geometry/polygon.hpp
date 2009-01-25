#ifndef PASTELGEOMETRY_POLYGON_HPP
#define PASTELGEOMETRY_POLYGON_HPP

#include "pastel/geometry/polygon.h"

#include "pastel/sys/ensure.h"

namespace Pastel
{

	// Polygon

	template <int N, typename Real>
	Polygon<N, Real>& Polygon<N, Real>::operator=(
		const Polygon& that)
	{
		Polygon copy(that);
		swap(copy);
		return *this;
	}

	template <int N, typename Real>
	Point<N, Real>&
		Polygon<N, Real>::operator[](integer index)
	{
		PENSURE2(index >= 0 && index < data_.size(),
			index, data_.size());

		return data_[index];
	}

	template <int N, typename Real>
	const Point<N, Real>&
		Polygon<N, Real>::operator[](integer index) const
	{
		PENSURE2(index >= 0 && index < data_.size(),
			index, data_.size());

		return data_[index];
	}

	template <int N, typename Real>
	void Polygon<N, Real>::swap(Polygon& that)
	{
		data_.swap(that.data_);
	}

	template <int N, typename Real>
	integer Polygon<N, Real>::size() const
	{
		return data_.size();
	}

	template <int N, typename Real>
	void Polygon<N, Real>::pushBack(
		const Point<N, Real>& that)
	{
		data_.push_back(that);
	}

	template <int N, typename Real>
	void Polygon<N, Real>::popBack()
	{
		PENSURE(!data_.empty());
		data_.pop_back();
	}

	template <int N, typename Real>
	void Polygon<N, Real>::insert(
		integer index, const Point<N, Real>& that)
	{
		PENSURE2(index >= 0 && index <= data_.size(),
			index, data_.size());
		data_.insert(
			data_.begin() + index, that);
	}

	template <int N, typename Real>
	void Polygon<N, Real>::erase(
		integer index)
	{
		PENSURE2(index >= 0 && index < data_.size(),
			index, data_.size());

		data_.erase(
			data_.begin() + index);
	}

	template <int N, typename Real>
	void Polygon<N, Real>::reserve(
		integer size)
	{
		data_.reserve(size);
	}

}

#endif
