% Description: Testing for random_orthogonal.m.
% DocumentationOf: random_orthogonal.m

function [] = test_random_orthogonal()

clear all;
close all;

figure;
drawAngleHistogram(@correctAlgorithm)
title('Eigenvalue-angle distribution (correct algorithm)')

figure;
drawAngleHistogram(@incorrectAlgorithm)
title('Eigenvalue-angle distribution  (incorrect algorithm)')

end

function Q = correctAlgorithm(d, orientation)
    eval(import_pastel);
    Q = random_orthogonal(d, 'orientation', orientation);
end

function Q = incorrectAlgorithm(d, orientation)
     [Q, R] = qr(randn(d, d));

     % Leaving this normalization step out
     % provides an incorrect algorithm.
     %Q(:, diag(R) < 0) = -Q(:, diag(R) < 0);

     if (orientation ~= 0 && sign(det(Q)) ~= orientation)
     	Q(:, 1) = -Q(:, 1);
     end
end

function y = drawAngleHistogram(algorithm)
	d = 8;
	n = 2000;

	angleSet = zeros(d, n);
	for i = 1 : n
	     orientation = 1;

	     Q = algorithm(d, orientation);

	     assert(orientation == 0 || sign(det(Q)) == sign(orientation));
	     E = eig(Q);
	     angleSet(:, i) = angle(E);
	end

	hist(angleSet(:), 100);
end