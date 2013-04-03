% ICP
% Locally optimal transformation between unpaired point-sets.
%
% [Q, t, pairSet] = icp(modelSet, sceneSet, matchingDistance, ...
%                       'key', value, ...)
%
% where
%
% MODELSET is an (d x n) numeric array, where each column
% contains the coordinates of a d-dimensional point. The
% points in the MODELSET are attempted to match to the points
% in the SCENESET.
%
% SCENESET is an (d x m) numeric array, where each column
% contains the coordinates of a d-dimensional point.
%
% MATCHINGDISTANCE ('matchingDistance') is a non-negative real number
% which specifies the distance over which to accept points being matched.
%
% Q is a (d x d) special-orthogonal matrix, containing the
% matching rotation.
%
% T is a (d x 1) vector, containing the matching translation.
%
% PAIRSET is a (2 x k)-integer-array of indices, where each column is
% a pair (i, j), where i is the index of a model point, and j is the index
% of its matched scene point. 
%
% Optional input arguments in 'key'-value pairs:
%
% KNEAREST ('kNearest') is a positive integer which specifies the number 
% of nearest neighbors to search for each model point in each iteration. 
% Having a greater number of nearest neighbors allows flexibility for 
% finding bijective pairings (when MATCHINGTYPE = biunique). 
% Default: 10.
%
% MINITERATIONS ('minIterations') is a positive integer which specifies 
% the minimum number of iterations for the algorithm to take.
% Default: 1.
%
% MAXITERATIONS ('maxIterations') is a positive integer which 
% specifies the maximum number of iterations for the algorithm to take. 
% It must hold that MINITERATIONS < MAXITERATIONS.
% Default: 100.
%
% MINERROR ('minError') is the minimum trimmed-mean-square error under 
% which to accept the transformation and stop iteration. 
% Default: 1e-11.
%
% MATRIX ('matrix') is a string which specifies constraints 
% for the matrix Q. Must be one of
%     free: Q^T Q = I (default)
%     identity: Q = I
%
% TRANSLATION ('translation') is a string which specifies constraints 
% for the translation t. Must be one of
%     free: T free (default)
%     identity: T = 0
%
% ORIENTATION ('orientation') is an integer which specifies constraints
% for the determinant of Q. Must be one of
%    -1: det(Q) < 0,
%     0: det(Q) free, or
%     1: det(Q) > 0 (default).
%
% MATCHINGTYPE ('matchingType') is a string which specifies the strategy 
% to remove nearest neighbor pairs from being used in the estimation of
% the transformation in an iteration. Must be one of
%      closest: closest point pairing
%      biunique: approximate minimum-distance maximum matching
%                in the k-neighbor graph (default)
%
% Q0 ('q0') is a (d x d) real matrix, containing the initial guess on 
% the matching transformation Q. Default: eye(d, d).
%
% T0 ('t0') is a (d x 1) vector, containing the initial guess on
% the matching translation T. Default: s - Q0 * m, where s and m are
% the centroids of the scene and model point-sets, respectively. This
% is the optimal translation assuming SCENESET and MODELSET match
% bijectively and Q0 = Q.
%
% INLIERTHRESHOLD ('inlierThreshold') is a real number in the range 
% [0, 1] which specifies a threshold for the ratio of model points taking
% part on estimation (the inlier ratio). When the inlier ratio exceeds 
% the inlier threshold, the KNEAREST is decremented by 1 (if > 1). This
% coarse-to-fine strategy helps the algorithm to get over possible local 
% minima. Default: 0.25 (obtained by experimentation)
%
% LAMBDATHRESHOLD ('lambdaThreshold') is a real number in the range
% [0, 1] which specifies a threshold for the ratio of NC-outliers in the
% model set (lambda). When lambda exceeds the lambda threshold, then
% the strategy for distance-rejection changes.
% Default: 0.1
%
% REPORTER ('reporter') is a lambda function which takes a single
% struct argument. The reporter is called after each iteration with
% the currently found transformation. This can be used to visualize
% or debug the workings of the algorithm. The reported struct consists
% of the following fields:
%
%     modelSet, sceneSet: The input values.
%     Q, t, pairSet: As in return values.
%     transformedSet: Q * modelSet + ones(1, n) * t
%     iteration: The iteration number, an integer.
%     lambda
%     meanDistance: The mean-squared distance between the paired points.
%     inlierRatio
%
% Return values
% -------------
%
% It should approximately be true that 
% 
%     Q * modelSet + t * ones(1, n)
%
% matches sceneSet. The parameter choices correspond to known 
% algorithms roughly as follows:
%
%     Original ICP: 
%         kNearest = 1
%         matchingType = 'closest'
%
%     Biunique ICP: 
%         kNearest = k
%         matchingType = 'biunique'

