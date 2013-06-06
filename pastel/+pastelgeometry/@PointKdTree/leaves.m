% LEAVES
% Returns the number of leaf nodes in a kd-tree.
%
% L = leaves()
%
% Return values
% -------------
%
% L is the number of leaf nodes in the kd-tree.

function L = leaves(self)
    eval(import_pastel);

    L = pastelgeometrymatlab(...
        'pointkdtree_leaves', ...
        self.kdTree);
end
