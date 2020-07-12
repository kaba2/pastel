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

if n < 0
    error('N must be non-negative.');
end

Q = pastelmathmatlab('random_orthogonal', n, orientation);

end
