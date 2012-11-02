% LS_TRANSLATION
% Least-squares translation between paired point-sets.
%
% t = ls_translation(fromSet, toSet)
%
% where
%
% FROMSET is an (d x n) real matrix, where each column contains
% the coordinates of a d-dimensional point.
%
% TOSET is an (d x n) real matrix, where each column contains
% the coordinates of a d-dimensional point.
%
% T is a (d x 1) real vector containing the optimal translation.
%
% The T is chosen such that it minimizes the following
% Frobenius norm:
%
%      ||(fromSet + t * ones(1, n)) - toSet||

% Description: Least-squares translation between paired point-sets.

function t = ls_conformal_affine(fromSet, toSet)

eval(import_pastel);

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

% For the optimal transformation it holds that
%
%    p' + t = q'
%
% where p' and q' are centroids of fromSet and toSet, respectively.
% Therefore,
%
%    t = q' - p'.

n = size(fromSet, 2);

% Compute the centroids of the point-sets.
fromCentroid = sum(fromSet, 2) / n;
toCentroid = sum(toSet, 2) / n;

% Compute the optimal translation.
t = toCentroid - fromCentroid;


