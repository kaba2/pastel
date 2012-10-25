clear all;
close all;

m = 10;
n = 100;
trials = 1000;
threshold = 1e-11;

fails = 0;
for k = 1 : trials
    % Generate a random rotation.
    M = randn(m, m) / sqrt(2);
    [Q, R] = qr(M);
    for i = 1 : m
        if R(i, i) < 0
            Q(:, i) = -Q(:, i);
        end
    end

    % Generate a random translation.
    t = randn(m, 1) * 10;

    % Generate a non-negative random scaling.
    s = randn(1, 1) * 5;
    if s < 0
        s = -s;
    end

    % Generate test point-sets.
    P = randn(m, n);
    R = s * Q * P + t * ones(1, n);

    % Compute the transformation back by least-squares.
    [QE, tE, sE] = ls_conformal_affine(P, R);

    % Check that the errors are small.
    if norm(QE - Q) > threshold || ...
       norm(tE - t) > threshold || ...
       norm(sE - s) > threshold
        fails = fails + 1;
    end
end

if fails > 0
    failPercent = num2str((fails / trials) * 100);
    error(['Computation failed in ', num2str(failPercent), '% cases']);
end