% COHERENT_POINT_DRIFT
% Locally optimal transformation between unpaired point-sets.
%
% [Q, S, t] = coherent_point_drift(modelSet, sceneSet, ...
%                 matchingDistance, 'key', value, ...)
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
% Q is a (d x d) orthogonal matrix, giving the estimated 
% rotation/reflection.
%
% S is a (d x d) symmetric matrix, giving the estimated scaling.
%
% T is a (d x 1) matrix, giving the estimated translation.
%
% Optional input arguments in 'key'-value pairs:
%
% NOISERATIO ('noiseRatio') is a real number between (0, 1), which
% gives the weight for an additive uniform distribution component for
% the Gaussian mixture model. Larger noise-ratio makes the algorithm
% more tolerant to noise, but declines convergence rate when the
% actual noise level is lower. Default: 0.2.
%
% MATRIX ('matrix') is a string which specifies constraints 
% for the matrix Q. Must be one of
%     free: Q^T Q = I (default)
%     identity: Q = I
%
% SCALING ('scaling') is a string which specifies constraints
% for the scaling S. Must be one of
%     free: S^T = S (default)
%     conformal: S = sI
%     rigid: S = I
%
% TRANSLATION ('translation') is a string which specifies constraints 
% for the translation t. Must be one of
%     free: T free (default)
%     identity: T = 0
%
% ORIENTATION ('orientation') is an integer which specifies constraints
% for the determinant of A. Must be one of
%    -1: det(A) < 0,
%     0: det(A) free (free), or
%     1: det(A) > 0.
%
% Q0 ('Q0') is a (d x d) real orthogonal matrix, containing the initial 
% guess on Q. Default: eye(d, d).
%
% S0 ('S0') is a (d x d) real symmetric matrix, containing the initial 
% guess on S. Default: eye(d, d).
%
% T0 ('t0') is a (d x 1) vector, containing the initial guess on
% the matching translation T. Default: s - Q0 * S0 * m, where s and m are
% the centroids of the scene and model point-sets, respectively. This
% is the optimal translation assuming SCENESET and MODELSET match
% bijectively and Q0 = Q, and S0 = S.
%
% MINITERATIONS ('minIterations') is a positive integer which specifies 
% the minimum number of iterations for the algorithm to take.
% Default: 1.
%
% MAXITERATIONS ('maxIterations') is a positive integer which 
% specifies the maximum number of iterations for the algorithm to take. 
% It must hold that MINITERATIONS <= MAXITERATIONS.
% Default: 100.
%
% MINERROR ('minError') is the minimum error under 
% which to accept the transformation and stop iteration. 
% Default: 1e-11.
%
% DRAWPICTURES ('drawPictures') is a boolean which specifies whether
% the algorithm should draw pictures of the process (useful for 
% debugging). Default: false
%
% It should approximately be true that 
% 
%     Q * S * modelSet + t * ones(1, n)
%
% matches sceneSet.
%
% Reference:
%
% Point Set Registration: Coherent Point Drift,
% Andriy Myronenko, Xubo Song,
% IEEE Transactions on Pattern Analysis and Machine Intelligence,
% Volume 32, Number 12, December 2010.

% Description: Point-set registration between unpaired point-sets.
% Detail: Coherent Point Drift algorithm.

function [Q, S, t] = coherent_point_drift(...
    modelSet, sceneSet, varargin)

eval(import_pastel);

% Check that the dimensions of the point-sets are equal.
if size(modelSet, 1) ~= size(sceneSet, 1)
    error('The dimensions of MODELSET and SCENESET must be equal.')    
end

d = size(sceneSet, 1);
m = size(modelSet, 2);
n = size(sceneSet, 2);

