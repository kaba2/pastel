// Description: Coherent point drift algorithm.
// Documentation: coherent_point_drift.txt

#ifndef PASTELGEOMETRY_COHERENT_POINT_DRIFT_H
#define PASTELGEOMETRY_COHERENT_POINT_DRIFT_H

#include "pastel/geometry/pattern_matching/ls_affine.h"
#include "pastel/geometry/nearestset/nearestset_concept.h"

namespace Pastel
{

    using Cpd_Matrix = LsAffine_Matrix;
    using Cpd_Scaling = LsAffine_Scaling;
    using Cpd_Translation = LsAffine_Translation;

    template <typename Real>
    using Cpd_Return = LsAffine_Return<Real>;

    //! Coherent point drift algorithm.
    /*!
    Preconditions:
    0 <= minIterations <= maxIterations
    0 < noiseRatio < 1

    Finds matrices Q, S, and t such that

        Q * S * fromSet + t * ones(1, n)

    matches toSet.

    Input
    -----

    fromSet ((d x n) real matrix):
    A set of n points, given as a matrix, where each column
    contains the coordinates of a d-dimensional point. 

    toSet ((d x m) real matrix):
    A set of m points, given as a matrix, where each column
    contains the coordinates of a d-dimensional point.

    Returns
    -------

    Q ((d x d) real matrix):
    The estimated rotation/reflection; an orthogonal matrix.
    Will use the memory space of Q0, if Q0 is std::moved in.

    S ((d x d) real matrix):
    The estimated scaling; a symmetric matrix.
    Will use the memory space of S0, if S0 is std::moved in.

    t ((d x 1) real matrix):
    The estimated translation.
    Will use the memory space of t0, if t0 is std::moved in.

    Optional input arguments
    ------------------------

    Q0 ((d x d) real matrix : arma::eye(d, d)):
    Initial guess on Q; an orthogonal matrix.

    S0 ((d x d) real matrix : arma::eye(d, d)):
    Initial guess on S; a symmetric matrix.

    t0 ((d x 1) real vector : see below): 
    Initial guess on t. Default: s - Q0 * S0 * m, where 's' and 'm' are
    the centroids of the scene and model point-sets, respectively. This
    is the optimal translation assuming 'toSet' and 'fromSet' match
    bijectively and Q0 = Q, and S0 = S.

    noiseRatio (Real : 0.2):
    A real number between (0, 1), which gives the weight for an 
    additive improper uniform distribution component for the Gaussian 
    mixture model. Larger noise-ratio makes the algorithm more 
    tolerant to noise, but declines convergence rate when the actual 
    noise level is lower.

    matrix (Cpd_Matrix : Free):
    Specifies constraints for the matrix Q.
        Free: Q^T Q = I
        Identity: Q = I

    scaling (Cpd_Scaling : Free):
    Specifies constraints for the scaling S.
        Free: S^T = S
        Conformal: S = sI
        Rigid: S = I

    translation (Cpd_Translation : Free):
    Specifies constraints for the translation t. 
        Free: no constraint
        Identity: T = 0

    orientation (integer : 0): 
    Specifies constraints for the determinant of A.
        <0: det(A) < 0,
         0: no constraint
        >0: det(A) > 0.

    minIterations (integer : 0):
    The minimum number of iterations for the algorithm to take.

    maxIterations (integer : std::max(minIterations, 100)):
    The maximum number of iterations for the algorithm to take. 

    minError (Real : 1e-11):
    The minimum error under which to accept the transformation 
    and stop iteration.
    */
    template <
		typename Real,
        typename... ArgumentSet
    >
    Cpd_Return<Real> coherentPointDrift(
        const arma::Mat<Real>& fromSet, 
        const arma::Mat<Real>& toSet,
        ArgumentSet&&... argumentSet)
    {
        // Point Set Registration: Coherent Point Drift,
        // Andriy Myronenko, Xubo Song,
        // IEEE Transactions on Pattern Analysis and Machine Intelligence,
        // Volume 32, Number 12, December 2010.

        ENSURE_OP(fromSet.n_rows, ==, toSet.n_rows);

        integer d = toSet.n_rows;
        integer m = fromSet.n_cols;
        integer n = toSet.n_cols;

        // Optional input arguments
        Real noiseRatio = 
            PASTEL_ARG_S(noiseRatio, 0.2);
        integer minIterations = 
            PASTEL_ARG_S(minIterations, 0);
        integer maxIterations = 
            PASTEL_ARG_S(maxIterations, std::max(minIterations, (integer)100));
        Real minError = 
            PASTEL_ARG_S(minError, 1e-11);
        Cpd_Matrix matrix = 
            PASTEL_ARG_S(matrix, Cpd_Matrix::Free);
        Cpd_Scaling scaling = 
            PASTEL_ARG_S(scaling, Cpd_Scaling::Free);
        Cpd_Translation translation = 
            PASTEL_ARG_S(translation, Cpd_Translation::Free);
        integer orientation = 
            PASTEL_ARG_S(orientation, (integer)0);
        // The Q0, S0, and t0 are deliberately
        // captured by value; we std::move
        // them later.
        auto Q0 = 
            PASTEL_ARG_S(Q0, arma::Mat<Real>());
        auto S0 = 
            PASTEL_ARG_S(S0, arma::Mat<Real>());
        auto t0 = 
            PASTEL_ARG_S(t0, arma::Col<Real>());

        if (Q0.is_empty())
        {
            Q0 = arma::eye<arma::Mat<Real>>(d, d);
        }

        if (S0.is_empty())
        {
            S0 = arma::eye<arma::Mat<Real>>(d, d);
        }

        ENSURE_OP(Q0.n_rows, ==, d);
        ENSURE_OP(Q0.n_cols, ==, d);
        ENSURE_OP(S0.n_rows, ==, d);
        ENSURE_OP(S0.n_cols, ==, d);
        ENSURE(noiseRatio > 0);
        ENSURE(noiseRatio < 1);
        ENSURE_OP(minIterations, >=, 0);
        ENSURE_OP(minIterations, <=, maxIterations);

        if (t0.is_empty())
        {
            // Compute centroids for both point-sets.
            arma::Col<Real> modelCentroid = 
                arma::sum(fromSet, 1) / m;
            arma::Col<Real> sceneCentroid = 
                arma::sum(toSet, 1) / n;
            
            t0 = sceneCentroid - Q0 * S0 * modelCentroid;
        }

        ENSURE_OP(t0.n_rows, ==, d);
        ENSURE_OP(t0.n_cols, ==, 1);

        // Store the memory pointer into Q0.
        auto* q0Pointer = Q0.memptr();

        // Set the initial guesses.
        arma::Mat<Real> Q = std::move(Q0);
        arma::Mat<Real> S = std::move(S0);
        arma::Col<Real> t = std::move(t0);

        // Compute the transformed model-set according
        // to the initial guess.
        arma::Mat<Real> transformedSet = Q * S * fromSet + t * arma::ones<arma::Mat<Real>>(1, n);

        // Compute a constant to be used later.
        Real c = (noiseRatio / (1 - noiseRatio)) * ((Real)m / n);

        // Compute an initial estimate for sigma^2.
        Real sigma2 = 0;
        for (integer j = 0; j < n;++j)
        {
            sigma2 += arma::accu(arma::square(
                transformedSet - toSet.col(j) * arma::ones<arma::Mat<Real>>(1, n)));
        }
        sigma2 = sigma2 / (d * m * n);

        // The weighting matrix will be computed here.
        arma::Mat<Real> W(m, n);

        // These will be used as temporary space for
        // computing the weighting matrix.
        arma::Row<Real> expSet(m);

        // These will store the previous estimate.
        arma::Mat<Real> qPrev(d, d);
        arma::Mat<Real> sPrev(d, d);
        arma::Col<Real> tPrev(d);

        for (integer iteration = 0; iteration < maxIterations; ++iteration)
        {
            // Compute a constant for the improper uniform distribution.
            // Note that this is dependent on 'sigma2', which is being
            // updated at each iteration; this cannot be moved out of
            // the loop.
            Real f = std::pow(2 * constantPi<Real>() * sigma2, (Real)d / 2) * c;

            // Compute the weighting matrix.
            for (integer j = 0;j < n;++j)
            {
                expSet = 
                    arma::exp(
                        -arma::sum(
                            arma::square(
                                transformedSet - toSet.col(j) * arma::ones<arma::Mat<Real>>(1, n)
                            )
                        )
                        / (2 * sigma2)
                    );
                W.col(j) = expSet / (arma::accu(expSet) + f);
            }

            // Store the previous transformation for comparison.
            qPrev.swap(Q);
            sPrev.swap(S);
            tPrev.swap(t);

            // Compute a new estimate for the optimal transformation.
            auto lsMatch = lsAffine(
                fromSet, toSet,
                PASTEL_TAG(matrix), matrix,
                PASTEL_TAG(scaling), scaling,
                PASTEL_TAG(translation), translation,
                PASTEL_TAG(orientation), orientation,
                PASTEL_TAG(W), W,
                // This avoids the reallocation of 
                // Q, S. and t.
                PASTEL_TAG(Q), std::move(Q),
                PASTEL_TAG(S), std::move(S),
                PASTEL_TAG(t), std::move(t)
                );

            Q = std::move(lsMatch.Q);
            S = std::move(lsMatch.S);
            t = std::move(lsMatch.t);
            
            // Compute the transformed model-set.
            transformedSet = 
                Q * S * fromSet + t * arma::ones<arma::Mat<Real>>(1, n);

            // Compute a new estimate for sigma^2.
            sigma2 = 0;
            for (integer j = 0;j < n;++j)
            {
                sigma2 += 
                    arma::accu(
                        W.col(j).t() %
                        arma::sum(
                            arma::square(
                                transformedSet - toSet.col(j) * arma::ones<arma::Mat<Real>>(1, n)
                            )
                        ) 
                    );
            }
            sigma2 /= arma::accu(W) * d;

            if (arma::norm(qPrev - Q, "fro") < minError && 
                arma::norm(sPrev - S, "fro") < minError &&
                arma::norm(tPrev - t, "fro") < minError &&
                iteration >= minIterations - 1)
            {
                // When the change to the previous transformation 
                // falls below the given error threshold, we will 
                // stop, provided that a minimum number of iterations
                // has been performed.
               break;
            }
        }

        ENSURE(qPrev.memptr() == q0Pointer ||
            Q.memptr() == q0Pointer);

        if (Q.memptr() != q0Pointer)
        {
            // We want to store the result in the
            // memory space of the original Q0, S0
            // and t0. This is so that the user
            // gets back the passed matrices.

            qPrev.swap(Q);
            sPrev.swap(S);
            tPrev.swap(t);

            Q = qPrev;
            S = sPrev;
            t = tPrev;
        }

        return {std::move(Q), std::move(S), std::move(t)};
    }

}

#endif
