% MERGE
% Flattens a kd-tree into a single node.
%
% merge()

function merge(self)
    eval(import_pastel);

    pastelgeometrymatlab(...
        'pointkdtree_merge', ...
        self.kdTree)
end
