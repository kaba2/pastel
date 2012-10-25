% RANDOM_ROTATION
% Generates a random rotation.
%
% Q = random_rotation(n)
%
% where
%
% N is a non-negative integer specifying the dimension of the rotation.
%
% Q is a special-orthogonal (n x n) real matrix.

function Q = random_rotation(n)

if n < 0
    error('N must be non-negative.');
end

M = randn(n, n) / sqrt(2);
[Q, R] = qr(M);
for i = 1 : n
    if R(i, i) < 0
        Q(:, i) = -Q(:, i);
    end
end
