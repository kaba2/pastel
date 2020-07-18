// Description: Coherent point drift algorithm.
// Documentation: coherent_point_drift.txt

#ifndef PASTELGEOMETRY_COHERENT_POINT_DRIFT_H
#define PASTELGEOMETRY_COHERENT_POINT_DRIFT_H

#include "pastel/geometry/pattern_matching/ls_affine.h"
#include "pastel/geometry/nearestset/nearestset_concept.h"
#include "pastel/sys/output/null_output.h"
#include "pastel/sys/math/constants.h"

namespace Pastel
{

    using Cpd_Matrix = LsAffine_Matrix;
    using Cpd_Scaling = LsAffine_Scaling;
    using Cpd_Translation = LsAffine_Translation;

    template <typename Real>
    struct Cpd_State
    {
        MatrixView<Real> Q;
        MatrixView<Real> S;
        MatrixView<Real> t;
        Real sigma2;
        MatrixView<Real> W;
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

        (Q * S * fromSet).colwise() + t

    matches toSet.

    Input
    -----

    fromSet ((d x n) dreal matrix):
    A set of n points, given as a matrix, where each column
    contains the coordinates of a d-dimensional point. 

    toSet ((d x m) dreal matrix):
    A set of m points, given as a matrix, where each column
    contains the coordinates of a d-dimensional point.

    Qs ((d x d) real matrix):
    Initial guess on Q and storage for the solution; an orthogonal matrix. 
    Use identity matrix if no better initial guess.

    Ss ((d x d) real matrix):
    Initial guess on S and storage for the solution; a symmetric matrix. 
    Use identity matrix if no better initial guess.
    
    ts ((d x 1) real vector): 
    Initial guess on t and storage for the solution; a column matrix. 
    Use zero vector if no better initial guess.

    Returns
    -------

    sigma2 (Real):
    The estimated variance is Identity(d, d) * sigma2.

    Optional input arguments
    ------------------------

    initialize (bool : true):
    Whether to initialize the matrices to defaults:
    Q = identity
    S = identity 
    t = zero
    If false, the passed matrices are used instead.

