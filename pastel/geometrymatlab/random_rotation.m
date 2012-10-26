% RANDOM_ROTATION
% Generates a uniform random rotation.
%
% Q = random_rotation(n)
%
% where
%
% N is a non-negative integer specifying the dimension of the rotation.
%
% Q is a special-orthogonal (n x n) real matrix.
%
% Description: Uniform random rotation
% Documentation: random_rotation_matrix.txt

function Q = random_rotation(n)

% See "How to Generate Random Matrices
% from the Classical Compact Groups",
% Francesco Mezzadri,
% Notices of the AMS,
% Volume 54, Number 5, 2007.

if n < 0
    error('N must be non-negative.');
end

M = randn(n, n);
[Q, R] = qr(M);
Q(:, diag(R) < 0) = -Q(:, diag(R) < 0);
if det(Q) < 0
    Q(:, 1) = -Q(:, 1);
end
