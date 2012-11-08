% Description: Testing for ls_affine.m.
% DocumentationOf: ls_affine.m

function test_ls_affine()

clear all;
close all;

eval(import_pastel);

m = 10;
n = 100;
trials = 100;
threshold = 1e-11;

fails = 0;
for k = 1 : trials
    % Generate a random affine transformation.
    s = 1;
    A = randn(m, m);
    t = randn(m, 1) * 10;

    % Generate test point-sets.
    P = randn(m, n);
    R = A * P + t * ones(1, n);

    % Compute the transformation back by least-squares.
    [AE, tE, sE] = ls_affine(P, R);

    % Check that the errors are small.
    if norm(AE - A) > threshold || ...
       norm(tE - t) > threshold || ...
       norm(sE - s) > threshold
        fails = fails + 1;
    end
end

if fails > 0
    failPercent = num2str((fails / trials) * 100);
    error(['Computation failed in ', num2str(failPercent), '% cases']);
end
