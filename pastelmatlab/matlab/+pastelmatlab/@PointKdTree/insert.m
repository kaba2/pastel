% INSERT
% Inserts points into a kd-tree.
%
% idSet = insert(pointSet)
%
% where
%
% POINTSET is an (m x n) numeric array which contains n points
% of dimension m. The m must match with the dimension
% of the kd-tree. Native type: real.
%
% Return values
% -------------
%
% IDSET is a positive integer array containing the identifiers 
% of the inserted points in the kd-tree.

% Description: Inserts points into a kd-tree
% DocumentationOf: PointKdTree.m

function idSet = insert(self, pointSet)
    eval(import_pastel);

    if self.dimension() ~= size(pointSet, 1)
        error('Dimensions of the kd-tree and the point-set do not match.');
    end

    idSet = pastelmatlab(...
        'pointkdtree_insert', ...
        self.kdTree, pointSet);
end
