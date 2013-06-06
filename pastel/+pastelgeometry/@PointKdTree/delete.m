% DELETE
% Destructs a kd-tree.
%
% delete()

function delete(self)
    eval(import_pastel);
    
    pastelgeometrymatlab(...
        'pointkdtree_destruct', ...
        self.kdTree)
end
