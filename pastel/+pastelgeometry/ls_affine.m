% LS_AFFINE
% Optimal affine transformation between point-sets.
%
% [Q, S, t] = ls_affine(fromSet, toSet, 'key', value, ...)
%
% where
%
% FROMSET is an (d x m) real matrix, where each column contains
% the coordinates of a d-dimensional point.
%
% TOSET is an (d x n) real matrix, where each column contains
% the coordinates of a d-dimensional point. If W is not specified,
% it must hold that m = n.
%
% Q is a (d x d) real orthogonal matrix, representing a rotation or
% a reflection.
%
% S is a (d x d) real symmetric matrix, representing a scaling. Note 
% that by the eigenvalue decomposition, a symmetric matrix is the 
% composition of a rotation, axis-aligned scaling, and a reverse rotation. 
%
% T is a (d x 1) real matrix, representing a translation.
%
% Optional input arguments in 'key'-value pairs:
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
% for the determinant of QS. Must be one of
%    -1: det(QS) < 0,
%     0: det(QS) free (default), or
%     1: det(QS) > 0.
%
% W ('W') is a (m x n) non-negative real matrix, which contains the 
% weights for the least-squares error metric. If W is not given, then 
% it is required that m = n, and it is assumed that W = eye(m, n). 
%
% The Q, S, and T are chosen such that they minimize the error metric
%
%      sum_{i = 1}^m sum_{j = 1}^n w_{ij} ||(QS p_i + t) - r_j||^2
%
% subject to the given constraints. If m = n, and W is diagonal,
% then the error metric simplifies to
%
%      sum_{i = 1}^m w_{ii} ||(QS p_i + t) - r_i||^2.

% Description: Optimal affine transformation between point-sets.
% Documentation: ls_transformations.txt

function [Q, S, t] = ls_affine(fromSet, toSet, varargin)

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
if iscell(W) && size(fromSet, 2) ~= size(toSet, 2)
    error(['Since W is not specified, the number of points in ', ...
        'FROMSET and TOSET must be equal.']);
end

% Check matrix.
matrixSet = {'free', 'identity'};
if ~ismember(matrix, matrixSet)
    error('MATRIX must be either free or identity.');
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

if strcmp(scaling, 'free') && orientation ~= 0
    warning('Orientation can not be forced when SCALING is free.');
end

if strcmp(matrix, 'identity') && orientation ~= 0
    if ~(strcmp(scaling, 'rigid') && orientation == 1)
        warning('Orientation can not be forced when MATRIX is identity.');
    end
end

d = size(fromSet, 1);
m = size(fromSet, 2);
n = size(toSet, 2);

P = fromSet;
R = toSet;

% Defaults for Q and S.
Q = eye(d, d);
S = eye(d, d);

% Compute the sum of all weights.
if ~iscell(W)
    % With weighting.
    totalWeight = sum(W(:));
    if totalWeight <= 0
        error('W = 0 is not allowed.');
    end
end

if strcmp(translation, 'free')
    % Compute the centroids of the point-sets.
    if ~iscell(W)
        fromCentroid = fromSet * W * (ones(n, 1) / totalWeight);
        toCentroid = toSet * W' * (ones(m, 1) / totalWeight);
    else
        % Without weighting.
        fromCentroid = sum(fromSet, 2) / m;
        toCentroid = sum(toSet, 2) / n;
    end
    
    % Form the centered point-sets. The optimal transformation
    % will map fromCentroid to toCentroid. After this the problem
    % has been reduced from affine to linear.
    P = P - fromCentroid * ones(1, m);
    R = R - toCentroid * ones(1, n);
end

if ~iscell(W)
    % With weighting.
    PP = P * diag(sum(W, 2)) * P';
    RP = R * W' * P';
else
    % Without weighting.
    PP = P * P';
    RP = R * P';
end

if strcmp(scaling, 'free') && strcmp(matrix, 'identity')
    % f(x) = Sx
    
    % Find the optimal scaling.
    S = lyap(PP, -(RP + RP'));
end

if strcmp(scaling, 'free') && strcmp(matrix, 'free')
    % f(x) = Ax
    
    % Compute the optimal linear transformation.
    [UP, UR, X, DP, DR] = gsvd(PP, RP);
    A = UR * (DR * pinv(DP)) * UP';
    %A = RP * pinv(PP);
    
    % Compute Q and S from A such that
    % A = QS and S is symmetric positive semi-definite.
    [U, D, V] = svd(A);
    Q = U * V';
    S = V * D * V';
end

if ~strcmp(scaling, 'free') && strcmp(matrix, 'free')
    % f(x) = sQx
    
    % Compute the optimal orthogonal transformation.
    [U, D, V] = svd(RP);
    Q = U * V';

    if orientation ~= 0
        % If orientation is to be forced at det(A) = g, where g = +- 1,
        % then A is given by:
        %
        %    Q = UDV^T, where
        %    D = [1, ..., 1, g det(UV^T)].

        % Compute the optimal oriented orthogonal Q.
        o = double(orientation);
        if sign(det(Q)) ~= sign(o)
            Q = U * diag([ones(1, d - 1), -1]) * V';
        end
    end
end

if strcmp(scaling, 'conformal')
    % f(x) = sQx
    
    % Compute the optimal scaling parameter.
    s = trace(Q' * RP) / trace(PP);
    S = s * eye(d, d);
end

t = zeros(d, 1);
if strcmp(translation, 'free')
    % Compute the optimal translation.
    t = toCentroid - Q * S * fromCentroid;
end

