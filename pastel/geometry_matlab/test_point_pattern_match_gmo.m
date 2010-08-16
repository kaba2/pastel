clear all;
close all;

% This program demonstrates the use of the point-pattern matching
% algorithm.

% At least 90% of the transformed model points must 
% each match a unique scene point. 'To match' means
% to be at a matching distance.
minMatchRatio = 0.9;

matchingDistance = 0.01;

% This should correspond roughly to the probability that
% if the algorithm finds no match, then there really is no
% match.
confidence = 0.95;

% The amplitude of the gaussian noise to apply to the model
% points in the scene.
noiseAmount = 0.02;

% Generate a random set of model points.

d = 2;
m = 100;
M = randn(d, m);

% Generate a random translation.
translation = rand(d, 1) * 5;

noise = noiseAmount * randn(d, m);

% Transform the model points by a known translation and apply noise.
S = M + kron(ones(1, m), translation) + noise;

% Permute the pointsets randomly so that the
% algorithm surely has no way to take advantage of 
% pairing.
n = size(S, d);
S = S(:, randperm(n));
M = M(:, randperm(m));

% Attempt to recover the similarity from the two
% pointsets using point-pattern matching.
tic
[pairSet, nTranslation, success] = ...
    pastel_point_pattern_match_gmo(M, S, ...
    minMatchRatio, matchingDistance, ...
    confidence);
timeSpent = toc;

pairSet

% Output the results.
if success
    fprintf('Found a match!\n');
else
    fprintf('No match was found.\n');
end
fprintf('The matching took %f s.\n', timeSpent);
fprintf('Here are the returned parameters, ');
fprintf('correct parameters in parentheses:\n');
fprintf('xDelta:   %f (%f)\n', ...
    nTranslation(1), translation(1));
fprintf('yDelta:   %f (%f)\n\n', ...
    nTranslation(2), translation(2));

% Visualize the point-sets.
nM = M + kron(ones(1, m), nTranslation);

figure;
hold on;    
title('Transformed model points (blue) vs scene points (red)');
scatter(S(1, :), S(2, :), 100, 'rx');
scatter(nM(1, :), nM(2, :), 100, 'b');
hold off;