    noiseRatio (Real : 0.2):
    A dreal number between (0, 1), which gives the weight for an 
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
		typename Real_from, int M_from, int N_from,
		typename Real_to, int M_to, int N_to,
		typename Real_Q, int M_Q, int N_Q,
		typename Real_S, int M_S, int N_S,
		typename Real_t, int M_t,
		typename... ArgumentSet
    >
    requires 
        IsPlain<Real_Q> &&
        IsPlain<Real_S> && 
        IsPlain<Real_t> && 
        IsSameObject<Real_from, Real_to, Real_Q, Real_S, Real_t>
    Real_from coherentPointDrift(
		const MatrixView<Real_from, M_from, N_from>& fromSet,
		const MatrixView<Real_to, M_to, N_to>& toSet,
		const MatrixView<Real_Q, M_Q, N_Q>& Qs,
		const MatrixView<Real_S, M_S, N_S>& Ss,
		const ColMatrixView<Real_t, M_t>& ts,
        ArgumentSet&&... argumentSet)
    {
        // Point Set Registration: Coherent Point Drift,
        // Andriy Myronenko, Xubo Song,
        // IEEE Transactions on Pattern Analysis and Machine Intelligence,
        // Volume 32, Number 12, December 2010.

        ENSURE_OP(fromSet.rows(), ==, toSet.rows());

        using Real = Real_from;

		constexpr const int D = Common_Dimension<M_from, M_to, M_Q, M_S, M_t, N_Q, N_S>;

		MapMatrix<Real, D, N_from> P = asMatrix(fromSet);
		MapMatrix<Real, D, N_to> R = asMatrix(toSet);

        integer d = P.rows();
        integer m = P.cols();
        integer n = R.cols();

        ENSURE_OP(d, >, 0);
        ENSURE_OP(m, >, 0);
        ENSURE_OP(n, >, 0);
        
        ENSURE_OP(Qs.rows(), ==, d);
        ENSURE_OP(Qs.cols(), ==, d);
        ENSURE_OP(Ss.rows(), ==, d);
        ENSURE_OP(Ss.cols(), ==, d);
        ENSURE_OP(ts.rows(), ==, d);
        ENSURE_OP(ts.cols(), ==, 1);

        constexpr Real defaultMinError = 
            std::is_same<Real, float>::value ? 1e-4 : 1e-11;

        // Optional input arguments
        bool initialize =
            PASTEL_ARG_S(initialize, true);
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
		auto&& report =
			PASTEL_ARG_S(report, nullOutput());

        ENSURE(noiseRatio > 0);
        ENSURE(noiseRatio < 1);
        ENSURE_OP(minIterations, >=, 0);
        ENSURE_OP(minIterations, <=, maxIterations);

        MapMatrix<Real, D, D> Q = asMatrix(Qs);
        MapMatrix<Real, D, D> S = asMatrix(Ss);
        MapColMatrix<Real, D> t = asMatrix(ts);

        if (initialize) {
            // Initialize Q, S, and t.
            Q = Matrix<Real, D, D>::Identity(d, d);
            S = Matrix<Real, D, D>::Identity(d, d);
            t = ColMatrix<Real, D>::Zero(d, 1);
        }

        // Compute the transformed model-set according
        // to the initial guess.
        Matrix<Real, D, N_from> transformedSet = 
            (Q * S * P).colwise() + t;

        // Returns the transformed set centered on toSet.col(j).
        // Note that it is important that we return by decltype(auto),
        // to capture the expression template.
        auto deltaSet = [&](integer j) -> decltype(auto)
        {
            return transformedSet.colwise() - R.col(j);
        };

        // Compute a constant to be used later.
        Real c = (noiseRatio / (1 - noiseRatio)) * ((Real)m / n);

        // Compute an initial estimate for sigma^2.
        Real sigma2 = 0;
        for (integer j = 0; j < n;++j)
        {
            sigma2 += deltaSet(j).array().square().sum();
        }
        sigma2 = sigma2 / (d * m * n);

        // The weighting matrix will be computed here.
        Matrix<Real, N_from, N_to> W(m, n);
        MatrixView<Real, N_from, N_to> Ws = view(W);

        // These will be used as temporary space for
        // computing the weighting matrix.
        RowMatrix<Real, D> expSet(m);

        // These will store the previous estimate.
        Matrix<Real, D, D> qPrev(d, d);
        Matrix<Real, D, D> sPrev(d, d);
        ColMatrix<Real, D> tPrev(d, 1);

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
                expSet = (deltaSet(j).array().square().colwise().sum() / (-2 * sigma2)).exp();
                W.col(j) = expSet.transpose() / (expSet.sum() + f);
            }

            // Store the previous transformation for comparison.
            qPrev = Q;
            sPrev = S;
            tPrev = t;

            // Compute a new estimate for the optimal transformation.
            lsAffine(
                fromSet, toSet,
                Qs, Ss, ts,
                PASTEL_TAG(matrix), matrix,
                PASTEL_TAG(scaling), scaling,
                PASTEL_TAG(translation), translation,
                PASTEL_TAG(orientation), orientation,
                PASTEL_TAG(W), Ws
                );

            // Compute the transformed model-set.
            transformedSet = 
                (Q * S * P).colwise() + t;

            // Compute a new estimate for sigma^2.
            sigma2 = 0;
            for (integer j = 0;j < n;++j)
            {
                sigma2 += 
                    (W.col(j).transpose().array() * deltaSet(j).array().square().colwise().sum()).sum();
            }
            sigma2 /= W.sum() * d;

			// Report the current estimate.
			Cpd_State<Real> state = 
			{
                Qs, Ss, ts, sigma2, Ws
			};

			report(addConst(state));

            dreal qError = maxNorm(qPrev - Q);
            dreal sError = maxNorm(sPrev - S);
            dreal tError = maxNorm(tPrev - t);

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

        return sigma2;
    }

}

#endif
