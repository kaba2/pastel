% AS_POINTS
% Returns the coordinates of an identifier point-set.
%
% pointSet = as_points(idSet)
%
% where
%
% IDSET is a numeric array whose linearization contains
% n point identifiers, whose coordinates are to be 
% returned. Native type: integer.
%
% Return values
% -------------
%
% POINTSET is an (m x n) real array, which contains n
% points of dimension m.
%
% If a point identifier is not part of the kd-tree, a
% NaN vector is returned for that point.

% Description: Returns the coordinates of an identifier point-set.
% DocumentationOf: PointKdTree.m

function pointSet = as_points(self, idSet)
    eval(import_pastel);

    pointSet = pastelgeometrymatlab(...
        'pointkdtree_as_points', ...
        self.kdTree, idSet);
end
