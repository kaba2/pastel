% CLEAR
% Removes points and structure from a kd-tree.
%
% clear()

function clear(self)
    eval(import_pastel);

    pastelgeometrymatlab(...
        'pointkdtree_clear', ...
        self.kdTree)
end