% Description: Locally optimal transformation between unpaired point-sets.
% Detail: Original and Biunique ICP algorithms.

function [Q, t, pairSet] = icp(modelSet, sceneSet, ...
    matchingDistance, varargin)

% See _Robust ICP Registration using Biunique Correspondence_,
% Lei Zhang, Sung-In Choi, Soon-Yong Park
% International Conference on 3D Imaging, Modeling, Processing,
% Visualization and Transmission, 2011.

eval(import_pastel);

% Check that the dimensions of the point-sets are equal.
if size(modelSet, 1) ~= size(sceneSet, 1)
    error('The dimensions of MODELSET and SCENESET must be equal.')    
end

d = size(sceneSet, 1);
n = size(modelSet, 2);
m = size(sceneSet, 2);

% Optional input arguments
kNearest = 10;
minIterations = 1;
maxIterations = 100;
minError = 1e-11;
matrix = 'orthogonal';
translation = 'free';
orientation = 1;
matchingType = 'biunique';
Q0 = eye(d, d);
t0 = {};
inlierThreshold = 0.25;
lambdaThreshold = 0.1;
reporter = {};
eval(process_options({...
    'kNearest', 'minIterations', 'maxIterations', ...
    'minError', 'matrix', 'translation', ...
    'orientation', 'matchingType', ...
    'Q0', 't0', 'inlierThreshold', 'lambdaThreshold', ...
    'reporter'}, ...
    varargin));

if iscell(t0)
    % Compute centroids for both point-sets.
    modelCentroid = sum(modelSet, 2) / n;
    sceneCentroid = sum(sceneSet, 2) / m;
    
    t0 = sceneCentroid - Q0 * modelCentroid;
end

concept_check(...
    modelSet, 'pointset', ...
    sceneSet, 'pointset', ...
    minError, 'real', ...
    matchingDistance, 'real', ...
    reporter, 'lambda_function');

if kNearest < 1
    error('KNEAREST must be at least 1.')
end

if minIterations > maxIterations
    error('It must hold that MINITERATIONS <= MAXITERATIONS.');
end

matchingTypeSet = {'closest', 'biunique'};
if ~ismember(matchingType, matchingTypeSet)
    error('MATCHINGTYPE must be either closest or biunique.');
end

if matchingDistance < 0
    error('MATCHINGDISTANCE must a non-negative real number.');
end

if size(Q0, 1) ~= d || size(Q0, 2) ~= d
    error(['Q0 must be of size d x d, where d is ', ...
        'the dimension of the point-sets.']);
end

if size(t0, 1) ~= d || size(t0, 2) ~= 1
    error(['t0 must be of size d x 1, where d is ', ...
        'the dimension of the point-sets.']);
end

% Check matrix.
matrixSet = {'free', 'identity'};
if ~ismember(matrix, matrixSet)
    error('MATRIX must be either free, or identity.');
end

% Check translation.
translationSet = {'free', 'identity'};
if ~ismember(translation, translationSet)
    error('TRANSLATION must be either free or identity.');
end

% Check orientation.
orientationSet = [-1, 0, 1];
if ~any(orientation == orientationSet)
    error('ORIENTATION must be one of -1, 0, or 1.')
end

biunique = strcmp(matchingType, 'biunique');

if strcmp(matchingType, 'closest') && kNearest > 1
    % When closest-point matching is used, using
    % more than one nearest neighbor is redundant.
    kNearest = 1;
    warning('KNEAREST set to 1 since MATCHINGTYPE = closest.')
