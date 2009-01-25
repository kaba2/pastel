#ifndef PASTELSYS_POINT_HPP
#define PASTELSYS_POINT_HPP

#include "pastel/sys/point.h"

namespace Pastel
{

    template <typename Real>
    class Point<Unbounded, Real>
        : boost::equality_comparable<
        Point<Unbounded, Real> >
    {
    private:
        enum
        {
            N = Unbounded
        };

    public:
        //! Constructs with all elements set to zero.
        Point()
            : data_(0)
        {
        }

        //! Constructs with the given point.
        Point(const Point& that)
            : data_(that.data_)
        {
        }

        //! Constructs with all elements set to the given value.
        explicit Point(const Real& that)
            : data_(that)
        {
        }

        //! Constructs from a vector.
        template <typename Expression>
        explicit Point(const VectorExpression
            <N, Real, Expression>& that)
            : data_(that)
        {
        }

        ~Point()
        {
        }

        operator Tuple<N, Real>&()
        {
            return asVector();
        }

        operator const Tuple<N, Real>&() const
        {
            return asVector();
        }

        void setSize(integer size)
        {
            ENSURE1(size >= 0, size);
            data_.setSize(size);
        }

        integer size() const
        {
            return data_.size();
        }

        //! Swaps two points.
        void swap(Point& that)
        {
            data_.swap(that.data_);
        }

        void clear()
        {
            data_.clear();
        }

        //! Sets all of the elements to the given value.
        void set(const Real& that)
        {
            data_.set(that);
        }

        bool operator==(const Point& that) const
        {
            return data_ == that.data_;
        }

        //! Returns the index:th element.
        Real& operator[](integer index)
        {
            return data_[index];
        }

        //! Returns the index:th element.
        const Real& operator[](integer index) const
        {
            return data_[index];
        }

        //! Adds the given value to all elements.
        Point<N, Real>& operator+=(const Real& that)
        {
            data_ += that;
            return *this;
        }

        //! Returns the point added by 'that' to all elements.
        Point<N, Real> operator+(const Real& that) const
        {
            Point result(*this);
            result += that;
            return result;
        }

        //! Subtracts the given value from all elements.
        Point<N, Real>& operator-=(const Real& that)
        {
            data_ -= that;
            return (Point&)*this;
        }

        //! Returns the point subtracted by 'that' from all elements.
        Point<N, Real> operator-(const Real& that) const
        {
            Point result(*this);
            result -= that;
            return result;
        }

        //! Translates the point by 'that'.
        template <typename Expression>
        Point<N, Real>& operator+=(
            const VectorExpression
            <N, Real, Expression>& that)
        {
            data_ += that;

            return *this;
        }

        //! Translates the point backwards by 'that'.
        template <typename Expression>
        Point<N, Real>& operator-=(
            const VectorExpression
            <N, Real, Expression>& that)
        {
            data_ -= that;

            return *this;
        }

        //! Returns the difference vector between two points.
        Vector<N, Real> operator-(
            const Point& that) const
        {
            return data_ - that.data_;
        }

        //! Returns the point translated by 'that'.
        template <typename Expression>
        Point<N, Real> operator+(
            const VectorExpression
            <N, Real, Expression>& that) const
        {
            Point result(*this);
            result += that;
            return result;
        }

        //! Returns the point translated backwards by 'that'.
        template <typename Expression>
        Point<N, Real> operator-(
            const VectorExpression
            <N, Real, Expression>& that) const
        {
            Point result(*this);
            result -= that;
            return result;
        }

        //! Interprets the point as a position vector.
        Vector<N, Real>& asVector()
        {
            return data_;
        }

        //! Interprets the point as a position vector.
        const Vector<N, Real>& asVector() const
        {
            return data_;
        }

    private:
        Vector<N, Real> data_;
    };

    template <int N, typename Real>
    void swap(Point<N, Real>& left,
        Point<N, Real>& right)
    {
        std::swap(left, right);
    }

	template <int N, typename Real, typename Expression>
	Point<N, Real> asPoint(const VectorExpression<N, Real, Expression>& that)
	{
		return Point<N, Real>(that);
	}

    template <int N, typename Real>
    Vector<N, Real>& asVector(Point<N, Real>& that)
    {
        return that.asVector();
    }

    template <int N, typename Real>
    const Vector<N, Real>& asVector(const Point<N, Real>& that)
    {
        return that.asVector();
    }

    template <int N, typename Real>
    Tuple<N, Real>& asTuple(Point<N, Real>& that)
    {
        return that.asVector().asTuple();
    }

    template <int N, typename Real>
    const Tuple<N, Real>& asTuple(const Point<N, Real>& that)
    {
        return that.asVector().asTuple();
    }

}

#endif
