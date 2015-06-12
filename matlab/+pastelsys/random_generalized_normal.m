% RANDOM_GENERALIZED_NORMAL
% Generates samples from a generalized normal distribution.
%
% X = random_generalized_normal(d, n, 'key', value, ...)
%
% where
%
% D is a positive integer which specifies the dimension of
% the distribution.
%
% N is a non-negative integer which specifies the number
% of samples to generate.
%
% X is a real (d x n)-matrix where each element is
% an independent sample from a generalized normal distribution
% with the given shape and scale parameters.
%
% Optional input arguments in 'key'-value pairs:
%
% SHAPE ('shape') is a real positive number, which specifies the
% shape of the distribution.
% Default: 2
%
% SCALE ('scale') is a real non-negative number, which specifies the
% scale of the distribution.
% Default: 1

% Description: Generates samples from a generalized normal distribution.
% Documentation: random_generalizedgaussian.txt

function X = random_generalized_normal(d, n, varargin)

import([pastel_package, '.*']);

concept_check(nargin, 'inputs', 2);
concept_check(nargout, 'outputs', 0 : 1);

% Concept checks
concept_check(...
	d, 'integer', ...
	d, 'positive', ...
	n, 'integer', ...
	n, 'non_negative');

% Optional input arguments
shape = 2;
scale = 1;
eval(process_options({'shape', 'scale'}, varargin));

concept_check(...
	'shape', 'real', ...
	'shape', 'positive', ...
	'scale', 'real', ...
	'scale', 'non-negative');

X = pastelsysmatlab('random_generalized_normal', d, n, shape, scale)
