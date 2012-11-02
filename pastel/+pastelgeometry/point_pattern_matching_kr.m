% POINT_PATTERN_MATCHING_KR
% Finds a translation between unpaired point-sets.
%
% [pairSet, translation, bias, success] = ...
%     point_pattern_matching_kr(modelPointSet, scenePointSet, 
%         matchingDistance, 'key', value, ...)
%
% where
%
% MODELPOINTSET is a (d x m)-real-array, where each column contains
% a d-dimensional point.
%
% SCENEPOINTSET is a (d x n)-real-array, where each column contains
% a d-dimensional point.
%
% MATCHINGDISTANCE ('matchingDistance') is a non-negative real number 
% which gives the distance under which a mapped point from the 
% 'modelPointSet' is considered to match a point from the 'scenePointSet'.
%
% Optional input arguments in 'key'-value pairs:
%
% KNEAREST ('kNearest') is a positive integer which specifies the number
% of nearest neighbor to search for each model point. The additional
% nearest neighbors are used in case the nearest neighbors coincide
% between model points; the algorithm then computes a maximum pairing
% from the available neighbors. Default: 16.
%
% MINMATCHRATIO ('minMatchRatio') is a real number in the range [0, 1]. 
% It gives the fraction of points in 'modelPointSet' that a translation 
% must match with points in 'scenePointSet' to be accepted. For example,
% 0.8 means that 80% of the points must match. Default: 1.
%
% MAXBIAS ('maxBias') is a real number in the range [0, 1] which gives 
% the maximum allowed bias for a pairing to be accepted as a match. See 
% below for the definition of bias. Default: 0.2
%
% MATCHINGMODE ('matchingMode') is a non-negative integer specifying which 
% match to return among all the matches. The values are: 
%     0: the first match, or
%     1: the best match (of maximum size, of minimum bias)
% Default: 0.
%
% PAIRSET is a (2 x k)-integer-array of indices, where each column is
% a pair (i, j), where i is the index of a scene point, and j is the index
% of its matched model point. 
%
% TRANSLATION is a (d x 1)-real-array which gives the matching translation
% in case such is found ('success' is 1).
%
% BIAS is a real number in the range [0, 1] which gives the bias of the
% found match. See below for the definition of bias.
%
% SUCCESS is a real number, which is 1 if a match was found, and 0
% otherwise.
%
% A transformed model-point is said to match if it has a unique scene 
% point in its matching distance, where distance is measured by the
% Euclidean norm.
%
% The bias is computed as the norm of the mean difference between the 
% paired points, divided by the matching distance. For example, if the 
% differences are uniformly scattered (due to noise), then the bias is 
% close to zero. On the other hand, if the pairs of each point are 
% located consistently at the same direction, then the bias is positive.
% Bias is indicative of an unoptimal translation, although the pairing 
% is correct. As an example, think of picking the scene point to align 
% with an extraneous point near an embedded model point.

% Description: Finds a matching translation between unordered pointsets
% Documentation: point_pattern_matching_kr.txt

function [pairSet, translation, bias, success] = ...
    point_pattern_matching_kr(...
    modelPointSet, scenePointSet, matchingDistance, varargin)

eval(import_pastel);

% Optional input arguments
kNearest = 16;
minMatchRatio = 1;
maxBias = 0.2;
matchingMode = 0;
eval(process_options({'kNearest', 'minMatchRatio', ...
    'matchingDistance', 'maxBias',  'matchingMode'}, ...
    varargin));

check(modelPointSet, 'pointset');
check(scenePointSet, 'pointset');
check(minMatchRatio, 'real');
check(matchingDistance, 'real');
check(maxBias, 'real');

if kNearest < 1
    error('kNearest must be at least 1.');
end

if minMatchRatio < 0 || minMatchRatio > 1
    error('minMatchRatio must be in the range [0, 1].');
end

if matchingDistance < 0
    error('matchingDistance must be non-negative.');
end

if maxBias < 0 || maxBias > 1
    error('maxBias must be in the range [0, 1].');
end

if matchingMode ~= 0 && matchingMode ~= 1
    error('matchingMode must be 0 or 1.');
end

if size(modelPointSet, 1) ~= size(scenePointSet, 1)
    error('The dimensions of modelPointSet and scenePointSet must be equal.');
end

[pairSet, translation, bias, success] = ...
    pastelgeometrymatlab('point_pattern_matching_kr', ...
    modelPointSet, scenePointSet, kNearest, ...
    minMatchRatio, matchingDistance, maxBias, ...
    matchingMode);


