clear all;
close all;

d = 8;
n = 10000;

angleSet = zeros(d, n);
for i = 1 : n
     Q = random_rotation(d);
     assert(det(Q) > 0);
     E = eig(Q);
     angleSet(:, i) = angle(E);
end

figure;
hist(angleSet(:), 100);
H = hist(angleSet(:), 100);

