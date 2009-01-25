#ifndef PASTELMATH_NURBS_HPP
#define PASTELMATH_NURBS_HPP

#include "pastel/math/nurbs.h"

namespace Pastel
{

    template <int N, typename Real>
    Nurbs<N, Real>::Nurbs(integer degree)
        : pointList_()
        , weightList_()
        , knotList_()
        , degree_(degree)
    {
        ENSURE1(degree > 0, degree);
    }

    template <int N, typename Real>
    void Nurbs<N, Real>::swap(Nurbs& that)
    {
        pointList_.swap(that.pointList_);
        weightList_.swap(that.weightList_);
        knotList_.swap(that.knotList_);
        std::swap(degree_, that.degree_);
    }

    template <int N, typename Real>
    void Nurbs<N, Real>::clear()
    {
        pointList_.clear();
        weightList_.clear();
        knotList_.clear();
        degree_ = 0;
    }

    template <int N, typename Real>
    integer Nurbs<N, Real>::degree() const
    {
        return degree_;
    }

    template <int N, typename Real>
    integer Nurbs<N, Real>::knots() const
    {
        return knotList_.size();
    }

    template <int N, typename Real>
    void Nurbs<N, Real>::addControlPoint(
        const Real& knot, const Point<N, Real>& point,
        const Real& weight)
    {
        const integer index = std::upper_bound(

        pointList_.resize(points);
        weightList_.resize(points);
        knotList_.resize(points + degree + 1);
    }
    
    template <int N, typename Real>
    integer Nurbs<N, Real>::controlPoints() const
    {
        return pointList_.size();
    }

    template <int N, typename Real>
    Point<N, Real>& Nurbs<N, Real>::controlPoint(integer index)
    {
        return pointList_[index];
    }

    template <int N, typename Real>
    const Point<N, Real>& Nurbs<N, Real>::controlPoint(integer index) const
    {
        return pointList_[index];
    }

    template <int N, typename Real>
    Real& Nurbs<N, Real>::weight(integer index)
    {
        return weightList_[index];
    }

    template <int N, typename Real>
    const Real& Nurbs<N, Real>::weight(integer index) const
    {
        return weightList_[index];
    }

}

#endif