end

kdTree = pointkdtree_construct(d);
pointkdtree_insert(kdTree, sceneSet);
pointkdtree_refine(kdTree);

Q = Q0;
t = t0;

neighborGraph = int32(zeros(2, kNearest * n));
for iteration = 0 : maxIterations - 1
    % Compute the transformed model-set.
    transformedSet = Q * modelSet + t * ones(1, n);
    
    % Find nearest neighbors for each point in the
    % transformed model-set.
    matchingDistanceSet = Inf(1, n);
    neighborSet = pointkdtree_search_nearest(...
        kdTree, transformedSet, ...
        matchingDistanceSet, kNearest);

    if biunique
        neighborGraph = biunique_matching(neighborSet');
        neighbors = size(neighborGraph, 2);
    else
        % Form a bipartite neighbor graph from the neighbor-relation.
        neighbors = 0;
        for i = 1 : n
            for j = 1 : kNearest
                neighbor = neighborSet(i, j);
                if neighbor == 0
                    break;
                end
                
                neighbors = neighbors + 1;
                neighborGraph(1, neighbors) = i;
                neighborGraph(2, neighbors) = neighbor;
            end
        end
    end
    
    % Matching pairs.
    matchSet = neighborGraph(:, 1 : neighbors);
    
    % Compute the positions of the corresponding points.
    aSet = transformedSet(:, matchSet(1, :));
    bSet = sceneSet(:, matchSet(2, :));
    distanceSet = sum((aSet - bSet).^2);
        
    % The ratio of NC-outliers w.r.t. model points.
    lambda = 1 - (neighbors / n);
    
    % Mean-squared-distance between corresponding points.
    meanDistance = mean(distanceSet);
    
    % Compute the threshold for rejecting pairs based on
    % their distance.
    t = meanDistance;
    if lambda > lambdaThreshold
        % Compute centroids for both point-sets.
        aCentroid = sum(aSet, 2) / size(aSet, 2);
        bCentroid = sum(bSet, 2) / size(bSet, 2);
        centroidDistance2 = sum((bCentroid - aCentroid).^2);
        
        t = t * (kNearest^lambda) + centroidDistance2;
    end
    
    % Reject those pairs which are too far apart.
    acceptSet = distanceSet <= t;
    
    acceptPairSet = distanceSet <= matchingDistance^2;
    pairSet = matchSet(:, acceptPairSet);
    
    % Compute the positions of the corresponding points.
    matchSet = matchSet(:, acceptSet);
    distanceSet = distanceSet(acceptSet);
    aSet = modelSet(:, matchSet(1, :));
    bSet = sceneSet(:, matchSet(2, :));

    meanDistance = mean(distanceSet);

    % Compute a new estimate for the optimal transformation.
    [Q, S, t] = ls_affine(aSet, bSet, ...
        'matrix', matrix, ...
        'scaling', 'rigid', ...
        'translation', translation, ...
        'orientation', orientation);

    % Compute the inlier ratio. It is the ratio of model points
    % taking part on the estimation phase.
    inlierRatio = size(matchSet, 2) / n;
    if inlierRatio > inlierThreshold
        % Since almost all points are inliers, we can afford
        % to decrease the number of nearest neighbors. This
        % also helps the algorithm to not to get stuck on
        % local minima.
        kNearest = max(kNearest - 1, 1);
    end
    
    if ~iscell(reporter)
        reporter(struct(...,
            'Q', Q, ...
            't', t, ...
            'pairSet', pairSet, ...
            'modelSet', modelSet, ...
            'sceneSet', sceneSet, ...
            'transformedSet', transformedSet, ...
            'iteration', iteration, ...
            'meanDistance', meanDistance, ...
            'lambda', lambda, ...
            'inlierRatio', inlierRatio));
    end
    
    if meanDistance < minError && iteration >= minIterations - 1
        % Since the mean-square-error has dropped
        % below the required level, we can stop iterating.
        break
    end
end

pointkdtree_destruct(kdTree);
