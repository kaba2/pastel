% Description: Testing for ICP
% DocumentationOf: icp.m

function test_icp()

clear all;
close all;

eval(import_pastel);

% Dimension of the point-sets.
d = 2;

% Number of points to generate to the secondary cluster.
%n2 = 0;
n2 = 100;

matchingDistance = 5;

% Ratio of points to keep in P.
pAlpha = 0.2;
%pAlpha = 1;
% Ratio of points to keep in R.
rAlpha = 0.5;
%rAlpha = 0.9;
%rAlpha = 1;

useFish = true;
if useFish
    % Load a point-set P from a SQUID file.
    file = fopen('+pastelgeometry\fish.txt', 'rt');
    % Read the number of points.
    n = fscanf(file, '# %d');
    % Read the points.
    P = fscanf(file, ' %d %d');
    P = reshape(P, [2, n]);
    fclose(file);
else
    % Generate a random point-set P.
    n = 100;
    P = randn(d, n) * 100;
end

% Generate a random transformation.
%Q = random_orthogonal(d, 'orientation', 1);
Q = eye(d, d);
t = (2 * rand(d, 1) - 1) * 1000;
%t = zeros(d, 1);

% Transform P augmented with the secondary cluster
% to get the point-set R.
PP = [P, randn(d, n2) * 100 + [700; 0] * ones(1, n2)];
R = Q * PP + t * ones(1, size(PP, 2));

% Permute R.
rPermutation = randperm(size(R, 2));
rPermutation = rPermutation(1 : floor(size(R, 2) * rAlpha));
R = R(:, rPermutation);

% Permute P.
pPermutation = randperm(size(P, 2));
pPermutation = pPermutation(1 : floor(size(P, 2) * pAlpha));
P = P(:, pPermutation);

% Find the common points in P and R.
commonSet = intersect(pPermutation, rPermutation);
alpha = size(commonSet, 2) / size(P, 2);

% Note: when there is an additional cluster of points,
% scaling modes other than rigid do not produce a 
% good solution. I don't know whether this is a feature
% or a bug. Without the additional points the other 
% scaling modes work also.
[qCpd, sCpd, tCpd] = coherent_point_drift(...
    P, R, ...,
    'noiseRatio', 0.5, ...
    'matrix', 'identity', ...
    'scaling', 'rigid', ...
    'translation', 'free');

rCpd = qCpd * P + tCpd * ones(1, size(P, 2));

function icpDraw(match)
    eval(import_pastel);

    if mod(match.iteration, 20) == 0
        draw_matching(match.transformedSet, match.sceneSet, match.pairSet);
        title(['ICP iteration ', num2str(match.iteration)]);
    end
end

% Find the transformation from P to R using the ICP.
match = icp(P, R, matchingDistance, ...
    'matrix', 'identity', ...
    'reporter', @icpDraw);
qIcp = match.Q;
tIcp = match.t;
rIcp = qIcp * P + tIcp * ones(1, size(P, 2));

% Find the transformation from P to R using our PPM.
match = point_pattern_matching_kr(...
    P, R, matchingDistance, ...
    'minMatchRatio', alpha * 0.9, ...
    'maxBias', 0.1, ...,
    'matchingMode', 0);

pairSetPpm = match.pairSet;
tPpm = match.translation;
bias = match.bias;
success = match.success;

qPpm = eye(d, d);
rPpm = qPpm * P + tPpm * ones(1, size(P, 2));

if ~success
    disp('ppm did not succeed.')
end

% Draw a nice picture.
figure;
scatter(R(1, :), R(2, :), 'r.')
hold on
axis equal
scatter(rIcp(1, :), rIcp(2, :), 'g.')
scatter(rPpm(1, :), rPpm(2, :), 'b.')
scatter(rCpd(1, :), rCpd(2, :), 'c.')
% for i = 1 : size(R, 2)
%     k = rPermutation(i);
%     j = find(pPermutation == k);
%     if ~isempty(j)
%         L = line([R(1, i); rIcp(1, j)], [R(2, i), rIcp(2, j)]);
%         set(L, 'Color', [0, 1, 0]);
%         L = line([R(1, i); rPpm(1, j)], [R(2, i), rPpm(2, j)]);
%         set(L, 'Color', [0, 0, 1]);
%     end
% end
title('ICP vs PPM vs CPD')
legend('Goal', 'ICP', 'PPM', 'CPD')
hold off

end