% LS_LINEAR
% Optimal linear transformation between paired point-sets.
%
% A = ls_linear(fromSet, toSet)
%
% where
%
% FROMSET is an (d x n) real matrix, where each column contains
% the coordinates of a d-dimensional point.
%
% TOSET is an (d x n) real matrix, where each column contains
% the coordinates of a d-dimensional point.
%
% A is a (d x d) real matrix, containing the matrix part of the
% optimal affine transformation.
%
% The A is chosen such that it minimizes the following
% Frobenius norm:
%
%      ||A * fromSet - toSet||

% Description: Optimal linear transformation between paired point-sets.

function A = ls_linear(fromSet, toSet)

eval(import_pastel);

concept_check(...
    fromSet, 'pointset', ...
    toSet, 'pointset');

m = size(fromSet, 2);
n = size(toSet, 2);

if n ~= m
    error('FROMSET and TOSET must have the same number of points.');
end

if size(fromSet, 1) ~= size(toSet, 1)
    error('FROMSET and TOSET must have the same dimension.');
end

A = (toSet * fromSet') / (fromSet * fromSet');
