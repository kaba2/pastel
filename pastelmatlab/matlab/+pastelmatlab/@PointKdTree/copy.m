% COPY
% Makes a copy of a kd-tree.
%
% kdTreeCopy = copy()
%
% Return values
% -------------
%
% KDTREECOPY is a copy of 'kdTree'.

% Description: Makes a copy of a kd-tree.
% DocumentationOf: PointKdTree.m

function kdTreeCopy = copy(self)
    eval(import_pastel);
    
    kdTreeCopy = pastelgeometry.PointKdTree(self.dimension());

    kdTreeCopy.kdTree = pastelmatlab(...
        'pointkdtree_copy', ...
        self.kdTree);
end
