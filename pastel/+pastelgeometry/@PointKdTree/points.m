% POINTS
% Returns the number of points in a kd-tree.
%
% N = points()
%
% Return values
% -------------
%
% N is the number of points in the kd-tree.

function N = points(self)
    eval(import_pastel);

    N = pastelgeometrymatlab(...
        'pointkdtree_points', ...
        self.kdTree);
end
