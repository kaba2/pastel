% POINTS
% Returns the number of points in a kd-tree.
%
% N = points()
%
% Return values
% -------------
%
% N is the number of points in the kd-tree.

% Description: Returns the number of points in a kd-tree.
% DocumentationOf: PointKdTree.m

function N = points(self)
    eval(import_pastel);

    N = pastelmatlab(...
        'pointkdtree_points', ...
        self.kdTree);
end
