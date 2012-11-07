% LS_AFFINE
% Optimal affine transformation between paired point-sets.
%
% [A, t] = ls_affine(fromSet, toSet)
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
% T is a (d x 1) real vector containing the translation part of the
% optimal affine translation.
%
% The A and T are chosen such that they minimize the following
% Frobenius norm:
%
%      ||(A * fromSet + t * ones(1, n)) - toSet||

% Description: Optimal affine transformation between paired point-sets.

function [A, t] = ls_affine(fromSet, toSet)

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

fromCentroid = sum(fromSet, 2) / m;
toCentroid = sum(toSet, 2) / n;

A = ls_linear(...
    fromSet - fromCentroid * ones(1, m), ...
    toSet - toCentroid * ones(1, n));

t = toCentroid - A * fromCentroid;
