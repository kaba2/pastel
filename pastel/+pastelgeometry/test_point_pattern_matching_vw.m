% Description: Testing for point_pattern_matching_vw.m.
% DocumentationOf: point_pattern_matching_vw.m

clear all;
close all;

eval(import_pastel);

% This program demonstrates the use of the point-pattern matching
% algorithm.

% At least 90% of the transformed model points must 
% each match a unique scene point. 'To match' means
% to be at a matching distance.
minMatchRatio = 0.9;

% The matching distance is given relative to the
% expected distance to the nearest neighbor, under
% the assumption of uniform distribution in a sphere.
relativeMatchingDistance = 0.01;

% This should correspond roughly to the probability that
% if the algorithm finds no match, then there really is no
% match.
confidence = 0.95;

% The amplitude of the gaussian noise to apply to the model
% points in the scene.
noiseAmount = 0;

% Generate a random set of model points.
% m     | time for non-match with confidence 0.95
% ------+----------------------------------------
% 10000 | 3667s ~= 1h
%  2500 |  176s ~= 3min
%  1250 |   45s
%   625 |   11s

m = 1250;
M = randn(2, m);

% Generate a random similarity.
rotation = rand() * 2 * pi;
scaling = rand() * 5;
translation = rand(2, 1) * 5;

noise = noiseAmount * randn(2, m);

% Transform the model points by a known similarity and apply noise.
Q = [cos(rotation), -sin(rotation); ...
    sin(rotation), cos(rotation)];
S = scaling * Q * M + kron(ones(1, m), translation) + scaling * noise;

% Permute the pointsets randomly so that the
% algorithm surely has no way to take advantage of 
% pairing.
n = size(S, 2);
S = S(:, randperm(n));
M = M(:, randperm(m));

% Attempt to recover the similarity from the two
% pointsets using point-pattern matching.
tic
[similarity, success] = ...
    point_pattern_matching_vw(M, S, ...
    minMatchRatio, relativeMatchingDistance, ...
    confidence);
timeSpent = toc;

nScaling = similarity(1);
nRotation = similarity(2);
nTranslation = similarity(3 : 4)';

% Output the results.
if success
    fprintf('Found a match!\n');
else
    fprintf('No match was found.\n');
end
fprintf('The matching took %f s.\n', timeSpent);
fprintf('Here are the returned parameters, ');
fprintf('correct parameters in parentheses:\n');
fprintf('Scale:    %f (%f)\n', ...
    nScaling, scaling);
fprintf('Rotation: %f (%f)\n', ...
    nRotation, rotation);
fprintf('xDelta:   %f (%f)\n', ...
    nTranslation(1), translation(1));
fprintf('yDelta:   %f (%f)\n\n', ...
    nTranslation(2), translation(2));

% Visualize the point-sets.
nQ = [cos(nRotation), -sin(nRotation); ...
    sin(nRotation), cos(nRotation)];
nM = nScaling * nQ * M + kron(ones(1, m), nTranslation);

figure;
hold on;    
title('Transformed model points (blue) vs scene points (red)');
scatter(S(1, :), S(2, :), 100, 'rx');
scatter(nM(1, :), nM(2, :), 100, 'b');
hold off;
