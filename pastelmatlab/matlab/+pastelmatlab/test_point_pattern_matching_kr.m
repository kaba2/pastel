% Description: Testing for point_pattern_matching_kr.m.
% Documentation: match_points_kr.txt

clear all;
close all;

eval(import_pastel);

drawCircles = true;

% This program demonstrates the use of the point-pattern matching
% algorithm.

% At least minMatchRatio * 100% of the transformed model points must 
% each match a unique scene point. 'To match' means
% to be at a matching distance.
minMatchRatio = 0.6;
%minMatchRatio = 1;

% The matching distance.
matchingDistance = 0.1;
%matchingDistance = 0.2;
%matchingDistance = 0.5;

% The maximum allowed bias.
maxBias = 0.2;

% 0 for first match, 1 for most stable maximum match.
matchingMode = 1;

% The amplitude of the gaussian noise to apply to the model
% points in the scene.
noiseAmount = 0.02;
%noiseAmount = 0;

missingPointsPercentage = 0.3;

% Generate a random set of model points.

d = 2;
m = 200;
M = randn(d, m);

missingPoints = min(floor(m * missingPointsPercentage), m);

% Generate a random translation.
translation = rand(d, 1) * 5;

noise = noiseAmount * randn(d, m);

% Transform the model points by a known translation and apply noise.
S = M + kron(ones(1, m), translation) + noise;

% Permute the pointsets randomly so that the
% algorithm surely has no way to take advantage of 
% pairing.

scenePermutation = randperm(m);
modelPermutation = randperm(m);

S = S(:, scenePermutation);
M = M(:, modelPermutation);

% Get rid of some of the scene points.
S = S(:, 1 : m - missingPoints);

n = size(S, d);

% Attempt to recover the similarity from the two
% pointsets using point-pattern matching.
tic
match = point_pattern_matching_kr(...
    M, S, matchingDistance, ...
    'minMatchRatio', minMatchRatio, ...
    'maxBias', maxBias, ...
    'matchingMode', matchingMode);
timeSpent = toc;

pairSet = match.pairSet;
nTranslation = match.translation;
bias = match.bias;
success = match.success;

foundPairSet = zeros(1, m);
foundPairSet(modelPermutation(pairSet(1, :))) = ...
    scenePermutation(pairSet(2, :));

correctPairSet = 1 : m;
correctPairs = sum(foundPairSet == correctPairSet);
%A = [correctPairSet; foundPairSet]'

% Output the results.

matchSize = size(pairSet, 2);

if success
    fprintf('Found a match with %d points!\n', matchSize);
    fprintf('Out of these points, %d match with the correct pair.\n', ...
        correctPairs);
    fprintf('The match has bias %f.\n', bias);
    fprintf('Here are the returned parameters, ');
    fprintf('correct parameters in parentheses:\n');
    fprintf('xDelta:   %f (%f)\n', ...
        nTranslation(1), translation(1));
    fprintf('yDelta:   %f (%f)\n', ...
        nTranslation(2), translation(2));
else
    fprintf('No match was found.\n');
end
fprintf('The matching took %f s.\n\n', timeSpent);

% Transform the model points by the found translation.
nM = M + kron(ones(1, m), nTranslation);

% Visualize the results.
figure;
hold on;

title('Transformed model points (red) vs scene points (blue)');
axis equal;

% Draw a blue circle around each scene point,
% with the matching distance as the radius.
if drawCircles
    [x, y, ignore] = cylinder(matchingDistance, 100);
    for i = 1 : n
        plot(x(1, :) + S(1, i), ...
            y(1, :) + S(2, i), 'b');
    end
end

% Draw a blue dot to each scene point.
scatter(S(1, :), S(2, :), 100, 'b.');

% Draw a red dot to each transformed model point.
scatter(nM(1, :), nM(2, :), 100, 'r.');

% Find out the inverse permutation of the model points.
[ignore, modelPermutationInv] = sort(modelPermutation);

% Draw some useful line segments.
for i = 1 : matchSize
    modelIndex = pairSet(1, i);
    sceneIndex = pairSet(2, i);
    correctModelIndex = modelPermutationInv(...
        scenePermutation(sceneIndex));

    % Draw a black line segment to the paired model point.
    line([S(1, sceneIndex); nM(1, modelIndex)], ...
        [S(2, sceneIndex); nM(2, modelIndex)], ...
        'Color', 'black');
    
    if modelIndex ~= correctModelIndex
        % Draw a red line segment to the correct model point
        % (if the paired model point is not the same).
        line([S(1, sceneIndex); nM(1, correctModelIndex)], ...
            [S(2, sceneIndex); nM(2, correctModelIndex)], ...
            'Color', 'red');
    end
end
    
hold off;
