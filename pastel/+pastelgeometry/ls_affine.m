% LS_AFFINE
% Optimal affine transformation between paired point-sets.
%
% [A, t, s] = ls_affine(fromSet, toSet, 'key', value, ...)
%
% where
%
% FROMSET is an (d x m) real matrix, where each column contains
% the coordinates of a d-dimensional point.
%
% TOSET is an (d x n) real matrix, where each column contains
% the coordinates of a d-dimensional point.
%
% A is a (d x d) real matrix, containing the matrix-part of the
% optimal transformation.
%
% T is a (d x 1) real vector containing the optimal translation.
%
% S is a non-negative real number containing the optimal scaling.
%
% Optional input arguments in 'key'-value pairs:
%
% MATRIX ('matrix') is a string which specifies constraints 
% for the matrix A. Must be one of
%     free: A free (default)
%     orthogonal: A^T A = I
%     identity: A = I
%
% SCALING ('scaling') is a string which specifies constraints
% for the scaling s. Must be one of
%     free: S >= 0 (default)
%     rigid: S = 1
%
% TRANSLATION ('translation') is a string which specifies constraints 
% for the translation t. Must be one of
%     free: T free (default)
%     identity: T = 0
%
% ORIENTATION ('orientation') is an integer which specifies constraints
% for the determinant of A. Must be one of
%    -1: det(A) < 0,
%     0: det(A) free (default), or
%     1: det(A) > 0.
%
% W ('W') is a (m x n) non-negative real matrix, which contains the 
% weights for the least-squares error metric.
%
% The A, T, and S are chosen such that they minimize the following
% Frobenius norm
%
%      ||(s * A * fromSet + t * ones(1, n)) - toSet||
%
% subject to the given constraints.

% Description: Optimal affine transformation between paired point-sets.

function [A, t, s] = ls_affine(fromSet, toSet, varargin)

eval(import_pastel);

concept_check(...
    fromSet, 'pointset', ...
    toSet, 'pointset');

% Optional input arguments
matrix = 'free';
scaling = 'free';
translation = 'free';
orientation = 0;
W = {};
eval(process_options({...
    'matrix', 'scaling', 'translation', ...
    'orientation', 'W'}, ...
    varargin));

% Check sufficient number of parameters.
if nargin < 2
    error('Not enough arguments.');
end

% Check equal dimensions requirement.
if size(fromSet, 1) ~= size(toSet, 1)
    error('The dimensions of FROMSET and TOSET must be equal.');
end

% Check equal number of points requirement.
if size(fromSet, 2) ~= size(toSet, 2)
    error('The number of points in FROMSET and TOSET must be equal.');
end

% Check matrix.
matrixSet = {'free', 'orthogonal', 'identity'};
if ~ismember(matrix, matrixSet)
    error('MATRIX must be either free, orthogonal, or identity.');
end

% Check scaling.
scalingSet = {'free', 'rigid'};
if ~ismember(scaling, scalingSet)
    error('SCALING must be either free or rigid.');
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

d = size(fromSet, 1);
m = size(fromSet, 2);
n = size(toSet, 2);

P = fromSet;
R = toSet;

if strcmp(translation, 'free')
    % Compute the centroids of the point-sets.
    if ~iscell(W)
        totalWeight = sum(W(:));
        fromCentroid = fromSet * W * (ones(n, 1) / totalWeight);
        toCentroid = toSet * W' * (ones(m, 1) / totalWeight);
    else
        fromCentroid = sum(fromSet, 2) / m;
        toCentroid = sum(toSet, 2) / n;
    end
    
    % Form the centered point-sets. The optimal transformation
    % will map fromCentroid to toCentroid.
    P = P - fromCentroid * ones(1, m);
    R = R - toCentroid * ones(1, n);
end

if ~iscell(W)
    c = sum(W, 2);
end

if strcmp(matrix, 'identity')
    % Use the identity matrix for A.
    A = eye(d, d);
elseif strcmp(matrix, 'free')
    % Compute the optimal linear transformation.
    if ~iscell(W)
        A = (R * W' * P') / (P * diag(c) * P');
    else
        A = (R * P') / (P * P');
    end
    
    if orientation ~= 0
        warning('Orientation can not be forced when the MATRIX is free.')
    end
else
    % Compute the optimal orthogonal transformation.
    if ~iscell(W)
        [U, D, V] = svd(R * W' * P');
    else
        [U, D, V] = svd(R * P');
    end
    A = U * V';

    if orientation ~= 0
        % If orientation is to be forced at det(A) = g, where g = +- 1,
        % then A is given by:
        %
        %    A = UDV^T, where
        %    D = [1, ..., 1, g det(UV^T)].

        % Compute the optimal oriented orthogonal Q.
        o = double(orientation);
        if sign(det(A)) ~= sign(o)
            A = U * diag([ones(1, d - 1), -1]) * V';
        end
    end
end

s = 1;
if strcmp(scaling, 'free') && ~strcmp(matrix, 'free')
    % Compute tr(P^T diag(c) P).
    if ~iscell(W)
        tracePtp = sum(sum((P * diag(c)).^2));
    else
        tracePtp = sum(P(:).^2);
    end
    
    % Compute the optimal scaling parameter.
    s = trace(D) / tracePtp;
end

t = zeros(d, 1);
if strcmp(translation, 'free')
    % Compute the optimal translation.
    t = toCentroid - s * A * fromCentroid;
end

