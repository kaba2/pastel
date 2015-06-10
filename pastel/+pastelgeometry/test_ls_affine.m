% Description: Testing for ls_affine.m.
% DocumentationOf: ls_affine.m

function test_ls_affine()

clear all;
close all;

eval(import_pastel);

trials = 400;
threshold = 1e-11;

% Randomly chosen cases.

fails = 0;
for k = 1 : trials
    m = randi(10);
    n = randi(100) + 10;
    W = eye(n) * rand() * 10;

    matrixSet = {'free', 'identity'};
    matrix = matrixSet{randi([1, numel(matrixSet)])};
    scalingSet = {'free', 'conformal', 'rigid'};
    scaling = scalingSet{randi([1, numel(scalingSet)])};
    translationSet = {'free', 'identity'};
    translation = translationSet{randi([1, numel(translationSet)])};
    orientationSet = {-1, 0, 1};
    orientation = orientationSet{randi([1, numel(orientationSet)])};
    
    if strcmp(scaling, 'free') || strcmp(matrix, 'identity')
        % Orientation can not be forced when scaling is free or
        % the matrix Q is identity.
        orientation = 0;
    end
       
    Q = eye(m, m);
    if strcmp(matrix, 'free')
        Q = random_orthogonal(m, 'orientation', orientation);
    end
    
    S = eye(m, m);
    if strcmp(scaling, 'free')
        S = randn(m, m);
        S = S + S';
        if strcmp(matrix, 'free')
            [U, D, V] = svd(Q * S);
            Q = U * V';
            S = V * D * V';
        end
    end
    if strcmp(scaling, 'conformal')
        s = abs(randn(1, 1) * 5);
        S = s * eye(m, m);
    end
    
    t = zeros(m, 1);
    if strcmp(translation, 'free')
        t = randn(m, 1) * 10;
    end

    % Generate test point-sets.
    P = randn(m, n);
    R = Q * S * P + t * ones(1, n);

    % Compute the transformation back by least-squares.
    [QE, SE, tE] = ls_affine(...
        P, R, ...
        'orientation', orientation, ...
        'matrix', matrix, ...
        'scaling', scaling, ...
        'translation', translation, ...
        'W', W);
    
    % Check that the errors are small.
    if norm(QE - Q) > threshold || ...
       norm(SE - S) > threshold || ...
       norm(tE - t) > threshold || ...
       (orientation ~= 0 && sign(det(QE * SE)) ~= sign(orientation))
        fails = fails + 1;
    end
end

if fails > 0
    failPercent = num2str((fails / trials) * 100);
    error(['Computation failed in ', num2str(failPercent), '% cases']);
end
