% DELETE
% Destructs a kd-tree.
%
% delete()

% Description: Destructs a kd-tree.
% DocumentationOf: PointKdTree.m

function delete(self)
    eval(import_pastel);
    
    pastelmatlab(...
        'pointkdtree_destruct', ...
        self.kdTree)
end