% Optional input arguments
noiseRatio = 0.2;
minIterations = 1;
maxIterations = 100;
minError = 1e-11;
matrix = 'free';
scaling = 'free';
translation = 'free';
orientation = 0;
Q0 = eye(d, d);
S0 = eye(d, d);
t0 = {};
drawPictures = false;
eval(process_options({...
    'noiseRatio', ...
    'minIterations', 'maxIterations', ...
    'minError', 'matrix', 'scaling', 'translation', ...
    'orientation', 'Q0', 'S0', 't0', ...
    'drawPictures'}, ...
    varargin));

if iscell(t0)
    % Compute centroids for both point-sets.
    modelCentroid = sum(modelSet, 2) / m;
    sceneCentroid = sum(sceneSet, 2) / n;
    
    t0 = sceneCentroid - Q0 * S0 * modelCentroid;
end

concept_check(...
    modelSet, 'pointset', ...
    sceneSet, 'pointset', ...
    minError, 'real', ...
    noiseRatio, 'real');

if noiseRatio <= 0 || noiseRatio >= 1
    error('It must hold that 0 < NOISERATIO < 1.');
end

if minIterations > maxIterations
    error('It must hold that MINITERATIONS <= MAXITERATIONS.');
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

% Check scaling.
scalingSet = {'free', 'conformal', 'rigid'};
if ~ismember(scaling, scalingSet)
    error('SCALING must be either free, conformal, or rigid.');
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

Q = Q0;
S = S0;
t = t0;

% Compute the transformed model-set.
transformedSet = Q * S * modelSet + t * ones(1, m);

% Compute an initial estimate for sigma^2.
sigma2 = 0;
for j = 1 : n
    distanceSet = sum((transformedSet - sceneSet(:, j) * ones(1, m)).^2);
    sigma2 = sigma2 + sum(distanceSet);
end
sigma2 = sigma2 / (d * m * n);

figuresDrawn = 0;
figuresToDraw = 4;

W = zeros(m, n);
f = (2 * pi * sigma2)^(d / 2) * ...
    (noiseRatio / (1 - noiseRatio)) * (m / n);
for iteration = 0 : maxIterations - 1
    if drawPictures
        % Draw a nice picture.
        if mod(iteration, 5) == 0 && figuresDrawn < figuresToDraw
            figure;
            scatter(transformedSet(1, :), transformedSet(2, :), 'g.');
            %axis([-10, 10, -10, 10]);
            axis equal;
            hold on;
            scatter(sceneSet(1, :), sceneSet(2, :), 'r.');
            title(['CPD iteration ', int2str(iteration)]);
            legend('Model', 'Scene');
            hold off;
            figuresDrawn = figuresDrawn + 1;
        end
    end
    
    % Compute the weighting matrix.
    for j = 1 : n
        distanceSet = sum((transformedSet - sceneSet(:, j) * ones(1, m)).^2);
        expSet = exp(-distanceSet / (2 * sigma2));
        W(:, j) = expSet / (sum(expSet) + f);
    end

    % Store the previous transformation for comparison.
    qPrev = Q;
    sPrev = S;
    tPrev = t;
    
    % Compute a new estimate for the optimal transformation.
    [Q, S, t] = ls_affine(...
        modelSet, sceneSet, ...
        'matrix', matrix, ...
        'scaling', scaling, ...
        'translation', translation, ...
        'orientation', orientation, ...
        'W', W);
    
    % Compute the transformed model-set.
    transformedSet = Q * S * modelSet + t * ones(1, m);

    % Compute a new estimate for sigma^2.
    sigma2 = 0;
    for j = 1 : n
        distanceSet = sum((transformedSet - sceneSet(:, j) * ones(1, m)).^2);
        sigma2 = sigma2 + sum(W(:, j)' .* distanceSet);
    end
    totalWeight = sum(W(:));
    sigma2 = sigma2 / (totalWeight * d);
    
    % When the change to the previous transformation falls 
    % below the given error threshold, we will stop, provided that 
    % a minimum number of iterations has been performed.
    if norm(sPrev - S) < minError && ...
       norm(qPrev - Q) < minError && ...
       norm(tPrev - t) < minError && ...
       iteration >= minIterations - 1
       % We have convergence.
       break;
    end
end

