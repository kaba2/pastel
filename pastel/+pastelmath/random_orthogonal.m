% RANDOM_ORTHOGONAL
% Generates a uniform random orthogonal matrix.
%
% Q = random_rotation(n, 'key', value, ...)
%
% where
%
% N is a non-negative integer specifying the dimension of the rotation.
%
% Q is an orthogonal (n x n) real matrix.
%
% Optional input arguments in 'key'-value pairs:
%
% ORIENTATION ('orientation') is an integer which can be used to 
% specify restrictions for the determinant of Q. The options are:
%    -1: det(Q) = -1,
%     0: no restrictions (default), or
%     1: det(Q) = 1.
%
% Description: Uniform random rotation
% Documentation: random_rotation_matrix.txt

function Q = random_orthogonal(n, varargin)

eval(import_pastel);

% Optional input arguments
orientation = 0;
eval(process_options({'orientation'}, ...
    varargin));

% See "How to Generate Random Matrices
% from the Classical Compact Groups",
% Francesco Mezzadri,
% Notices of the AMS,
% Volume 54, Number 5, 2007.

if n < 0
    error('N must be non-negative.');
end

% Check orientation requirements.
orientationSet = [-1, 0, 1];
if ~any(orientationSet == orientation)
    error('ORIENTATION must be one of -1, 0, or 1.')
end

M = randn(n, n);
[Q, R] = qr(M);
Q(:, diag(R) < 0) = -Q(:, diag(R) < 0);

if orientation ~= 0 
    % Take care of orientation.
    if sign(det(Q)) ~= sign(orientation)
        Q(:, 1) = -Q(:, 1);
    end
end
