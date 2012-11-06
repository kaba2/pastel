% POINT_PATTERN_MATCHING_VW
% Finds a similarity between unordered 2d pointsets.
%
% [similarity, success] = point_pattern_matching_vw(
%     modelPointSet, scenePointSet, ...
%     minMatchRatio, relativeMatchingDistance, ...
%     confidence)
%
% where
%
% MODELPOINTSET is a (2 x m)-real-array, where each column contains
% a 2-dimensional point.
%
% SCENEPOINTSET is a (2 x n)-real-array, where each column contains
% a 2-dimensional point.
%
% MINMATCHRATIO is a real number in the range [0, 1]. It gives the 
% fraction of points in 'modelPointSet' that a similarity must match 
% with points in 'scenePointSet' to be accepted. For example, 0.8 means 
% that 80% of the points must match.
%
% RELATIVEMATCHINGDISTANCE is a real number in the range [0, 1]. It gives
% the matching distance as a fraction of the expected distance to a nearest
% neighbor in the 'scenePointSet', under the assumption of uniformly 
% distributed points in a sphere.
%
% CONFIDENCE is a real number in the range [0, 1]. It is roughly the 
% probability, that if the algorithm finds no match, then there really 
% is no match. Default: 0.99. 
%
% SIMILARITY is a (1 x 4)-real-array [scaling, rotation, xDelta, yDelta],
% where 'rotation' represents a rotation angle in radians, 'scaling' is a 
% non-negative number representing scaling, and [xDelta, yDelta] 
% represents a translation.
%
% SUCCESS is a real number, which is 1 if a match was found, and 0
% otherwise.
%
% To be more exact, by a similarity we mean here an orientation-preserving
% conformal affine transformation. It is given by:
%
% Q = [cos(rotation), -sin(rotation); ...
%      sin(rotation),  cos(rotation)];
% translation = [xDelta, yDelta];
% y = scaling * Q * x + translation;
%
% A transformed model-point is said to match if it has a unique scene 
% point in its matching distance, where distance is measured by the
% Euclidean norm.
%
% Assumptions
% -----------
%
% The algorithm makes the following assumptions, whose validity you should
% confirm before using this algorithm. 
%
% 1) m <= n
%
% 2) The searched-for pattern in the 'scenePointSet' should not have too 
% many additional points: searching for a constellation in the sky probably
% fails. The important thing here is that the local neighborhoods of the 
% model should map to mostly similar local neighborhoods of the scene, at
% least for some part of the model. Otherwise the algorithm can fail to 
% find a global match, because of failing to find a local match.
%
% 3) The performance of this algorithm relies on being able to reject 
% candidates based on local non-similarity. Therefore, if pointsets are 
% highly self-similar (local neighborhoods look the same), the algorithm 
% will have to do checking with whole pointsets, defeating performance. 
% However, the algorithm will still find a match if it exists, so in that
% sense the algorithm is robust against increasing self-similarity.
%
% In practice you should experiment with the algorithm with your 
% particular data to see if it fits for you.
%
% Notes
% -----
%
% The slowest case for the algorithm is when there is no match.
% The 'confidence' parameter should alleviate this greatly: for with
% high probability the first few model points do have a corresponding
% point in the scene if there is a match.
%
% I have timed the algorithm in the worst case by generating m model points
% at random, and making sure the m scene points do not match the model 
% points. Here are the results:
%
% m     | time for non-match with confidence 0.95
% ------+----------------------------------------
% 10000 | 3667s ~= 1h
%  2500 |  176s ~= 3min
%  1250 |   45s
%   625 |   11s
%
% The time needed to state a non-match seems to grow quadratically.
% I'm not sure if this is the expected behaviour.

% Description: Finds a similarity between unordered 2d pointsets
% Documentation: point_pattern_matching_vw.txt

function [similarity, success] = point_pattern_matching_vw(...
    modelPointSet, scenePointSet, ...
    minMatchRatio, relativeMatchingDistance, ...
    confidence)

eval(import_pastel);

if nargin < 5
    confidence = 0.99;
end

concept_check(...
    modelPointSet, 'pointset', ...
    scenePointSet, 'pointset', ...
    minMatchRatio, 'real', ...
    relativeMatchingDistance, 'real', ...
    confidence, 'real');

if minMatchRatio < 0 || minMatchRatio > 1
    error('minMatchRatio must be in the range [0, 1].');
end

if relativeMatchingDistance < 0 || relativeMatchingDistance > 1
    error('relativeMatchingDistance must be in the range [0, 1]');
end

if confidence < 0 || confidence > 1
    error('confidence must be in the range [0, 1].');
end

[similarity, success] = ...
    pastelgeometrymatlab('point_pattern_matching_vw', ...
    modelPointSet, scenePointSet, ...
    minMatchRatio, relativeMatchingDistance, ...
    confidence);


