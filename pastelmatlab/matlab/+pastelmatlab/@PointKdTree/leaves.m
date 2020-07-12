% LEAVES
% Returns the number of leaf nodes in a kd-tree.
%
% L = leaves()
%
% Return values
% -------------
%
% L is the number of leaf nodes in the kd-tree.

% Description: Returns the number of leaf nodes in a kd-tree.
% DocumentationOf: PointKdTree.m

function L = leaves(self)
    eval(import_pastel);

    L = pastelmatlab(...
        'pointkdtree_leaves', ...
        self.kdTree);
end
