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
% Optional input arguments in 'key'-value pairs:
%
% TRANSLATION ('translation') is a string which specifies whether
% translation is included in the class of possible transformations.
% Must be one of
%     affine: T is free (default)
%     linear: T = zeros(1, d)
%
% The A and T are chosen such that they minimize the following
% Frobenius norm:
%
%      ||(A * fromSet + t * ones(1, n)) - toSet||

% Description: Optimal affine transformation between paired point-sets.

function [A, t] = ls_affine(fromSet, toSet, varargin)

eval(import_pastel);

concept_check(...
    fromSet, 'pointset', ...
    toSet, 'pointset');

% Optional input arguments
translation = 'affine';
eval(process_options({...
    'translation'}, ...
    varargin));

if size(fromSet, 2) ~= size(toSet, 2)
    error('FROMSET and TOSET must have the same number of points.');
end

if size(fromSet, 1) ~= size(toSet, 1)
    error('FROMSET and TOSET must have the same dimension.');
end

d = size(fromSet, 1);
n = size(fromSet, 2);

P = fromSet;
R = toSet;

if strcmp(translation, 'affine')
    % Compute the centroids of both point-sets.
    fromCentroid = sum(fromSet, 2) / n;
    toCentroid = sum(toSet, 2) / n;
    
    P = P - fromCentroid * ones(1, n);
    R = R - toCentroid * ones(1, n);
end

% Compute the optimal transformation.
A = (R * P') / (P * P');

t = zeros(d, 1);
if strcmp(translation, 'affine')
    % Compute the optimal translation.
    t = toCentroid - A * fromCentroid;
end
