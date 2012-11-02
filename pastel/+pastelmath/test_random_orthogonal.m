% Description: Testing for random_orthogonal.m.
% DocumentationOf: random_orthogonal.m

clear all;
close all;

d = 8;
n = 10000;

angleSet = zeros(d, n);
for i = 1 : n
     orientation = 1;
     Q = random_orthogonal(d, 'orientation', orientation);
     assert(sign(det(Q)) == sign(orientation));
     E = eig(Q);
     angleSet(:, i) = angle(E);
end

figure;
hist(angleSet(:), 100);
H = hist(angleSet(:), 100);

