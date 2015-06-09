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

pastelgeometrymatlab('ls_affine', ...
    fromSet, toSet, ...
    W, matrix, scaling, ...
    translation, orientation)