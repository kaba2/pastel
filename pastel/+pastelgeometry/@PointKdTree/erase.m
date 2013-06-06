% ERASE
% Removes a set of points from a kd-tree.
%
% erase(idSet)
%
% where
%
% IDSET is an arbitrary-dimensional numeric array which 
% contains point identifiers as returned by 'insert'.
% Native type: integer.
%
% If 'idSet' is not given, then all points are removed.

function erase(self, idSet)
    eval(import_pastel);

    if nargin < 2
        pastelgeometrymatlab(...
            'pointkdtree_erase', ...
            self.kdTree);
    else
        pastelgeometrymatlab(...
            'pointkdtree_erase', ...
            self.kdTree, idSet);
    end
end
