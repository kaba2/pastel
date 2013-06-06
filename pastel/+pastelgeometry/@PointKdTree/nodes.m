% NODES
% Returns the number of nodes in a kd-tree.
%
% N = nodes()
%
% Return values
% -------------
%
% N is the number of nodes in the kd-tree.

function N = nodes(self)
    eval(import_pastel);

    N = pastelgeometrymatlab(...
        'pointkdtree_nodes', ...
        self.kdTree);
end
