% CLEAR
% Removes points and structure from a kd-tree.
%
% clear()

% Description: Removes points and structure from a kd-tree.
% DocumentationOf: PointKdTree.m

function clear(self)
    eval(import_pastel);

    pastelmatlab(...
        'pointkdtree_clear', ...
        self.kdTree)
end
