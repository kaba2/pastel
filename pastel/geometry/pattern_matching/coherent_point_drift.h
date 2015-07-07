// Description: Coherent point drift algorithm.
// Documentation: coherent_point_drift.txt

#ifndef PASTELGEOMETRY_COHERENT_POINT_DRIFT_H
#define PASTELGEOMETRY_COHERENT_POINT_DRIFT_H

#include "pastel/geometry/pattern_matching/ls_affine.h"
#include "pastel/geometry/nearestset/nearestset_concept.h"
#include "pastel/sys/output/null_output.h"

namespace Pastel
{

    using Cpd_Matrix = LsAffine_Matrix;
    using Cpd_Scaling = LsAffine_Scaling;
    using Cpd_Translation = LsAffine_Translation;

    template <typename Real>
    struct Cpd_Return
    {
        arma::Mat<Real> Q;
        arma::Mat<Real> S;
        arma::Col<Real> t;
        Real sigma2;
    };

    template <typename Real>
    struct Cpd_State
    {
        arma::Mat<Real> Q;
        arma::Mat<Real> S;
        arma::Col<Real> t;
        Real sigma2;
        arma::Mat<Real> W;
    };

    //! Coherent point drift algorithm.
    /*!
    Preconditions:
    0 <= minIterations <= maxIterations
    0 < noiseRatio < 1
    n > 0
    m > 0
    d > 0

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
    Initialized with Q0.

    S ((d x d) real matrix):
    The estimated scaling; a symmetric matrix.
    Initialized with S0.

    t ((d x 1) real matrix):
    The estimated translation.
	Initialized with t0.

    sigma2 (Real):
    The estimated variance is eye(d, d) * sigma2.

    Optional input arguments
    ------------------------

    Q0 ((d x d) real matrix : arma::Mat<Real>()):
    Initial guess on Q; an orthogonal matrix. Empty matrix is
    interpreted as a (d x d) identity matrix.

    S0 ((d x d) real matrix : arma::Mat<Real>()):
    Initial guess on S; a symmetric matrix. Empty matrix is
    interpreted as a (d x d) identity matrix.

    t0 ((d x 1) real vector : arma::Col<Real>()): 
    Initial guess on t. Empty matrix is
    interpreted as a (d x 1) zero matrix.

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
        Diagonal: S is diagonal
        Conformal: S = sI
        Rigid: S = I

    translation (Cpd_Translation : Free):
    Specifies constraints for the translation t. 
        Free: no constraint
        Identity: T = 0

    orientation (integer : 1): 
    Specifies constraints for the determinant of A.
        <0: det(A) < 0,
         0: no constraint
        >0: det(A) > 0.

    minIterations (integer : 0):
    The minimum number of iterations for the algorithm to take.

    maxIterations (integer : std::max(minIterations, 100)):
    The maximum number of iterations for the algorithm to take. 

    minError (Real : see below):
    The minimum error under which to accept the transformation 
    and stop iteration. For float 1e-4; for double 1e-11.
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

        ENSURE_OP(d, >, 0);
        ENSURE_OP(m, >, 0);
        ENSURE_OP(n, >, 0);

        constexpr Real defaultMinError = 
            std::is_same<Real, float>::value ? 1e-4 : 1e-11;

        // Optional input arguments
        Real noiseRatio = 
            PASTEL_ARG_S(noiseRatio, 0.2);
        integer minIterations = 
            PASTEL_ARG_S(minIterations, 0);
        integer maxIterations = 
            PASTEL_ARG_S(maxIterations, std::max(minIterations, (integer)100));
        Real minError = 
            PASTEL_ARG_S(minError, defaultMinError);
        Cpd_Matrix matrix = 
            PASTEL_ARG_ENUM(matrix, Cpd_Matrix::Free);
        Cpd_Scaling scaling = 
            PASTEL_ARG_ENUM(scaling, Cpd_Scaling::Free);
        Cpd_Translation translation = 
            PASTEL_ARG_ENUM(translation, Cpd_Translation::Free);
        integer orientation = 
            PASTEL_ARG_S(orientation, (integer)1);
        arma::Mat<Real> Q = 
            PASTEL_ARG_S(Q0, arma::Mat<Real>());
        arma::Mat<Real> S = 
            PASTEL_ARG_S(S0, arma::Mat<Real>());
        arma::Col<Real> t = 
            PASTEL_ARG_S(t0, arma::Col<Real>());
		auto&& report =
			PASTEL_ARG_S(report, nullOutput());

        ENSURE(noiseRatio > 0);
        ENSURE(noiseRatio < 1);
        ENSURE_OP(minIterations, >=, 0);
        ENSURE_OP(minIterations, <=, maxIterations);

        if (Q.is_empty())
        {
            // The initial Q was not specified.
			
			// Reset the matrix, to clear a possible
			// replicated strict flag.
			Q.reset();

            // Use the identity matrix.
            Q.eye(d, d);
        }
 
        ENSURE_OP(Q.n_rows, ==, d);
        ENSURE_OP(Q.n_cols, ==, d);

        if (S.is_empty())
        {
            // The initial S was not specified.
			
			// Reset the matrix, to clear a possible
			// replicated strict flag.
			S.reset();

            // Use the identity matrix.
            S.eye(d, d);
        }

        ENSURE_OP(S.n_rows, ==, d);
        ENSURE_OP(S.n_cols, ==, d);

        if (t.is_empty())
        {
            // The initial t was not specified.
			
			// Reset the matrix, to clear a possible
			// replicated strict flag.
			t.reset();

			// Use the zero matrix.
			t.zeros(d);
        }

        ENSURE_OP(t.n_rows, ==, d);
        ENSURE_OP(t.n_cols, ==, 1);

        // We wish to preserve the memory storage
        // of Q, S, and t. Store the memory addresses
        // to check the preservation later.
        const Real* qPointer = Q.memptr();
        const Real* sPointer = S.memptr();
        const Real* tPointer = t.memptr();

        // Compute the transformed model-set according

        // to the initial guess.
        arma::Mat<Real> transformedSet = 
            Q * S * fromSet + t * arma::ones<arma::Mat<Real>>(1, m);

        // Returns the transformed set centered on toSet.col(j).
        // Note that it is important that we return by decltype(auto),
        // to capture the expression template.
        auto deltaSet = [&](integer j) -> decltype(auto)
        {
            return transformedSet - toSet.col(j) * arma::ones<arma::Mat<Real>>(1, m);
        };

        // Compute a constant to be used later.
        Real c = (noiseRatio / (1 - noiseRatio)) * ((Real)m / n);

        // Compute an initial estimate for sigma^2.
        Real sigma2 = 0;
        for (integer j = 0; j < n;++j)
        {
            sigma2 += arma::accu(arma::square(deltaSet(j)));
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
            if (sigma2 == 0)
            {
                // Having zero sigma^2 can happen at least with easy
                // cases where there is an exact solution; it is related
                // to fast convergence.
                break;
            }

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
                        -arma::sum(arma::square(deltaSet(j))) / (2 * sigma2)
                    );
                W.col(j) = expSet.t() / (arma::accu(expSet) + f);
            }

            // Store the previous transformation for comparison.
            qPrev = Q;
            sPrev = S;
            tPrev = t;

            // Compute a new estimate for the optimal transformation.
            auto lsMatch = lsAffine(
                fromSet, toSet,
                matrix,
                scaling,
                translation,
                PASTEL_TAG(orientation), orientation,
                PASTEL_TAG(W), W,
                // This avoids the reallocation of 
                // Q, S. and t.
                PASTEL_TAG(Q0), std::move(Q),
                PASTEL_TAG(S0), std::move(S),
                PASTEL_TAG(t0), std::move(t)
                );

            Q = std::move(lsMatch.Q);
            S = std::move(lsMatch.S);
            t = std::move(lsMatch.t);
 
            // Compute the transformed model-set.
            transformedSet = 
                Q * S * fromSet + t * arma::ones<arma::Mat<Real>>(1, m);

            // Compute a new estimate for sigma^2.
            sigma2 = 0;
            for (integer j = 0;j < n;++j)
            {
                sigma2 += 
                    arma::accu(
                        W.col(j).t() % arma::sum(arma::square(deltaSet(j))) 
                    );
            }
            sigma2 /= arma::accu(W) * d;

			// Report the current estimate.
			Cpd_State<Real> state = 
			{
				std::move(Q),
				std::move(S),
				std::move(t),
				sigma2,
                std::move(W)
			};

			report(addConst(state));

			Q = std::move(state.Q);
			S = std::move(state.S);
			t = std::move(state.t);
            W = std::move(state.W);

            real qError = arma::norm(qPrev - Q, "inf");
            real sError = arma::norm(sPrev - S, "inf");
            real tError = arma::norm(tPrev - t, "inf");

			if (std::max(std::max(qError, sError), tError) <= minError && 
                iteration + 1 >= minIterations)
            {
                // When the change to the previous transformation 
                // falls below the given error threshold, we will 
                // stop, provided that a minimum number of iterations
                // has been performed.
               break;
            }
        }

        // Make sure that memory was not reallocated.
        ASSERT(Q.memptr() == qPointer);
        unused(qPointer);

        ASSERT(S.memptr() == sPointer);
        unused(sPointer);

        ASSERT(t.memptr() == tPointer);
        unused(tPointer);

        return {std::move(Q), std::move(S), std::move(t), sigma2};
    }

}

#endif
