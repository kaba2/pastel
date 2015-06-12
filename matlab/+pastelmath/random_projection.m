% RANDOM_PROJECTION
% Computes a projection to a random subspace of given dimension.
%
% projectedSet = random_projection(pointSet, 'key', value, ...)
%
% where
%
% POINTSET is a (d x n) real matrix, where each column contains
% the coordinates of a d-dimensional point.
%
% Return values
% -------------
% 
% PROJECTEDSET is an (m x n) real matrix, where each column contains
% the coordinates of an m-dimensional point. The projectedSet(:, i) 
% is the orthogonal projection of pointSet(:, i) to the selected 
% random m-dimensional subspace. The m is specified by the optional 
% DIMENSION argument.
%
% Optional input arguments
% ------------------------
%
% DIMENSION ('dimension') is an integer which specifies the dimension
% of the subspace on which to project the points.
% Default: 2

% Description: Projection to a random subspace
% Documentation: projection.txt

function projectedSet = random_projection(pointSet, varargin)

eval(import_pastel);

% Optional input arguments
dimension = 2;
eval(process_options({'dimension'}, ...
    varargin));

d = size(pointSet, 1);

if dimension > d
    error('Subspace dimension must be >= point-set dimension.');
end

R = pastelmath.random_orthogonal(d, 'orientation', 1);
projectedSet = R(1 : dimension, :) * pointSet;
