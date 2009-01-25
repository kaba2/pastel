#ifndef PASTELMATH_NURBS_H
#define PASTELMATH_NURBS_H

#include "pastel/sys/point.h"

#include "pastel/sys/smallmap.h"

#include <vector>

namespace Pastel
{

    template <int N, typename Real>
    class Nurbs
    {
    public:
        // Using default copy constructor.
        // Using default assignment.
        // Using default destructor.

        explicit Nurbs(integer degree = 3);

        void swap(Nurbs& that);
        void clear();

        integer degree() const;
        integer knots() const;

        void setControlPoints(integer points);
        integer controlPoints() const;

        Point<N, Real>& controlPoint(integer index);
        const Point<N, Real>& controlPoint(integer index) const;

        Real& weight(integer index);
        const Real& weight(integer index) const;

    private:
        std::vector<Point<N, Real> > pointList_;
        std::vector<Real> weightList_;
        std::vector<Real> knotList_;
        integer degree_;
    };

}

#include "pastel/math/nurbs.hpp"

#endif
