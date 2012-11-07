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
% SCALING ('scaling') is a string which specifies whether scaling
% is included in the class of possible transformations. 
% Must be one of
%     conformal: S is free (default)
%     rigid: S = 1
%
% TRANSLATION ('translation') is a string which specifies whether
% translation is included in the class of possible transformations.
% Must be one of
%     affine: T is free (default)
%     linear: T = zeros(1, d)
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

% Description: Conformal-affine transformation between paired point-sets.

function [Q, t, s] = ls_conformal_affine(fromSet, toSet, varargin)

eval(import_pastel);

concept_check(...
    fromSet, 'pointset', ...
    toSet, 'pointset');

% Optional input arguments
orientation = 0;
scaling = 'conformal';
translation = 'affine';
eval(process_options({...
    'orientation', 'scaling', 'translation'}, ...
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
if ~any(orientation == orientationSet)
    error('ORIENTATION must be one of -1, 0, or 1.')
end

% Check scaling.
scalingSet = {'conformal', 'rigid'};
if ~ismember(scaling, scalingSet)
    error('SCALING must be either conformal or rigid.');
end

% Check translation.
translationSet = {'affine', 'linear'};
if ~ismember(translation, translationSet)
    error('TRANSLATION must be either affine or linear.');
end

d = size(fromSet, 1);
n = size(fromSet, 2);

P = fromSet;
R = toSet;

if strcmp(translation, 'affine')
    % For the optimal transformation it holds that
    %
    %    sQp' + t = q'
    %
    % where p' and q' are centroids of fromSet and toSet, 
    % respectively. Therefore,
    %
    %    t = q' - sQp'.
    
    % Compute the centroids of the point-sets.
    fromCentroid = sum(fromSet, 2) / n;
    toCentroid = sum(toSet, 2) / n;
    
    % Form the centered point-sets. The optimal transformation
    % will map fromCentroid to toCentroid.
    P = P - fromCentroid * ones(1, n);
    R = R - toCentroid * ones(1, n);
end

% Now we only need to minimize w.r.t. Q. If P and R are the centered
% versions of fromSet and toSet, respectively, then Q is given by:
%
%    Q = UV^T,
%
% where USV^T is the singular value decomposition of RP^T. 

% Compute the svd of RP^T.
[U, D, V] = svd(R * P');

% Compute the optimal orthogonal transformation Q.
Q = U * V';

if orientation ~= 0
    % If orientation is to be forced at det(Q) = s, where s = +- 1,
    % Then Q is given by:
    %
    %    Q = UDV^T, where
    %    D = [1, ..., 1, s det(UV^T)].
    
    % Compute the optimal oriented orthogonal Q.
    o = double(orientation);
    if sign(det(Q)) ~= sign(o)
        Q = U * diag([ones(1, d - 1), -1]) * V';
        %assert(sign(det(Q)) == sign(o));
    end
end

s = 1;
if strcmp(scaling, 'conformal')
    % Compute the optimal scaling parameter.
    s = trace(D) / sum(P(:).^2);
end

t = zeros(d, 1);
if strcmp(translation, 'affine')
    % Compute the optimal translation.
    t = toCentroid - s * Q * fromCentroid;
end

