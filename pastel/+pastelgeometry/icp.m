% ICP
% Locally optimal transformation between unpaired point-sets.
%
% [Q, T] = icp(modelSet, sceneSet, 'key', value, ...)
%
% where
%
% MODELSET is an (d x n) numeric array, where each column
% contains the coordinates of a d-dimensional point. The
% points in the MODELSET are tried to match to points
% in the SCENESET.
%
% SCENESET is an (d x m) numeric array, where each column
% contains the coordinates of a d-dimensional point.
%
% Q is a (d x d) special-orthogonal matrix, containing the
% matching rotation.
%
% T is a (d x 1) vector, containing the matching translation.
%
% Optional input arguments in 'key'-value pairs:
%
% MATCHINGRATIO ('matchingRatio') is a real-number in the range [0, 1]. 
% It is the ratio of points in MODELSET which to use for finding a
% a pairing by closest points. This provides the trimming
% for the Trimmed ICP. Setting ALPHA = 1 corresponds to the
% original ICP algorithm. Default: 1.
%
% MATCHINGDISTANCE ('matchingDistance') is either a non-negative real 
% number or an arbitrarily-dimensional real-matrix whose linearization 
% contains n non-negative real numbers. It specifies for each point in 
% MODELSET the maximum distance over which the nearest neighbors are 
% searched. In case the MATCHINGDISTANCE is a real number, the same 
% distance is assumed for all points. Default: Inf.
%
% KNEAREST ('kNearest') is an integer which specifies the number of 
% nearest neighbors to search for each point. Having a greater number of 
% nearest neighbors allows flexibility for finding bijective pairings (as 
% used in Biunique ICP). Default: 1.
%
% MINITERATIONS ('minIterations') is a non-negative integer which 
% specifies the minimum number of iterations for the algorithm to take. 
% Default: 0.
%
% MAXITERATIONS ('maxIterations') is a non-negative integer which 
% specifies the maximum number of iterations for the algorithm to take. 
% Default: 100.
%
% MINERROR ('minError') is the minimum trimmed-mean-square error under 
% which to accept the transformation and stop iteration. Default: 1e-11.
%
% TRANSFORMTYPE ('transformType') is a string which specifies the class of
% transformations to search the optimal transformation in. Must be one of
%      conformal: x |--> Qx + t. (default)
%      translation: x |--> x + t.
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
% It should approximately be true that 
% 
%     Q * modelSet + T * ones(1, n)
%
% matches sceneSet. The parameter choices correspond to known 
% algorithms as follows:
%
%     Original ICP: 
%         kNearest = 1
%         matchingRatio = 1
%         matchingDistance = Inf
%
%     Trimmed ICP: 
%         kNearest = 1
%         matchingRatio = r
%         matchingDistance = Inf
%
%     Biunique ICP: 
%         kNearest = k
%         matchingRatio = 1
%         matchingDistance = eps
%
% Description: Locally optimal transformation between unpaired point-sets.
% Detail: Generalizes Original, Trimmed, and Biunique ICP algorithms.

function [Q, t] = icp(modelSet, sceneSet, varargin)

eval(import_pastel);

% Check that the dimensions of the point-sets are equal.
if size(modelSet, 1) ~= size(sceneSet, 1)
    error('The dimensions of MODELSET and SCENESET must be equal.')    
end

d = size(sceneSet, 1);
n = size(modelSet, 2);
m = size(sceneSet, 2);

% Compute centroids for both point-sets.
modelCentroid = sum(modelSet, 2) / n;
sceneCentroid = sum(sceneSet, 2) / m;

% Optional input arguments
matchingRatio = 1;
matchingDistance = Inf;
kNearest = 1;
minIterations = 0;
maxIterations = 100;
minError = 1e-11;
transformType = 'conformal';
Q0 = eye(d, d);
t0 = {};
eval(process_options({'matchingRatio', 'matchingDistance', ...
    'kNearest', 'minIterations', 'maxIterations', ...
    'minError', 'transformType', 'Q0', 't0'}, ...
    varargin));

if iscell(t0)
    t0 = sceneCentroid - Q0 * modelCentroid;
end

check(modelSet, 'pointset');
check(sceneSet, 'pointset');
check(matchingRatio, 'real');
check(matchingDistance, 'real');
check(minError, 'real');

if kNearest < 1
    error('KNEAREST must be at least 1.')
end

if matchingRatio < 0 || matchingRatio > 1
    error('MATCHINGRATIO must be in the range [0, 1]');
end

if numel(matchingDistance) == 1
    matchingDistanceSet = ones(1, n) * matchingDistance^2;
else
    matchingDistanceSet = matchingDistance(:).^2;
end

transformTypeSet = {'conformal', 'translation'};
if ~ismember(transformType, transformTypeSet)
    error('TRANSFORMTYPE must be either conformal or translation.');
end

if size(Q0, 1) ~= d || size(Q0, 2) ~= d
    error(['Q0 must be of size d x d, where d is ', ...
        'the dimension of the point-sets.']);
end

if size(t0, 1) ~= d || size(t0, 2) ~= 1
    error(['t0 must be of size d x 1, where d is ', ...
        'the dimension of the point-sets.']);
end

kdTree = pointkdtree_construct(d);
pointkdtree_insert(kdTree, sceneSet);
pointkdtree_refine(kdTree);

conformal = strcmp(transformType, 'conformal');

Q = Q0;
t = t0;

nTrimmed = ceil(n * matchingRatio);
for iteration = 0 : maxIterations - 1
    % Compute the transformed model-set.
    transformedSet = Q * modelSet + t * ones(1, n);
    
    if (mod(iteration, 2) == 0 && iteration < 8) || ...
       iteration == maxIterations - 1
        figure;
        scatter(transformedSet(1, :), transformedSet(2, :), 'g.');
        %axis([-10, 10, -10, 10]);
        axis equal;
        hold on;
        scatter(sceneSet(1, :), sceneSet(2, :), 'r.');
        title(['ICP iteration ', int2str(iteration)]);
        legend('Model', 'Scene');
        hold off;
    end
        
    % Find nearest neighbors for each point in the
    % transformed model-set.
    [neighborSet, distanceSet] = pointkdtree_search_nearest(...
        kdTree, transformedSet, ...
        matchingDistanceSet, kNearest);
    
    % Sort model-points by the distance to the first neighbor.
    [ignore, I] = sort(distanceSet(:, 1));
    
    % Pick an alpha-ratio of the model-points with smallest distances.
    matchSet = I(1 : nTrimmed);
    nearestSet = pointkdtree_as_points(kdTree, neighborSet(matchSet, 1));
    
    % Compute the trimmed mean-square error.
    trimmedMse = sum(distanceSet(matchSet, 1)) / nTrimmed;
   
    % Compute a new estimate for the optimal transformation.
    if conformal
        [Q, t] = ls_conformal_affine(...
            modelSet(:, matchSet), ...
            nearestSet, 'orientation', 1);
    else
        t = ls_translation(modelSet(:, matchSet), nearestSet);
    end

    if trimmedMse < minError && iteration >= minIterations - 1
        % Since the trimmed mean-square-error has dropped
        % below the required level, we can stop iterating.
        break
    end
end

pointkdtree_destruct(kdTree);
