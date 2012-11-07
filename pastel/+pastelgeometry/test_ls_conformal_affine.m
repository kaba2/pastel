% Description: Testing for ls_conformal_affine.m.
% DocumenationOf: ls_conformal_affine.m

function test_ls_conformal_affine()

eval(import_pastel);

m = 10;
n = 100;
trials = 100;
threshold = 1e-11;

fails = 0;
for k = 1 : trials
    % Generate a random conformal-affine transformation.
    orientation = 2 * randi([0, 1]) - 1;
    Q = random_orthogonal(m, 'orientation', orientation);
    t = randn(m, 1) * 10;
    s = abs(randn(1, 1) * 5);

    % Generate test point-sets.
    P = randn(m, n);
    R = s * Q * P + t * ones(1, n);

    % Compute the transformation back by least-squares.
    [QE, tE, sE] = ls_conformal_affine(P, R, 'orientation', orientation);

    % Check that the errors are small.
    if norm(QE - Q) > threshold || ...
       norm(tE - t) > threshold || ...
       norm(sE - s) > threshold || ...
       sign(det(QE)) ~= sign(orientation)
        fails = fails + 1;
    end
end

if fails > 0
    failPercent = num2str((fails / trials) * 100);
    error(['Computation failed in ', num2str(failPercent), '% cases']);
end
