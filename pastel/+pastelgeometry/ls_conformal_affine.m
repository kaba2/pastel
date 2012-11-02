% LS_CONFORMAL_AFFINE
% Conformal-affine transformation between paired point-sets.
%
% [Q, t, s] = ls_conformal_affine(fromSet, toSet, 'key', value, ...)
%
% where
%
% FROMSET is an (d x n) real matrix, where each column contains
% the coordinates of a d-dimensional point.
%
% TOSET is an (d x n) real matrix, where each column contains
% the coordinates of a d-dimensional point.
%
% Q is a (d x d) real orthogonal matrix, containing the optimal
% orthogonal transformation satisfying the possible orientation 
% restriction.
%
% T is a (d x 1) real vector containing the optimal translation.
%
% S is a non-negative real number containing the optimal scaling.
%
% Optional input arguments in 'key'-value pairs:
%
% ORIENTATION ('orientation') is an integer which can be used to 
% specify restrictions for the determinant of Q. The options are:
%    -1: det(Q) = -1,
%     0: no restrictions (default), or
%     1: det(Q) = 1.
%
% The Q, T, and S are chosen such that they minimize the following
% Frobenius norm:
%
%      ||(s * Q * fromSet + t * ones(1, n)) - toSet||
%
% However, it turns out that the same Q and T also minimize
%
%      ||(Q * fromSet + t * ones(1, n)) - toSet||
%
% Therefore, this same function can be used to obtain an optimal
% rigid affine transformation between point-sets by ignoring S.
%
% Description: Conformal-affine transformation between paired point-sets.

function [Q, t, s] = ls_conformal_affine(fromSet, toSet, varargin)

eval(import_pastel);

% Optional input arguments
orientation = 0;
eval(process_options({'orientation'}, ...
    varargin));

% Check sufficient number of parameters.
if nargin < 2
    error('Not enough arguments.');
end

% Check equal dimensions requirement.
if size(fromSet, 1) ~= size(toSet, 1)
    error('The dimensions of FROMSET and TOSET must be equal.');
end

% Check equal number of points requirement.
if size(fromSet, 2) ~= size(toSet, 2)
    error('The number of points in FROMSET and TOSET must be equal.');
end

% Check orientation requirements.
orientationSet = [-1, 0, 1];
if ~any(orientationSet == orientation)
    error('ORIENTATION must be one of -1, 0, or 1.')
end

% For the optimal transformation it holds that
%
%    sQp' + t = q'
%
% where p' and q' are centroids of fromSet and toSet, respectively.
% Therefore,
%
%    t = q' - sQp'.
%
% Now we only need to minimize w.r.t. Q. If P and R are the centered
% versions of fromSet and toSet, respectively, then Q is given by:
%
%    Q = UV^T,
%
% where USV^T is the singular value decomposition of RP^T. 
% If orientation is to be forced at det(Q) = s, where s = +- 1,
% Then Q is given by:
%
%    Q = UDV^T, where
%    D = [1, ..., 1, s det(UV^T)].

m = size(fromSet, 1);
n = size(fromSet, 2);

% Compute the centroids of the point-sets.
fromCentroid = sum(fromSet, 2) / n;
toCentroid = sum(toSet, 2) / n;

% Form the centered point-sets. The optimal transformation
% will map fromCentroid to toCentroid.
P = fromSet - fromCentroid * ones(1, n);
R = toSet - toCentroid * ones(1, n);

% Compute the svd of RP^T.
[U, S, V] = svd(R * P');

% Compute the optimal orthogonal transformation Q.
Q = U * V';

if orientation ~= 0
    % Compute the optimal oriented orthogonal Q.
    o = double(orientation);
    if sign(det(Q)) ~= sign(o)
        Q = U * diag([ones(1, m - 1), -1]) * V';
        assert(sign(det(Q)) == sign(o));
    end
end

scale = 1;
if nargout >= 3
    % Compute the optimal scaling parameter.
    Lr = sum(sum(R .* (Q * P)));
    Lp = sum(sum(P.^2));
    s = Lr / Lp;
    scale = s;
end

% Compute the optimal translation.
t = toCentroid - scale * Q * fromCentroid;


