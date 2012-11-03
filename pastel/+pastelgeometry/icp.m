% ICP
% Locally optimal transformation between unpaired point-sets.
%
% [Q, T] = icp(modelSet, sceneSet, 'key', value, ...)
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
% Q is a (d x d) special-orthogonal matrix, containing the
% matching rotation.
%
% T is a (d x 1) vector, containing the matching translation.
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
% TRANSFORMTYPE ('transformType') is a string which specifies the class of
% transformations to search the optimal transformation in. Must be one of
%      conformal: x |--> Qx + t. (default)
%      translation: x |--> x + t.
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
% the inlier threshold, the KNEAREST is decrement by 1 (if > 1). This
% coarse-to-fine strategy helps the algorithm to get over possible local 
% minima. Default: 0.25 (obtained by experimentation)
%
% LAMBDATHRESHOLD ('lambdaThreshold') is a real number in the range
% [0, 1] which specifies a threshold for the ratio of NC-outliers in the
% model set (lambda). When lambda exceeds the lambda threshold, then
% the strategy for distance-rejection changes.
%
% DRAWPICTURES ('drawPictures') is a boolean which specifies whether
% the algorithm should draw pictures of the process (useful for 
% debugging). Default: false
%
% It should approximately be true that 
% 
%     Q * modelSet + T * ones(1, n)
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

function [Q, t] = icp(modelSet, sceneSet, varargin)

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

% Compute centroids for both point-sets.
modelCentroid = sum(modelSet, 2) / n;
sceneCentroid = sum(sceneSet, 2) / m;

% Optional input arguments
kNearest = 10;
minIterations = 1;
maxIterations = 100;
minError = 1e-11;
transformType = 'conformal';
matchingType = 'biunique';
Q0 = eye(d, d);
t0 = {};
inlierThreshold = 0.25;
lambdaThreshold = 0.1;
drawPictures = false;
eval(process_options({...
    'kNearest', 'minIterations', 'maxIterations', ...
    'minError', 'transformType', 'matchingType', ...
    'Q0', 't0', 'inlierThreshold', 'lambdaThreshold', ...
    'drawPictures'}, ...
    varargin));

if iscell(t0)
    t0 = sceneCentroid - Q0 * modelCentroid;
end

check(modelSet, 'pointset');
check(sceneSet, 'pointset');
check(minError, 'real');

if kNearest < 1
    error('KNEAREST must be at least 1.')
end

if minIterations > maxIterations
    error('It must hold that MINITERATIONS <= MAXITERATIONS.');
end

transformTypeSet = {'conformal', 'translation'};
if ~ismember(transformType, transformTypeSet)
    error('TRANSFORMTYPE must be either conformal or translation.');
end

matchingTypeSet = {'closest', 'biunique'};
if ~ismember(matchingType, matchingTypeSet)
    error('MATCHINGTYPE must be either closest or biunique.');
end

if size(Q0, 1) ~= d || size(Q0, 2) ~= d
    error(['Q0 must be of size d x d, where d is ', ...
        'the dimension of the point-sets.']);
end

if size(t0, 1) ~= d || size(t0, 2) ~= 1
    error(['t0 must be of size d x 1, where d is ', ...
        'the dimension of the point-sets.']);
end

conformal = strcmp(transformType, 'conformal');
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
        % This is a greedy algorithm to approximate minimum weight
        % maximum bipartite matching. It is the one given in the
        % Biunique ICP paper.
        neighbors = 0;
        reservedSet = false(1, m);
        for i = 1 : n
            for j = 1 : kNearest
                neighbor = neighborSet(i, j);
                if neighbor == 0
                    break;
                end
                if ~reservedSet(neighbor)
                    neighbors = neighbors + 1;
                    neighborGraph(1, neighbors) = i;
                    neighborGraph(2, neighbors) = neighbor;
                    reservedSet(neighbor) = true;
                    break;
                end
            end
        end
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
    aSet = modelSet(:, matchSet(1, :));
    bSet = pointkdtree_as_points(kdTree, matchSet(2, :));
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
    
    % Compute the positions of the corresponding points.
    aSet = aSet(:, acceptSet);
    bSet = bSet(:, acceptSet);
    distanceSet = distanceSet(acceptSet);
    meanDistance = mean(distanceSet);
    matchSet = matchSet(:, acceptSet);

    % Compute a new estimate for the optimal transformation.
    if conformal
        % The class of conformal-affine transformations.
        [Q, t] = ls_conformal_affine(aSet, bSet, ...
            'orientation', 1);
    else
        % The class of translations.
        t = ls_translation(aSet, bSet);
    end

    if drawPictures
        % Draw a nice picture.
        if (mod(iteration, 2) == 0 && iteration < 8) || ...
           iteration == maxIterations - 1
            figure;
            scatter(transformedSet(1, :), transformedSet(2, :), 'g.');
            %axis([-10, 10, -10, 10]);
            axis equal;
            hold on;
            scatter(sceneSet(1, :), sceneSet(2, :), 'r.');
            for i = 1 : size(matchSet, 2)
                u = matchSet(1, i);
                v = matchSet(2, i);
                line([transformedSet(1, u), sceneSet(1, v)], ...
                    [transformedSet(2, u), sceneSet(2, v)]);
            end        
            title(['ICP iteration ', int2str(iteration)]);
            legend('Model', 'Scene');
            hold off;
        end
    end

    % Compute the inlier ratio. It is the ratio of model points
    % taking part on the estimation phase.
    inlierRatio = size(matchSet, 2) / n;
    if inlierRatio > inlierThreshold
        % Since almost all points are inlier, we can afford
        % to decrease the number of nearest neighbors. This
        % also helps the algorithm to not to get stuck on
        % local minima.
        kNearest = max(kNearest - 1, 1);
    end
    
    if meanDistance < minError && iteration >= minIterations - 1
        % Since the mean-square-error has dropped
        % below the required level, we can stop iterating.
        break
    end
end

pointkdtree_destruct(kdTree);
