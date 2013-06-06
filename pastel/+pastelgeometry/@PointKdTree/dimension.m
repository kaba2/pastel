% DIMENSION
% Returns the dimension of a kd-tree.
%
% D = dimension()
%
% Return values
% -------------
%
% D is the dimension of the tree.

function D = dimension(self)
    eval(import_pastel);

    D = pastelgeometrymatlab(...
        'pointkdtree_dimension', ...
        self.kdTree);
end
