% COHERENT_POINT_DRIFT
% Locally optimal transformation between unpaired point-sets.
%
% [Q, S, t] = coherent_point_drift(fromSet, toSet, ...
%                 'key', value, ...)
%
% where
%
% FROMSET is an (d x n) numeric array, where each column
% contains the coordinates of a d-dimensional point. The
% points in the FROMSET are attempted to match to the points
% in the TOSET.
%
% TOSET is an (d x m) numeric array, where each column
% contains the coordinates of a d-dimensional point.
%
% Q is a (d x d) orthogonal matrix, giving the estimated 
% rotation/reflection.
%
% S is a (d x d) symmetric matrix, giving the estimated scaling.
%
% T is a (d x 1) matrix, giving the estimated translation.
%
% Optional input arguments in 'key'-value pairs:
%
% NOISERATIO ('noiseRatio') is a real number between (0, 1), which
% gives the weight for an additive uniform distribution component for
% the Gaussian mixture model. Larger noise-ratio makes the algorithm
% more tolerant to noise, but declines convergence rate when the
% actual noise level is lower. Default: 0.2.
%
% MATRIX ('matrix') is a string which specifies constraints 
% for the matrix Q. Must be one of
%     free: Q^T Q = I (default)
%     identity: Q = I
%
% SCALING ('scaling') is a string which specifies constraints
% for the scaling S. Must be one of
%     free: S^T = S (default)
%     conformal: S = sI
%     rigid: S = I
%
% TRANSLATION ('translation') is a string which specifies constraints 
% for the translation t. Must be one of
%     free: T free (default)
%     identity: T = 0
%
% ORIENTATION ('orientation') is an integer which specifies constraints
% for the determinant of A. Must be one of
%    -1: det(A) < 0,
%     0: det(A) free (default), or 
%     1: det(A) > 0.
%
% Q0 ('Q0') is a (d x d) real orthogonal matrix, containing the initial 
% guess on Q. Default: eye(d, d).
%
% S0 ('S0') is a (d x d) real symmetric matrix, containing the initial 
% guess on S. Default: eye(d, d).
%
% T0 ('t0') is a (d x 1) vector, containing the initial guess on
% the matching translation T. Default: s - Q0 * S0 * m, where s and m are
% the centroids of the scene and model point-sets, respectively. This
% is the optimal translation assuming TOSET and FROMSET match
% bijectively and Q0 = Q, and S0 = S.
%
% MINITERATIONS ('minIterations') is a positive integer which specifies 
% the minimum number of iterations for the algorithm to take.
% Default: 1.
%
% MAXITERATIONS ('maxIterations') is a positive integer which 
% specifies the maximum number of iterations for the algorithm to take. 
% It must hold that MINITERATIONS <= MAXITERATIONS.
% Default: 100.
%
% MINERROR ('minError') is the minimum error under 
% which to accept the transformation and stop iteration. 
% Default: 1e-11.
%
% It should approximately be true that 
% 
%     Q * S * fromSet + t * ones(1, n)
%
% matches toSet.

% Description: Point-set registration between unpaired point-sets.
% Detail: Coherent Point Drift algorithm.
% Documentation: coherent_point_drift.txt

function [Q, S, t, sigma2] = coherent_point_drift(...
    fromSet, toSet, varargin)

eval(import_pastel);

d = size(toSet, 1);

% Optional input arguments
noiseRatio = 0.2;
minIterations = 1;
maxIterations = 100;
minError = 1e-11;
matrix = 'free';
scaling = 'free';
translation = 'free';
orientation = 0;
Q0 = [];
S0 = [];
t0 = [];

eval(process_options({...
    'noiseRatio', ...
    'minIterations', 'maxIterations', ...
    'minError', 'matrix', 'scaling', 'translation', ...
    'orientation', 'Q0', 'S0', 't0'}, ...
    varargin));

[Q, S, t, sigma2] = pastelgeometrymatlab(...
    'coherent_point_drift', ...
    fromSet, ...
    toSet, ...
    noiseRatio, ...
    matrix, ...
    scaling, ...
    translation, ...
    orientation, ...
    Q0, ...
    S0, ...
    t0, ...
    minIterations, ...
    maxIterations, ...
    minError);

end
