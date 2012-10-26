clear all;
close all;

% Dimension of the point-sets.
m = 2;

% Generate a random point-set P.
%n = 100;
%P = randn(m, n);

% Number of points to generate to the secondary cluster.
%n2 = 0;
n2 = 100;

% Ratio of points to keep in P.
%pAlpha = 0.8;
pAlpha = 1;
% Ratio of points to keep in R.
%rAlpha = 0.5;
rAlpha = 1;

% Load a point-set from a SQUID file.
file = fopen('fish.txt', 'rt');
% Read the number of points.
n = fscanf(file, '# %d');
% Read the points.
P = fscanf(file, ' %d %d');
P = reshape(P, [2, n]);
fclose(file);

% Generate a random transformation.
%Q = random_rotation(m);
Q = eye(m, m);
t = (2 * rand(m, 1) - 1) * 1000;
%t = zeros(m, 1);

% Transform P augmented with the secondary cluster
% to get the point-set R.
PP = [P, randn(m, n2) + [4; 0] * ones(1, n2)];
R = Q * PP + t * ones(1, size(PP, 2));

% Permute R.
rPermutation = randperm(size(R, 2));
rPermutation = rPermutation(:, 1 : floor(size(R, 2) * rAlpha));
R = R(:, rPermutation);

% Permute P.
pPermutation = randperm(size(P, 2));
pPermutation = pPermutation(:, 1 : floor(size(P, 2) * pAlpha));
P = P(:, pPermutation);

% Find the common points in P and R.
commonSet = intersect(pPermutation, rPermutation);
alpha = size(commonSet, 2) / size(P, 2);

% Find the transformation from P to R using the ICP.
[qIcp, tIcp] = icp(P, R, ...
    'matchingRatio', 0.5, ...
    'minIterations', 100, ...
    'maxIterations', 100, ...
    't0', zeros(m, 1));
rIcp = qIcp * P + tIcp * ones(1, size(P, 2));

% Find the transformation from P to R using our PPM.
[pairSet, tPpm, stability, success] = ...
    point_pattern_matching_kr(P, R, 10, ...
    'minMatchRatio', alpha * 0.9, ...
    'maxBias', 0.2, ...,
    'matchingMode', 0);
qPpm = eye(m, m);
rPpm = qPpm * P + tPpm * ones(1, size(P, 2));

if ~success
    disp('ppm did not succeed.')
end

% Draw a nice picture.
figure;
scatter(R(1, :), R(2, :), 'r')
hold on
axis equal
scatter(rIcp(1, :), rIcp(2, :), 'b.')
scatter(rPpm(1, :), rPpm(2, :), 'g.')
for i = 1 : size(R, 2)
    k = rPermutation(i);
    j = find(pPermutation == k);
    if ~isempty(j)
        L = line([R(1, i); rIcp(1, j)], [R(2, i), rIcp(2, j)]);
        set(L, 'Color', [0, 0, 1]);
        L = line([R(1, i); rPpm(1, j)], [R(2, i), rPpm(2, j)]);
        set(L, 'Color', [0, 1, 0]);
    end
end
title('ICP vs PPM')
legend('Goal', 'ICP', 'PPM')
hold off


