// Description: Polygon class

#ifndef PASTELGEOMETRY_POLYGON_H
#define PASTELGEOMETRY_POLYGON_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/tuple.h"
#include "pastel/sys/vector.h"

#include <vector>

namespace Pastel
{

	template <typename Real, int N = Dynamic>
	class Polygon
	{
	public:
		using Real_ = Real;
		static constexpr int N_ = N;

		// Using default constructor.
		// Using default copy constructor.
		// Using default destructor.

		Polygon<Real, N>& operator=(
			const Polygon& that);

		Vector<Real, N>& operator[](integer index);
		const Vector<Real, N>& operator[](integer index) const;

		void swap(Polygon& that);

		integer size() const;

		void pushBack(const Vector<Real, N>& that);
		void popBack();

		void insert(integer index, const Vector<Real, N>& that);
		void erase(integer index);

		void reserve(integer size);

	private:
		std::vector<Vector<Real, N> > data_;
	};

	using Polygon2 = Polygon<dreal, 2>;
	using Polygon3 = Polygon<dreal, 3>;
	using Polygon4 = Polygon<dreal, 4>;

}

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
	Vector<Real, N>&
		Polygon<Real, N>::operator[](integer index)
	{
		PENSURE2(index >= 0 && index < data_.size(),
			index, data_.size());

		return data_[index];
	}

	template <typename Real, int N>
	const Vector<Real, N>&
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
		const Vector<Real, N>& that)
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
		integer index, const Vector<Real, N>& that)
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
