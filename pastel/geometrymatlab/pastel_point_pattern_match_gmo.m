% PASTEL_POINT_PATTERN_MATCH_GMO
% Finds a translation between unordered pointsets.
%
% [pairSet, translation, success] = pastel_point_pattern_match(
%     modelPointSet, scenePointSet, ...
%     minMatchRatio, matchingDistance)
%
% where
%
% MODELPOINTSET is a (d x m)-real-array, where each column contains
% a d-dimensional point.
%
% SCENEPOINTSET is a (d x n)-real-array, where each column contains
% a d-dimensional point.
%
% MINMATCHRATIO is a real number in the range [0, 1]. It gives the 
% fraction of points in 'modelPointSet' that a translation must match 
% with points in 'scenePointSet' to be accepted. For example, 0.8 means 
% that 80% of the points must match.
%
% MATCHINGDISTANCE is a non-negative real number which gives the distance 
% under which a mapped point from the 'modelPointSet' is considered 
% matching a point from the 'scenePointSet'.
%
% SUCCESS is a real number, which is 1 if a match was found, and 0
% otherwise.
%
% TRANSLATION is a (d x 1)-real-array which gives the matching translation
% in case such is found ('success' is 1).
%
% A transformed model-point is said to match if it has a unique scene 
% point in its matching distance, where distance is measured by the
% Euclidean norm.

% Description: Finds a similarity between unordered 2d pointsets
% Documentation: point_pattern_match.txt

function [pairSet, translation, success] = ...
    pastel_point_pattern_match_gmo(...
    modelPointSet, scenePointSet, ...
    minMatchRatio, matchingDistance)

check(modelPointSet, 'pointset');
check(scenePointSet, 'pointset');
check(minMatchRatio, 'real');
check(matchingDistance, 'real');

if minMatchRatio < 0 || minMatchRatio > 1
    error('minMatchRatio must be in the range [0, 1].');
end

if matchingDistance < 0
    error('matchingDistance must be non-negative.');
end

if size(modelPointSet, 1) ~= size(scenePointSet)
    error('The dimensions of modelPointSet and scenePointSet must be equal.');
end

[pairSet, translation, success] = ...
    pastelgeometrymatlab('point_pattern_match_gmo', ...
    modelPointSet, scenePointSet, ...
    minMatchRatio, matchingDistance);


