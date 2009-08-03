#ifndef PASTEL_POLYGON_HPP
#define PASTEL_POLYGON_HPP

#include "pastel/geometry/polygon.h"

#include "pastel/sys/ensure.h"

namespace Pastel
{

	// Polygon

	template <typename Real, int N>
	Polygon<Real, N>& Polygon<Real, N>::operator=(
		const Polygon& that)
	{
		Polygon copy(that);
		swap(copy);
		return *this;
	}

	template <typename Real, int N>
	Point<Real, N>&
		Polygon<Real, N>::operator[](integer index)
	{
		PENSURE2(index >= 0 && index < data_.size(),
			index, data_.size());

		return data_[index];
	}

	template <typename Real, int N>
	const Point<Real, N>&
		Polygon<Real, N>::operator[](integer index) const
	{
		PENSURE2(index >= 0 && index < data_.size(),
			index, data_.size());

		return data_[index];
	}

	template <typename Real, int N>
	void Polygon<Real, N>::swap(Polygon& that)
	{
		data_.swap(that.data_);
	}

	template <typename Real, int N>
	integer Polygon<Real, N>::size() const
	{
		return data_.size();
	}

	template <typename Real, int N>
	void Polygon<Real, N>::pushBack(
		const Point<Real, N>& that)
	{
		data_.push_back(that);
	}

	template <typename Real, int N>
	void Polygon<Real, N>::popBack()
	{
		PENSURE(!data_.empty());
		data_.pop_back();
	}

	template <typename Real, int N>
	void Polygon<Real, N>::insert(
		integer index, const Point<Real, N>& that)
	{
		PENSURE2(index >= 0 && index <= data_.size(),
			index, data_.size());
		data_.insert(
			data_.begin() + index, that);
	}

	template <typename Real, int N>
	void Polygon<Real, N>::erase(
		integer index)
	{
		PENSURE2(index >= 0 && index < data_.size(),
			index, data_.size());

		data_.erase(
			data_.begin() + index);
	}

	template <typename Real, int N>
	void Polygon<Real, N>::reserve(
		integer size)
	{
		data_.reserve(size);
	}

}

#endif
