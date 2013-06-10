% MERGE
% Flattens a kd-tree into a single node.
%
% merge()

% Description: Flattens a kd-tree into a single node.
% DocumentationOf: PointKdTree.m

function merge(self)
    eval(import_pastel);

    pastelgeometrymatlab(...
        'pointkdtree_merge', ...
        self.kdTree)
end
