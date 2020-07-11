// Description: Iterated closest points (ICP)
// Documentation: icp.txt

#ifndef PASTELGEOMETRY_ICP_H
#define PASTELGEOMETRY_ICP_H

#include "pastel/geometry/ls_affine.h"

namespace Pastel
{

    using Icp_Matrix = LsAffine_Matrix;
    using Icp_Scaling = LsAffine_Scaling;
    using Icp_Translation = LsAffine_Translation;

    enum class Icp_Matching : integer
    {
        Closest,
        Biunique
    };

    template <typename Real>
    struct Icp_Return
    {
        Matrix<Real> Q;
        Matrix<Real> S;
        ColMatrix<Real> t;
    };

    template <typename Real>
    using Icp_State = 
        Icp_Return<Real>;

    //! Iterated closest points (ICP)
    /*
    Preconditions:
    0 <= inlierThreshold <= 1
    0 <= lambdaThreshold <= 1

    Locally optimal transformation between unpaired point-sets.

    modelSet:
    A set of n d-dimensional points. The points in the 'modelSet'
    are attempted to match to the points in the 'sceneSet'.

    sceneSet:
    A set of m d-dimensional points.

    matchingDistance2: 
    The distance over which to accept points being matched.

    Optional arguments
    ------------------

    kNearest (integer : 10):
    The number of nearest neighbors to search for each model point in 
    each iteration. Having a greater number of nearest neighbors allows 
    flexibility for finding bijective pairings (when matching = 
    Icp_Matching::Biunique). 

    minIterations (integer : 0):
    The minimum number of iterations for the algorithm to take.

    maxIterations (integer : 100):
    The maximum number of iterations for the algorithm to take. 
    It must hold that MINITERATIONS < MAXITERATIONS.
    Default: 100.

    minError (Real : 1e-11):
    The minimum trimmed-mean-square error under  which to accept 
    the transformation and stop iteration.

    matrix (Icp_Matrix : Free):
    Constraint for the matrix Q. 
        Free: Q^T Q = I
        Identity: Q = I

    translation (Icp_Translation : Free)
    Constraint for the translation t.
        Free: no constraint
        Identity: T = 0

    orientation (integer : 1)
    Constraint for the determinant of Q.
       <0: det(Q) < 0,
        0: no constraint,
       >0: det(Q) > 0.

    matching (Icp_Matching : Biunique):
    The strategy for pairing points in each iteration.
         Closest: closest point pairing; the original algorithm
         Biunique: approximate minimum-distance maximum matching
                   in the k-neighbor graph; the Biunique ICP 
                   algorithm.

    Q0: (Matrix<Real> : Matrix<Real>()):
    A (d, d) matrix by which to initialize the returned Q.
    If empty, then Q is initialized with the identity matrix,
    with fresh memory.

    S0: (Matrix<Real> : Matrix<Real>()):
    A (d, d) matrix by which to initialize the returned S.
    If empty, then S is initialized with the identity matrix,
    with fresh memory.

    t0: (ColMatrix<Real> : ColMatrix<Real>()):
    A (d, 1) matrix by which to initialize the returned t.
    If empty, then t is initialized with s - Q0 * m, where s 
    and m are the centroids of the scene and model point-sets, 
    respectively. This is the optimal translation assuming 
    'sceneSet' and 'modelSet' match bijectively and Q0 = Q.

    inlierThreshold (Real : 0.25):
    Specifies a threshold for the ratio of model points taking
    part on estimation (the inlier ratio). When the inlier ratio exceeds 
    the inlier threshold, the 'kNearest' is decremented by 1 (if > 1). This
    coarse-to-fine strategy helps the algorithm to get over possible local 
    minima.

    lambdaThreshold (Real : 0.1):
    Specifies a threshold for the ratio of NC-outliers in the
    model set (lambda). When lambda exceeds the lambda threshold, 
    then the strategy for distance-rejection changes.

    REPORTER ("reporter") is a lambda function which takes a single
    struct argument. The reporter is called after each iteration with
    the currently found transformation. This can be used to visualize
    or debug the workings of the algorithm. The reported struct consists
    of the following fields:

        modelSet, sceneSet: The input values.
        Q, t, pairSet: As in return values.
        transformedSet: Q * modelSet + ones(1, n) * t
        iteration: The iteration number, an integer.
        lambda
        meanDistance: The mean-squared distance between the paired points.
        inlierRatio

    Return values
    -------------

    The returned MATCH object is a struct containing the following fields.

    Q ("Q") is a (d x d) special-orthogonal matrix, containing the
    matching rotation.

    T ("t") is a (d x 1) vector, containing the matching translation.

    PAIRSET ("pairSet") is a (2 x k)-integer-array of indices, where each 
    column is a pair (i, j), where i is the index of a model point, and j
    is the index of its matched scene point. 

    It should approximately be true that 

        Q * modelSet + t * ones(1, n)

    matches sceneSet. The parameter choices correspond to known 
    algorithms roughly as follows:

        Original ICP: 
            kNearest = 1
            matchingType = "closest"

        Biunique ICP: 
            kNearest = k
            matchingType = "biunique"
    */
    template <
        NearestSet_Concept Model_NearestSet,
        NearestSet_Concept Scene_NearestSet,
        typename... ArgumentSet
    >
    Icp_Return<Real> icp(
        const Model_NearestSet& modelSet, 
        const Scene_NearestSet& sceneSet, 
        Real matchingDistance2,
        ArgumentSet&&... argumentSet)
    {
        // See _Robust ICP Registration using Biunique Correspondence_,
        // Lei Zhang, Sung-In Choi, Soon-Yong Park
        // International Conference on 3D Imaging, Modeling, Processing,
        // Visualization and Transmission, 2011.

        ENSURE_OP(pointSetDimension(modelSet), ==, pointSetDimension(sceneSet));

        integer kNearest = 
            PASTEL_ARG_S(kNearest, 10);
        integer minIterations = 
            PASTEL_ARG_S(minIterations, 0);
        integer maxIterations = 
            PASTEL_ARG_S(maxIterations, 100);
        Real minError = 
            PASTEL_ARG_S(minError, 1e-11);
        Icp_Matrix matrix = 
            PASTEL_ARG_ENUM(matrix, Icp_Matrix::Free);
        Icp_Translation translation = 
            PASTEL_ARG_ENUM(translation, Icp_Translation::Free);
        integer orientation = 
            PASTEL_ARG_S(orientation, 1);
        Icp_Matching matching = 
            PASTEL_ARG_ENUM(matching, Icp_Matching::Biunique);
        auto&& report = 
            PASTEL_ARG_S(report, nullOutput());

        Matrix<Real> Q = PASTEL_ARG_S(Q0, Matrix<Real>());
        Matrix<Real> S = PASTEL_ARG_S(S0, Matrix<Real>());
        ColMatrix<Real> t = PASTEL_ARG_S(t0, ColMatrix<Real>());

        Real inlierThreshold = 0.25;
        Real lambdaThreshold = 0.1;
 
        integer d = pointSetDimension(sceneSet);
        integer n = pointSetSize(modelSet);
        integer m = pointSetSize(sceneSet);

        if (Q.isempty())
        {
            // Clear a possible replicated strict flag.
            Q.reset();
            // The default for Q is the identity matrix.
            Q = Matrix<Real>::Identity(d, d);
        }

        ENSURE_OP(Q.rows(), ==, d);
        ENSURE_OP(Q.cols(), ==, d);

        if (S.isempty())
        {
            // Clear a possible replicated strict flag.
            S.reset();
            // The default for S is the identity matrix.
            S = Matrix<Real>::Identity(d, d);
        }

        ENSURE_OP(S.rows(), ==, d);
        ENSURE_OP(S.cols(), ==, d);

        if (t.isempty())
        {
            // Clear a possible replicated strict flag.
            t.reset();

            // Compute centroids for both point-sets.
            modelCentroid = sum(modelSet, 2) / n;
            sceneCentroid = sum(sceneSet, 2) / m;

            // The default for t matches centroids. 
            t = sceneCentroid - Q * S * modelCentroid;
        }

        ENSURE_OP(t.rows(), ==, d);
        ENSURE_OP(t.cols(), ==, 1);
        ENSURE_OP(kNearest, >=, 1);
        ENSURE_OP(minIterations, >=, 0);
        ENSURE_OP(minIterations, <=, maxIterations);
        ENSURE(!negative(matchingDistance2));

        if (matching == Icp_Matching::Closest && kNearest > 1)
        {
            // When closest-point matching is used, using
            // more than one nearest neighbor is redundant.
            kNearest = 1;
        }

        using Tree = PointKdTree<int>;

        Tree kdTree(d);
        kdTree.insert(sceneSet);
        kdTree.refine();

        Vector<Real> transformedPoint(ofDimension(d));
        Matrix<arma::uword> neighborGraph = Matrix<arma::uword>::Zero(
            2, kNearest * n);
        for (integer iteration = 0;iteration < maxIterations;++iteration)
        {
            // Compute the transformed model-set.
            Matrix<Real> transformedSet = 
                (Q * S * modelSet).colwise() + t;

            // Find nearest neighbors for each point in the
            // transformed model-set.
            for (integer j = 0;j < transformedSet.cols();++j)
            {
                for (integer i = 0;i < d;++i)
                {
                    transformedPoint(i) = transformedSet(i, j);
                }

                // TODO!
                ENSURE(false);

                searchNearest(
                    kdTree,
                    transformedPoint, 
                    PASTEL_TAG(maxDistance2), matchingDistance2, 
                    PASTEL_TAG(kNearest), kNearest);
            }

            if (matching == Icp_Matching::Biunique)
            {
                neighborGraph = biunique_matching(neighborSet);
                neighbors = size(neighborGraph, 2);
            }
            else
            {
                // Form a bipartite neighbor graph from the neighbor-relation.
                neighbors = 0;
                for (integer i = 0; i < n;++i)
                {
                    for (integer j = 0;j < kNearest;++j)
                    {
                        neighbor = neighborSet(i, j);
                        if (neighbor == 0)
                        {
                            break;
                        }
 
                        neighbors = neighbors + 1;
                        neighborGraph(1, neighbors) = i;
                        neighborGraph(2, neighbors) = neighbor;
                    }
                }
            }
 
            // Matching pairs.
            matchSet = neighborGraph(:, 1 : neighbors);
 
            // Compute the positions of the corresponding points.
            aSet = transformedSet(:, matchSet(1, :));
            bSet = sceneSet(:, matchSet(2, :));
            distanceSet = sum((aSet - bSet).^2);
 
            // The ratio of NC-outliers w.r.t. model points.
            lambda = 1 - (neighbors / n);
 
            // Mean-squared-distance between corresponding points.
            meanDistance = distanceSet.colwise().mean();
 
            // Compute the threshold for rejecting pairs based on
            // their distance.
            t = meanDistance;
            if (lambda > lambdaThreshold)
            {
                // Compute centroids for both point-sets.
                ColMatrix<Real> aCentroid = aSet.rowwise().mean();
                ColMatrix<Real> bCentroid = bSet.rowwise().mean();
                Real centroidDistance2 = (bCentroid - aCentroid).squaredNorm();
 
                t = t * std::pow(kNearest, lambda) + centroidDistance2;
            }
 
            // Reject those pairs which are too far apart.
            acceptSet = distanceSet <= t;
 
            acceptPairSet = distanceSet <= matchingDistance2^2;
            pairSet = matchSet(:, acceptPairSet);
 
            // Compute the positions of the corresponding points.
            matchSet = matchSet(:, acceptSet);
            distanceSet = distanceSet(acceptSet);
            aSet = modelSet(:, matchSet(1, :));
            bSet = sceneSet(:, matchSet(2, :));

            Real meanDistance = distanceSet.colwise().mean();

            // Compute a new estimate for the optimal transformation.
            auto lsMatch = lsAffine(
                aSet, bSet, 
                PASTEL_TAG(matrix), matrix, 
                PASTEL_TAG(scaling), scaling, 
                PASTEL_TAG(translation), translation, 
                PASTEL_TAG(orientation), orientation,
                PASTEL_TAG(Q0), std::move(Q),
                PASTEL_TAG(S0), std::move(S),
                PASTEL_TAG(t0), std::move(t)
                );

            Icp_State<Real> state = 
            {
                std::move(lsMatch.Q);
                std::move(lsMatch.S);
                std::move(lsMatch.t);
            };

            report(state);

            Q = std::move(state.Q);
            S = std::move(state.S);
            t = std::move(state.t);

            // Compute the inlier ratio. It is the ratio of model points
            // taking part on the estimation phase.
            Real inlierRatio = (Real)size(matchSet, 2) / n;
            if (inlierRatio > inlierThreshold)
            {
                // Since almost all points are inliers, we can afford
                // to decrease the number of nearest neighbors. This
                // also helps the algorithm to not to get stuck on
                // local minima.
                kNearest = std::max(kNearest - 1, (integer)1);
            }
 
            if (meanDistance < minError && 
                iteration >= minIterations - 1)
            {
                // Since the mean-square-error has dropped
                // below the required level, we can stop iterating.
                break
            }
        }

        return {std::move(Q), std::move(S), std::move(t), std::move(pairSet)};
    }

}

#endif
