function draw_matching(aSet, bSet, pairSet)

% Draw a nice picture.
figure;
scatter(aSet(1, :), aSet(2, :), 'b.');
%axis([-10, 10, -10, 10]);
axis equal;
hold on;
scatter(bSet(1, :), bSet(2, :), 'r.');
for i = 1 : size(pairSet, 2)
    u = pairSet(1, i);
    v = pairSet(2, i);
    line([aSet(1, u), bSet(1, v)], ...
        [aSet(2, u), bSet(2, v)]);
end        
hold off;
